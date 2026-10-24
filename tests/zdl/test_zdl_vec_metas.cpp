// Issue #55: meta_base::vec_metas(const vector&) runs vec_metas_added() after
// every assignment, and libzdl writes back what vec_metas() returned, so the
// implicit markers the hooks append must not pile up.
//
// vec_metas_added() is an inline virtual reached through vtables that libzdl's
// translation units emit, so build libzdl from source against this include/
// tree: a prebuilt libzdl.lib runs the hooks of whatever header it was
// compiled with.
#include <gtest/gtest.h>

#include <string>
#include <utility>
#include <vector>

#include <zdl/zdl_builtin.h>
#include <zdl/zdl_member.h>
#include <zdl/zdl_module.h>
#include <zdl/zdl_struct.h>

namespace {

typedef std::vector<std::pair<std::string, std::string> > meta_vec;

meta_vec one_meta(const std::string& key, const std::string& val) {
    return meta_vec(1, std::make_pair(key, val));
}

int pair_count(const meta_base& value, const std::string& key, const std::string& val) {
    int count = 0;
    for (const auto& meta : value.vec_metas()) {
        if (meta.first == key && meta.second == val) ++count;
    }
    return count;
}

int key_count(const meta_base& value, const std::string& key) {
    int count = 0;
    for (const auto& meta : value.vec_metas()) {
        if (meta.first == key) ++count;
    }
    return count;
}

// The read-modify-write zdl_parser_context::add_current_member_metadata() does
// for comments in an empty struct body, without the pop_back() of a trailing
// ("msg", "1") that libzdl does before writing back.
void write_back_with(meta_base& value, const std::string& key, const std::string& val) {
    meta_vec metas = value.vec_metas();
    metas.push_back(std::make_pair(key, val));
    value.vec_metas(metas);
}

zdl_struct_ptr make_struct(const std::string& name) {
    return zdl_struct_ptr(new zdl_struct(zdl_module_ptr((zdl_module*)0), name));
}

zdl_member_ptr make_member(const zdl_struct_ptr& parent, const std::string& name,
                           const std::string& minsize, const std::string& maxsize,
                           zdl_type* type = 0) {
    return zdl_member_ptr(new zdl_member(parent.get(), 0, name, minsize, maxsize, type));
}

}  // namespace

TEST(ZdlStructVecMetasTest, WritingBackMsgStructMetadataKeepsOneImplicitMarker) {
    zdl_struct_ptr s = make_struct("MSGLogin");
    s->vec_metas(meta_vec());
    EXPECT_EQ(1, pair_count(*s, "msg", "1"));

    write_back_with(*s, "_trailing_comment", "tail");
    EXPECT_EQ(1, pair_count(*s, "msg", "1"));
    EXPECT_EQ(2u, s->vec_metas().size());

    std::string value;
    ASSERT_TRUE(s->fetch_meta("msg", value));
    EXPECT_EQ("1", value);
    ASSERT_TRUE(s->fetch_meta("_trailing_comment", value));
    EXPECT_EQ("tail", value);
}

TEST(ZdlStructVecMetasTest, ReassigningUnchangedMsgStructMetadataLeavesItAsIs) {
    zdl_struct_ptr s = make_struct("MSGLogin");
    s->vec_metas(one_meta("structmeta", "\"s\""));
    const meta_vec first = s->vec_metas();
    ASSERT_EQ(2u, first.size());

    s->vec_metas(first);
    EXPECT_EQ(first, s->vec_metas());
    EXPECT_EQ(1, pair_count(*s, "msg", "1"));
}

TEST(ZdlStructVecMetasTest, ExplicitMsgValueStaysAheadOfTheImplicitMarker) {
    // The explicit entry is kept as written and the implicit marker goes behind
    // it, so fetch_meta() returns the explicit value.
    zdl_struct_ptr s = make_struct("MSGCustom");
    s->vec_metas(one_meta("msg", "0"));
    write_back_with(*s, "_trailing_comment", "tail");

    ASSERT_FALSE(s->vec_metas().empty());
    EXPECT_EQ(std::make_pair(std::string("msg"), std::string("0")), s->vec_metas().front());
    EXPECT_EQ(1, pair_count(*s, "msg", "0"));
    EXPECT_EQ(1, pair_count(*s, "msg", "1"));

    std::string value;
    ASSERT_TRUE(s->fetch_meta("msg", value));
    EXPECT_EQ("0", value);
}

TEST(ZdlStructVecMetasTest, ExplicitMsgOneIsNotDuplicatedByTheImplicitMarker) {
    zdl_struct_ptr s = make_struct("MSGDefault");
    s->vec_metas(one_meta("msg", "1"));
    EXPECT_EQ(1u, s->vec_metas().size());
    EXPECT_EQ(1, pair_count(*s, "msg", "1"));
}

TEST(ZdlStructVecMetasTest, NonMsgStructGetsNoMarkerAndKeepsItsCountWhenReassigned) {
    zdl_struct_ptr s = make_struct("Login");
    s->vec_metas(one_meta("structmeta", "\"s\""));
    EXPECT_EQ(1u, s->vec_metas().size());

    s->vec_metas(meta_vec(s->vec_metas()));
    EXPECT_EQ(1u, s->vec_metas().size());

    write_back_with(*s, "_trailing_comment", "tail");
    EXPECT_EQ(2u, s->vec_metas().size());
    EXPECT_EQ(0, key_count(*s, "msg"));
}

TEST(ZdlMemberVecMetasTest, WritingBackOptionalArrayMetadataKeepsOneImplicitMarker) {
    zdl_struct_ptr parent = make_struct("Holder");
    zdl_member_ptr m = make_member(parent, "ids", "0", "10");
    ASSERT_TRUE(m->is_array());
    m->vec_metas(meta_vec());
    EXPECT_EQ(1, pair_count(*m, "o", ""));

    write_back_with(*m, "_previous_comment", "lead");
    EXPECT_EQ(1, pair_count(*m, "o", ""));
    EXPECT_EQ(2u, m->vec_metas().size());
    EXPECT_TRUE(m->is_optional());
}

TEST(ZdlMemberVecMetasTest, ExplicitOptionalMarkerOnOptionalArrayIsCountedOnce) {
    zdl_builtin_ptr int32_type(new zdl_builtin(ZDL_INT32));
    zdl_struct_ptr parent = make_struct("Holder");
    zdl_member_ptr m = make_member(parent, "ids", "0", "10", int32_type.get());
    m->vec_metas(one_meta("o", ""));
    EXPECT_EQ(1u, m->vec_metas().size());

    // zdl_struct indexes a member once per metadata entry it carries.
    ASSERT_EQ(0, parent->add_member(m));
    EXPECT_EQ(1, parent->get_member_count("o"));
}

TEST(ZdlMemberVecMetasTest, RequiredMembersGetNoMarkerAndKeepTheirCountWhenReassigned) {
    zdl_struct_ptr parent = make_struct("Holder");
    zdl_member_ptr scalar = make_member(parent, "id", "1", "1");
    zdl_member_ptr required = make_member(parent, "ids", "1", "10");
    ASSERT_FALSE(scalar->is_array());
    ASSERT_TRUE(required->is_array());

    for (const zdl_member_ptr& m : {scalar, required}) {
        m->vec_metas(one_meta("KEY1", ""));
        EXPECT_EQ(1u, m->vec_metas().size());
        m->vec_metas(meta_vec(m->vec_metas()));
        EXPECT_EQ(1u, m->vec_metas().size());
        EXPECT_EQ(0, key_count(*m, "o"));
    }
}

#ifndef USE_GTEST_MAIN
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
