#pragma once
#ifndef __ZCE_STRING__
#define __ZCE_STRING__

#include <string>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string.h>

#if __cplusplus < 201700L
#   include <boost/version.hpp>
#   if BOOST_VERSION < 106400
#       include <zce/string_view.hpp>
#   else
#       include <boost/utility/string_view.hpp>
#   endif
#else
#   include <string_view>
#endif

namespace zce
{
#if __cplusplus < 201700L
#   if BOOST_VERSION < 106400
#   else
    using string_view = boost::string_view;
#   endif
#else
    using string_view = std::string_view;
#endif

    struct none_t {
    };

    inline void replace(std::string &strBase, const std::string& strSrc, const std::string& strDes, bool bReplaceAll)
    {
        std::string::size_type pos = 0;
        std::string::size_type srcLen = strSrc.size();
        std::string::size_type desLen = strDes.size();
        pos = strBase.find(strSrc, pos);
        while (pos != std::string::npos) {
            strBase.replace(pos, srcLen, strDes);
            pos = strBase.find(strSrc, (pos + desLen));

            if (!bReplaceAll) {
                break;
            }
        }
    }

    template<typename T>
    inline std::string to_string(const T& value)
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    template<>
    inline std::string to_string<const std::string&>(const std::string& str)
    {
        return str;
    }

    template<typename T>
    inline std::string to_string(const std::vector<T>& arr)
    {
        std::ostringstream stream;
        for (unsigned i = 0; i < arr.size(); ++i) {
            stream << arr[i] << ",";
        }
        return stream.str();
    }

    template<>
    inline std::string to_string<bool>(const bool& _value)
    {
        return _value ? "true" : "false";
    }

    template<typename T1, typename T2, typename... Args>
    inline std::string to_string(T1&& t1, T2&& t2, Args&&... args)
    {
        std::ostringstream oss;
        oss << to_string(t1) << to_string(std::forward<T2>(t2), std::forward<Args>(args)...);
        return oss.str();
    }

    inline void trim(std::string& _str, bool _left = true, bool _right = true, const std::string& whiestr = " \t\n\r")
    {
        if (_right) _str.erase(_str.find_last_not_of(whiestr) + 1);
        if (_left) _str.erase(0, _str.find_first_not_of(whiestr));
    }

    inline std::string to_lower(const std::string& str) {
        std::string t = str;
        std::transform(t.begin(), t.end(), t.begin(), ::tolower);
        return t;
    }

    inline std::string to_upper(const std::string& str) {
        std::string t = str;
        std::transform(t.begin(), t.end(), t.begin(), ::toupper);
        return t;
    }

    inline std::string to_formula(const std::string& str) {
        struct formular {
            bool is_first_;
            formular() :is_first_(true) {

            }
            int operator()(int v){
                if (is_first_) {
                    is_first_ = false;
                    return ::toupper(v);
                }
                else {
                    if (v == '-' || v == ' ' || v == '\r' || v == '\n') {
                        is_first_ = true;
                    }
                    return ::tolower(v);
                }
            }
        };
        std::string t = str;
        std::transform(t.begin(), t.end(), t.begin(), formular());
        return t;
    }

    template<typename T>
    inline T parse_value(const std::string& _value)
    {
        std::istringstream stream(_value);
        T result;
        stream >> result;
        if (stream.fail()) {
            return T();
        }
        else {
            int item = stream.get();
            while (item != -1) {
                if (item != ' ' && item != '\t')
                    return T();
                item = stream.get();
            }
            return result;
        }
    }

    inline bool parse_bool(const std::string& _value)
    {
        if (_value == "True" || _value == "true" || _value == "TRUE" || _value == "1")
            return true;
        return false;
    }

    inline char parse_int8 (const std::string& _value)
    {
        return (char)parse_value<char>(_value);
    }

    inline unsigned char parse_uint8(const std::string& _value)
    {
        return (unsigned char)parse_value<unsigned char>(_value);
    }

    inline short parse_int16(const std::string& _value)
    {
        return parse_value<short>(_value);
    }

    inline unsigned short parse_uint16(const std::string& _value)
    {
        return parse_value<unsigned short>(_value);
    }

    inline int parse_int32(const std::string& _value)
    {
        return parse_value<int>(_value);
    }

    inline unsigned int parse_uint32(const std::string& _value)
    {
        return parse_value<unsigned int>(_value);
    }

    inline size_t parse_sizet(const std::string& _value)
    {
        return parse_value<size_t>(_value);
    }

    inline float parse_float(const std::string& _value)
    {
        return parse_value<float>(_value);
    }

    inline double parse_double(const std::string& _value)
    {
        return parse_value<double>(_value);
    }

