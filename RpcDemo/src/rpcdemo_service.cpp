#include <zce/zce_config.h>
#include <zce/zce_filesystem.h>
#ifndef WIN32
#    include <uuid/uuid.h>
#endif

#include "rpcdemo_service.h"

#ifndef _WIN32
#    include <sys/types.h>
#    include <sys/wait.h>
#endif
#include <zce/zce_mbpool.h>
#include "rpcdemo_config.h"
#include "rpcdemo_config_proto.h"
#include "rpcdemo_config_pack.h"
#include <zce/zce_timer.h>

#include <zce/zvm.h>
#include <zce/zds_schema.h>
#include <zce/zdp_base_proto.h>
#include <zce/zdp_base_pack.h>

#include "rpcdemo_proto.h"
#include "rpcdemo_pack.h"
#ifdef WIN32
#    pragma comment(lib, "ws2_32.lib")
#    pragma comment(lib, "iphlpapi.lib")
#    pragma comment(lib, "Shlwapi.lib")
#    pragma comment(lib, "Userenv.lib")
#    pragma comment(lib, "Crypt32.lib")
#    pragma comment(lib, "libzlib.lib")
#    pragma comment(lib, "libzce.lib")
#endif

rpcdemo_service* g_service = NULL;

rpcdemo_service::rpcdemo_service()
    :timer_count_(0)
{
}

