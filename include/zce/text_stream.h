#pragma once

#include <zce/zce_config.h>
#include <zce/zce_object.h>
#include <map>
#include <functional>

/************************************************************************/
/* Comparator for case-insensitive comparison in STL assos. containers  */
/************************************************************************/

static auto ci_less = [](const std::string & s1, const std::string & s2) -> bool {
        return std::lexicographical_compare
        (s1.begin(), s1.end(),   // source range
            s2.begin(), s2.end(),   // dest range
            [](const unsigned char& c1, const unsigned char& c2) -> bool {
                return tolower(c1) < tolower(c2);
            });  // comparison
};

struct ZCE_API TEXT_HEADER : virtual public zce_object
{
    static const std::string emptystr;

    static const std::string content_length;

    static const std::string transfer_encoding;

    static const std::map<std::string, std::string> special_case;

    enum TEXT_HEADER_E {
        TEXT_HEADER_UNKNOW,
        TEXT_HEADER_REQUEST,
        TEXT_HEADER_RESPONSE
    };

    TEXT_HEADER(TEXT_HEADER_E header_type = TEXT_HEADER_UNKNOW);

    const std::string& exparam(const std::string& parameter)const;

    void exparam(const std::string& parameter, const std::string& value);

	std::map<std::string, std::string>& exparam() { return exparam_; }

    const std::map<std::string, std::string>& const_exparam() const { return exparam_; }

    unsigned header_length() const { return header_length_; }

    void header_length(int len) { header_length_ = len; }

    unsigned body_length() const { return body_length_; }

    void body_length(int length);

    bool has_body_length() const { return has_body_length_; }

    TEXT_HEADER_E header_type_;

protected:

    std::map<std::string, std::string> exparam_;

    bool has_body_length_;

    unsigned header_length_;

    unsigned body_length_;
};