    template<typename T1, typename T2>
    inline T1 parse_bind2(const std::string& _value)
    {
        T2 p1, p2;
        std::istringstream stream(_value);
        stream >> p1 >> p2;
        if (stream.fail()) {
            return T1();
        }
        else {
            int item = stream.get();
            while (item != -1) {
                if (item != ' ' && item != '\t')
                    return T1();
                item = stream.get();
            }
            return T1(p1, p2);
        }
    }

    template<typename T1, typename T2>
    inline T1 parse_bind3(const std::string& _value)
    {
        T2 p1, p2, p3;
        std::istringstream stream(_value);
        stream >> p1 >> p2 >> p3;
        if (stream.fail()) {
            return T1();
        }
        else {
            int item = stream.get();
            while (item != -1) {
                if (item != ' ' && item != '\t')
                    return T1();
                item = stream.get();
            }
            return T1(p1, p2, p3);
        }
    }

    template<typename T1, typename T2>
    inline T1 parse_bind4(const std::string& _value)
    {
        T2 p1, p2, p3, p4;
        std::istringstream stream(_value);
        stream >> p1 >> p2 >> p3 >> p4;
        if (stream.fail()) {
            return T1();
        }
        else {
            int item = stream.get();
            while (item != -1) {
                if (item != ' ' && item != '\t')
                    return T1();
                item = stream.get();
            }
            return T1(p1, p2, p3, p4);
        }
    }

    namespace templates
    {
        class StringSplitter {
        public:
            /** @param big the string to be split
            @param splitter the delimiter
            */
            StringSplitter(const char * big, const char * splitter)
                : _big(big), _splitter(splitter) {
            }

            /** @return true if more to be taken via next() */
            bool more() {
                return _big[0];
            }

            /** get next split string fragment */
            std::string next() {
                const char* begin = _big;
                const char* end = begin;
                bool match = false;
                while (*end) {
                    const char* sp = _splitter;
                    while (*sp) {
                        if (*sp == *end) {
                            match = true;
                            break;
                        }
                        sp++;
                    }
                    if (match) {
                        _big = end + 1;
                        if (begin == end) {                            
                            return std::string();
                        }
                        else {
                            return std::string(begin, end - begin);
                        }
                    }
                    else {
                        ++end;
                    }
                }
                std::string s = _big;
                _big += strlen(_big);
                return s;
            }

            void split(std::vector<std::string>& l) {
                while (more()) {
                    l.push_back(next());
                }
            }

            std::vector<std::string> split() {
                std::vector<std::string> l;
                split(l);
                return l;
            }

            static std::vector<std::string> split(const std::string& big, const std::string& splitter) {
                StringSplitter ss(big.c_str(), splitter.c_str());
                return ss.split();
            }

            static std::string join(std::vector<std::string>& l, const std::string& split) {
                std::stringstream ss;
                for (unsigned i = 0; i<l.size(); i++) {
                    if (i > 0)
                        ss << split;
                    ss << l[i];
                }
                return ss.str();
            }

        private:
            const char * _big;
            const char * _splitter;
        };

        inline void split(std::vector<std::string>& _ret, const char* _source, const char* _delims)
        {
            StringSplitter spliter(_source, _delims);
            spliter.split(_ret);
        }

        inline void split_fullmatch(std::vector<std::string>& _ret, const std::string& _source, const std::string& _delims)
        {
            if (_source == "") {
                return;
            }
            size_t start = 0;
            size_t end = _source.find(_delims);
            while (start != _source.npos)
            {
                if (end != _source.npos)
                    _ret.push_back(_source.substr(start, end - start));
                else
                {
                    _ret.push_back(_source.substr(start));
                    break;
                }
                start = end;
                if (start != _source.npos)
                {
                    start += _delims.size();
                }
                end = _source.find(_delims, start);
            }
        }
    } // namespace templates

    inline std::vector<std::string> split(const char* _source, const char* _delims = "\t\n ")
    {
        std::vector<std::string> result;
        templates::split(result, _source, _delims);
        return result;
    }

    inline std::vector<std::string> split(const std::string& _source, const char* _delims = "\t\n ")
    {
        return split(_source.c_str(), _delims);
    }

    inline std::string standardise_path(const std::string& origin)
    {
        std::string path = origin;

        std::replace(path.begin(), path.end(), '\\', '/');
        return path;
    }

    inline bool start_with(const std::string& _source, const std::string& _value)
    {
        size_t count = _value.size();
        if (_source.size() < count)
            return false;
        for (size_t index = 0; index < count; ++index)
        {
            if (_source[index] != _value[index])
                return false;
        }
        return true;
    }

    inline bool end_with(const std::string& _source, const std::string& _value)
    {
        size_t count = _value.size();
        if (_source.size() < count)
            return false;
        size_t offset = _source.size() - count;
        for (size_t index = 0; index < count; ++index)
        {
            if (_source[index + offset] != _value[index])
                return false;
        }
        return true;
    }

}

#endif //__ZCE_STRING__