void rpcdemo_service::test_rpcdemo(const zce_object_ptr& vmobj)
{
    bool b = zdp::is_builtin_type<const char[6]>();
    
    ZTRACE("is_builtin_type<std::string>", b);

    vmptr_->rpc_call(vmobj, 0, "RpcCallVoid", zce_dblock{},
        [=](int error_code, const zce_dblock& retdata) {
            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
        }
    );

    vmptr_->rpc_call(vmobj, 0, "RpcCallInt32", 12345678, [=](int error_code, const zce_dblock& retdata) {
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
        if (error_code < 0) {
            return;
        }
        zce_int32 ival2 = 0;
        int ret = zdp::zds_unpack_builtin(ival2, retdata.rd_ptr(), (int)retdata.length(), 0);
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
        ZCE_ASSERT(ival2 == 12345678 * 2);
        });
#if 1
    std::vector<zce_int32> int32vec = { 12345678, 34567, 332323, 1212222 };
    vmptr_->rpc_call(vmobj, 0, "RpcCallInt32Vec", int32vec, [=](int error_code, const zce_dblock& retdata) {
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
        if (error_code < 0) {
            return;
        }
        std::vector<zce_int32> out_vec;
        int ret = zdp::zds_unpack_builtin(out_vec, retdata.rd_ptr(), (int)retdata.length(), 0);
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
        ZCE_ASSERT(int32vec == out_vec);
        });

    vmptr_->rpc_call(vmobj, 0, "RpcCallInt64", 123456789012345ll, [=](int error_code, const zce_dblock& retdata) {
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
        if (error_code < 0) {
            return;
        }
        zce_int64 ival2 = 0;
        int ret = zdp::zds_unpack_builtin(ival2, retdata.rd_ptr(), (int)retdata.length(), 0);
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
        ZCE_ASSERT(ival2 == 123456789012345ll * 2);
        });

    std::vector<zce_int64> int64vec = { 123456789012345ll, 34567564645600, 332564564564632300, 1212456456462220 };
    vmptr_->rpc_call(vmobj, 0, "RpcCallInt64Vec", int64vec, [=](int error_code, const zce_dblock& retdata) {
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
        if (error_code < 0) {
            return;
        }
        std::vector<zce_int64> out_vec;
        int ret = zdp::zds_unpack_builtin(out_vec, retdata.rd_ptr(), (int)retdata.length(), 0);
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
        ZCE_ASSERT(int64vec == out_vec);
        });

    vmptr_->rpc_call(vmobj, 0, "RpcCallFloat", 12345.6789, [=](int error_code, const zce_dblock& retdata) {
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
        if (error_code < 0) {
            return;
        }
        zce_float dval2 = 0;
        int ret = zdp::zds_unpack_builtin(dval2, retdata.rd_ptr(), (int)retdata.length(), 0);
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
        ZCE_ASSERT((dval2 - 12345.6789 * 2) < 1e-4);
        //ZCE_ASSERT(dval2 == 12345.6789 * 2);
        });

    std::vector<zce_float> flbvec = { 12345.6789f, 345.646456f, 3325.006323f, 121.00646222f };
    vmptr_->rpc_call(vmobj, 0, "RpcCallFloatVec", flbvec, [=](int error_code, const zce_dblock& retdata) {
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
        if (error_code < 0) {
            return;
        }
        std::vector<zce_float> out_vec;
        int ret = zdp::zds_unpack_builtin(out_vec, retdata.rd_ptr(), (int)retdata.length(), 0);
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
        ZCE_ASSERT(flbvec.size() == out_vec.size() && (flbvec[0] - out_vec[0]) < 1e-4);
        //ZCE_ASSERT(flbvec.size() == out_vec.size() && flbvec[0] * 2 == out_vec[0]);
        });

    vmptr_->rpc_call(vmobj, 0, "RpcCallDouble", 1234567890.12345, [=](int error_code, const zce_dblock& retdata) {
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
        if (error_code < 0) {
            return;
        }
        zce_double dval2 = 0;
        int ret = zdp::zds_unpack_builtin(dval2, retdata.rd_ptr(), (int)retdata.length(), 0);
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
        ZCE_ASSERT(dval2 == 1234567890.12345 * 2);
        });

    std::vector<zce_double> dlbvec = { 1234567890.12345, 34567500.00646456, 332564500.006323, 121245600.00646222 };
    vmptr_->rpc_call(vmobj, 0, "RpcCallDoubleVec", dlbvec, [=](int error_code, const zce_dblock& retdata) {
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
        if (error_code < 0) {
            return;
        }
        std::vector<zce_double> out_vec;
        int ret = zdp::zds_unpack_builtin(out_vec, retdata.rd_ptr(), (int)retdata.length(), 0);
        ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
        ZCE_ASSERT(dlbvec == out_vec);
        });

    zce_any subvec = zce_any::create_vector();
    subvec.vector().push_back(zce_any(12345));
    subvec.vector().push_back(zce_any("vector", 6));
    subvec.vector().push_back(zce_any(12345.67f));

    zce_any subdict = zce_any::create_dict();
    subdict.dict().insert(std::make_pair(zce_any("sub01", 5), zce_any(12345)));
    subdict.dict().insert(std::make_pair(zce_any("sub02", 5), zce_any(12345)));
    subdict.dict().insert(std::make_pair(zce_any("sub03", 5), zce_any(12345)));
    subdict.dict().insert(std::make_pair(zce_any("sub04", 5), subvec));
    subdict.dict().insert(std::make_pair(zce_any("sub05", 5), subdict));

    rpcdemo::ArgsMixed tMixed{};
    tMixed.anyVal = zce_any::create_dict();
    tMixed.anyVal.dict().insert(std::make_pair(zce_any("hello", 5), zce_any(1455555)));
    tMixed.anyVal.dict().insert(std::make_pair(zce_any("world", 5), subdict));
    tMixed.anyVal.dict().insert(std::make_pair(zce_any("float", 5), subvec));
    //tMixed.anyVal = subvec;
    tMixed.dblVal = 1234567890.12345;
    tMixed.dblValVec = { 1234567890.12345, -34567500.00646456, 332564500.006323, 121245600.00646222 };
    tMixed.fltVal = 12345.67f;
    tMixed.fltValVec = { 12345.6789f, 345.646456f, 3325.006323f, 121.00646222f };
    tMixed.i8Val = 126;
    tMixed.i8ValVec = { 126, -34, 33, 12 };
    tMixed.i16Val = 12345;
    tMixed.i16ValVec = { 12345, 345, -3323, 121 };
    tMixed.i32Val = 12345678;
    tMixed.i32ValVec = { -12345678, 34567, 332323, 1212222 };
    tMixed.i64Val = 123456789012345ll;
    tMixed.i64ValVec = { 123456789012345ll, -34567564645600, 332564564564632300, 1212456456462220 };
    tMixed.strVal = "hello world";

    vmptr_->rpc_call(vmobj, 0, "RpcCallMixed", tMixed,
        [=](int error_code, const zce_dblock& retdata) {
            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
            if (error_code < 0) {
                return;
            }
            rpcdemo::ArgsMixed outMixed{};
            int ret = zdp::zds_unpack(outMixed, retdata.rd_ptr(), (int)retdata.length(), 0, true);
            ZCE_ASSERT(ret >= 0);
            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
            ZCE_ASSERT(outMixed == tMixed);
        }
    );

    rpcdemo::ArgsDeepMixed tDeepMixed;
    //tDeepMixed.strValVec = { "hello world", "hello world", "hello world", "hello world" };
    tDeepMixed.ui8Val = 126;
    tDeepMixed.ui8ValVec = { 126, 34, 33, 12 };
    tDeepMixed.ui16Val = 12345;
    tDeepMixed.ui16ValVec = { 12345, 345, 3323, 121 };
    tDeepMixed.ui32Val = 12345678;
    tDeepMixed.ui32ValVec = { 12345678, 34567, 332323, 1212222 };
    tDeepMixed.ui64Val = 123456789012345ll;
    tDeepMixed.ui64ValVec = { 123456789012345ll, 34567564645600, 332564564564632300, 1212456456462220 };
    tDeepMixed.tMixed = tMixed;
    tDeepMixed.tMixedVec = { tMixed, tMixed, tMixed, tMixed };
    vmptr_->rpc_call(vmobj, 0, "RpcCallDeepMixed", tDeepMixed,
        [=](int error_code, const zce_dblock& retdata) {
            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
            if (error_code < 0) {
                return;
            }
            rpcdemo::ArgsDeepMixed outMixed{};
            int ret = zdp::zds_unpack(outMixed, retdata.rd_ptr(), (int)retdata.length(), 0, true);
            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
            ZCE_ASSERT(outMixed == tDeepMixed);
        }
    );

    vmptr_->rpc_call(vmobj, 0, "RpcCallDeepMixedVecArgs", tDeepMixed,
        [=](int error_code, const zce_dblock& retdata) {
            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
            if (error_code < 0) {
                return;
            }
            rpcdemo::ArgsDeepMixed outMixed{};
            int ret = zdp::zds_unpack(outMixed, retdata.rd_ptr(), (int)retdata.length(), 0, true);
            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
            ZCE_ASSERT(outMixed == tDeepMixed);
        }
    );

    vmptr_->rpc_call(vmobj, 0, "RpcCallDeepMixedVecArgsReturnList", tDeepMixed,
        [=](int error_code, const zce_dblock& retdata) {
            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
            if (error_code < 0) {
                return;
            }
            rpcdemo::ArgsDeepMixed outMixed{};
            int ret = zdp::zds_unpack(outMixed, retdata.rd_ptr(), (int)retdata.length(), 0, true);
            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
            ZCE_ASSERT(outMixed == tDeepMixed);
        }
    );

    vmptr_->rpc_call(vmobj, 0, "CreateDemoInstance", "Inst1",
        [=](int error_code, const zce_dblock& retdata) {
            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
            if (error_code < 0) {
                return;
            }
            zce_any any;
            int ret = zdp::zds_unpack_builtin(any, retdata.rd_ptr(), (int)retdata.length(), 0);
            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
            zce_object_wrapper<zdp_base::zobject_proxy_t>* proxy = any.get<zce_object_wrapper<zdp_base::zobject_proxy_t>>();
            ZCE_ASSERT(proxy && proxy->get().objid);

            vmptr_->rpc_call(vmobj, proxy->get().objid, "DemoInstanceSetStatus", tDeepMixed,
                [=](int error_code, const zce_dblock& retdata) {
                    ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
                    if (error_code < 0) {
                        return;
                    }
                    rpcdemo::ArgsDeepMixed outMixed{};
                    vmptr_->rpc_call(vmobj, proxy->get().objid, "DemoInstanceGetStatus", zce_dblock{},
                        [=](int error_code, const zce_dblock& retdata) {
                            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
                            if (error_code < 0) {
                                return;
                            }
                            rpcdemo::ArgsDeepMixed outMixed{};
                            int ret = zdp::zds_unpack(outMixed, retdata.rd_ptr(), (int)retdata.length(), 0, true);
                            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
                            ZCE_ASSERT(outMixed == tDeepMixed);
                        });
                });
        }
    );
#endif
}

bool rpcdemo_service::on_start(int argc, char* argv[])
{
    zce_init();
    zlog_param log_param{ "rpcdemo", 0 };

    zlog_init(0, &log_param);

    ZCE_DEBUG((ZLOG_TRACE, "start..."));

    int ret = rpcdemo_config_sigt::instance()->load("rpcdemo.xml");
    if (ret < 0) {
        ZCE_ASSERT(ret >= 0);
        return false;
    }

    zce_mbpool_sigt::instance()->add_pool(64, 128);
    zce_mbpool_sigt::instance()->add_pool(128, 64);
    zce_mbpool_sigt::instance()->add_pool(256, 32);
    zce_mbpool_sigt::instance()->add_pool(512, 32);
    zce_mbpool_sigt::instance()->add_pool(2048, 16);
    zce_mbpool_sigt::instance()->add_pool(16384, 16);
    zce_mbpool_sigt::instance()->add_pool(16*1024*1024, 4);

    rpcdemo::rpcdemo_server& conf = rpcdemo_config_sigt::instance()->get();

    zlog_setlevel(conf.info_.loglevel);

    zce_schedule_sigt::instance()->active(2);

    reactor_ptr_ = new zce_reactor();
    ret = reactor_ptr_->start();
    ZCE_ASSERT(ret >= 0);

    //rpcdemo_storm_sigt::instance()->listen("127.0.0.1", 36501);

    zce_smartptr<zce_timer_doozer> doozer_ptr(new zce_timer_tpl<rpcdemo_service>(this));
    timer_ = new zce_timer(reactor_ptr_, doozer_ptr, 1000);
    timer_->start();

    vmptr_ = new zvm(zce_smartptr<zce_schedule>(zce_schedule_sigt::instance()), reactor_ptr_);
    auto vmobj_local = vmptr_->boot("main", conf.info_.entry);
    ZCE_ASSERT(vmobj_local != 0);
    //auto vmobj = vmptr_->boot("main", "192.168.100.21", 36001);
    //ZCE_ASSERT(vmobj != 0);
    //test_tj010(vmobj);
    //vmobj_remote
    //auto vmobj = vmptr_->boot("rpcdemo", "127.0.0.1", 36001);
    //ZCE_ASSERT(vmobj != 0);
    //auto vmobj2 = vmptr_->boot("rpcdemo2", "127.0.0.1", 36001);
    //ZCE_ASSERT(vmobj2 != 0);

    //auto local_rpcdemo = vmptr_->get_vm("rpcdemo");

    //test_rpcdemo(local_rpcdemo);
    //test_rpcdemo(vmobj);
    //test_rpcdemo(vmobj2);
    //test_rpcdemo(vmobj);
    //test_rpcdemo(vmobj2);
    //test_rpcdemo(vmobj2);
    //test_rpcdemo(vmobj);

    //tj010::SetupModelInput setup_input{};

    //vmptr_->rpc_call(vmobj, "SetupModel", setup_input, [=](int error_code, const zce_dblock& retdata) {
    //    ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", error_code));
    //    if (error_code < 0) {
    //        return;
    //    }
    //    else {
    //        tj010::SetupModelOutput setup_output{};
    //        int ret = zdp::zds_unpack(setup_output, retdata.rd_ptr(), retdata.length(), 0);
    //        if (ret < 0) {
    //            ZCE_ERROR((ZLOG_ERROR, "rpc call ret: 0x%x", ret));
    //            return;
    //        }
    //        ZTRACE(setup_output.fOutput, setup_output.dOutput, setup_output.uModel.StableGain);
    //    }
    //});


    return true;
}

bool rpcdemo_service::on_stop()
{
    return true;
}


void rpcdemo_service::handle_timeout(const zce_any& arg)
{
	++timer_count_;

	do
	{
		bool conf_changed = rpcdemo_config_sigt::instance()->is_changed("rpcdemo.xml");
		if (!conf_changed)
			break;
		ZCE_ERROR((ZLOG_ERROR, "rpcdemo.xml change detected\n"));
		rpcdemo_config conf;
		int ret = conf.load("rpcdemo.xml");
		if (ret < 0)
			break;
		if (rpcdemo_config_sigt::instance()->get().info_.loglevel != conf.get().info_.loglevel) {
			rpcdemo_config_sigt::instance()->get().info_.loglevel = conf.get().info_.loglevel;
			zlog_setlevel(rpcdemo_config_sigt::instance()->get().info_.loglevel);
		}
	} while (0);

    if ((timer_count_ % 300) == 0) {
        print_object_stat();
    }

    //if (timer_count_ > 10) {
    //    vmptr_->destroy("main");
    //}
}

int main(int argc, char** argv)
{
    zce_init();

    rpcdemo_service svc;

    g_service = &svc;

    return svc.main(argc, argv);
}
