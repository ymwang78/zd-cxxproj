#pragma once

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>
#include <vector>
#include <string>

namespace ustd
{
	// 格式化字符串
	///////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	std::basic_string<T> FormatString(const T* format, ...);

	template<>
	inline std::string FormatString<char>(const char* format, ...)
	{
#if defined( __GNUC__ )
        std::string result;
        const char *ptr = NULL, *pre = format, *val;
        std::string fmt;
        char  ch;
        char  temp[128], prefix;
        bool leftadj, finish, valid;
        int  width, len;
        int  longtype;

        va_list args;
        va_start(args, format);
        while((ptr = strchr(pre, '%')) != NULL) {
            if( ptr - pre > 0) {
                result += std::string(pre, ptr);
            }
            pre = ptr;
            ptr++;

            finish = false;
            valid = true;
            width = -1;
            leftadj = false;
            prefix = ' ';
            fmt.clear();
            fmt.push_back('%');
            longtype = 0;
            while(finish == false && (ch = *ptr++) != 0) {
                switch(ch) {
                case ' ': case '#': case '+': 
                case '.': case 'h':
                    fmt.push_back(ch);
                    break;
                case 'L':
                    longtype = 1;
                    fmt.push_back(ch);
                    break;
                case 'l':
                    longtype++;
                    fmt.push_back(ch);
                    break;
                case '-':
                    leftadj = true;
                    fmt.push_back(ch);
                    break;
                case '0':
                    prefix = '0';
                    fmt.push_back(ch);
                    break;
                case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                    fmt.push_back(ch);
                    width = ch - '0';
                    while (isdigit(ch = *ptr)) {
                        width = 10 * width + ch - '0';
                        fmt.push_back(ch);
                        ptr++;
                    }
                    break;
                case '*':
                    if((width = va_arg(args, int)) < 0)
                        width = -width;
                    sprintf(temp, "%d", width);
                    fmt += temp;
                    break;
                case 'c':
                    fmt.push_back(ch);
                    snprintf(temp, sizeof(temp), fmt.c_str(), ch);
                    result += temp;
                    finish = true;
                    break;
                case 'D': case 'd': case 'i':
                case 'U': case 'u': case 'O': case 'o':
                case 'X': case 'x':
                    fmt.push_back(ch);
                    if(longtype == 0)
                        snprintf(temp, sizeof(temp), fmt.c_str(), va_arg(args, int));
                    else if(longtype == 1)
                        snprintf(temp, sizeof(temp), fmt.c_str(), va_arg(args, long));
                    else
                        snprintf(temp, sizeof(temp), fmt.c_str(), va_arg(args, long long));
                    result += temp;
                    finish = true;
                    break;
                case 'e': case 'E': case 'f': case 'F':
                case 'g': case 'G':
                    fmt.push_back(ch);
                    snprintf(temp, sizeof(temp), fmt.c_str(), va_arg(args, double));
                    result += temp;
                    finish = true;
                    break;
                case 'p':
                    fmt.push_back(ch);
                    snprintf(temp, sizeof(temp), fmt.c_str(), va_arg(args, void *));
                    result += temp;
                    finish = true;
                    break;
                case 's':
                    finish = true;
                    val = va_arg(args, char *);
                    if(val == NULL) {
                        result += "(null)";
                        break;
                    }
                    len = strlen(val);
                    if(width > len ) {
                        if(leftadj) {
                            result += val;
                            result += std::string(width - len, ' ');
                        }
                        else {
                            result += std::string(width - len, prefix);
                            result += val;
                        }
                    }
                    else
                        result += val;
                    finish = true;
                    break;
                default:
                    valid = false;
                    finish = true;
                    break;
                }
            }
            if(valid == false)
                result += std::string(pre, ptr);
            pre = ptr;
        }
        result += pre;
        va_end(args);
        return result;
#elif defined( _WIN32 ) || defined( _WIN64 )
		va_list vars;
		va_start(vars, format);
		int size = _vscprintf(format, vars);
		std::vector<char> result(size + 1);
		vsprintf_s(&result[0], result.size(), format, vars);
		va_end(vars);

		return std::string(&result[0], size);
#else
#      //todo
#endif
	}

    /*
	template<>
	inline std::wstring FormatString<wchar_t>(const wchar_t* format, ...)
	{
		va_list vars;
		va_start(vars, format);
		int size = _vscwprintf(format, vars);
		std::vector<wchar_t> result(size + 1);
		vswprintf_s(&result[0], result.size(), format, vars);
		va_end(vars);

		return std::wstring(&result[0], size);
	}
    */
	// 转化字符为十六进制可打印字符串
	///////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T> 
	void ToHexChar(unsigned char ch, T hex[3]);

	template<> 
	inline void ToHexChar<char>(unsigned char ch, char hex[3])
	{
#if defined( __GNUC__ )
        snprintf(hex, 3, "%02x", ch);
#elif defined( _WIN32 ) || defined( _WIN64 )
		sprintf_s(hex, 3, "%02x", ch);
#else
#      //todo
#endif
	}

    /*
	template<> 
	inline void ToHexChar<wchar_t>(unsigned char ch, wchar_t hex[3])
	{
		swprintf_s(hex, 3, L"%02x", ch);
	}
    */

	// 转化为字符串为十六进制可打印字符串
	///////////////////////////////////////////////////////////////////////////////////////////////
	template<typename T>
	std::basic_string<T> ToHexString(void* data, int length)
	{
		T hex[3];
		std::basic_string<T> output;
		for (int i = 0, pos = 0; i < length; i++)
		{
			ToHexChar(*((unsigned char*)data + i), hex);
			output += hex;
		}

		return output;
	}

	// 转化为字符串为十六进制可打印字符串
	///////////////////////////////////////////////////////////////////////////////////////////////
	// 参数distance, 表示换行的位置
	// 参数wrap, 表示换行符
	// 参数prefix，表示前置字符串
	// 参数suffix，表示后置字符串
	// 例：格式化MAC地址, ToHexString<char>(mac, 6, 1, "-", "", "");
	template<typename T>
	std::basic_string<T> ToHexString(
		void* data, 
		int length, 
		int distance, 
		const std::basic_string<T>& wrap, 
		const std::basic_string<T>& prefix, 
		const std::basic_string<T>& suffix)
	{
		if (distance == 0 || wrap.empty())
		{
			ToHexString<T>(data, length);
		}

		std::basic_string<T> output;
		if (!prefix.empty())
		{
			output += prefix;
		}

		T hex[3];
		for (int i = 0; i < length; i++)
		{
			if ((i + 1) == length || ((i + 1) % distance) != 0)
			{
				ToHexChar(*((unsigned char*)data + i), hex);
				output += hex;
			}
			else if (((i + 1) % distance) == 0)
			{
				ToHexChar(*((unsigned char*)data + i), hex);
				output += hex;
				output += wrap;
			}
			else
			{
				assert(false);
			}
		}

		if (!suffix.empty())
		{
			output += suffix;
		}

		return output;
	}

	// 转换IP地址到字符串
	///////////////////////////////////////////////////////////////////////////////////////////////
    /*
	template<typename T>
	std::basic_string<T> IpToString(unsigned long ipv4)
	{
		return FormatString(ConvertToString<T>("%d.%d.%d.%d").c_str(), 
			(ipv4 >> 24) & 0xFF, 
			(ipv4 >> 16) & 0xFF, 
			(ipv4 >> 8) & 0xFF, 
			ipv4 & 0xFF);
	}
    */

	// 转换地址端口为字符串, 格式为: xxx.xxx.xxx.xxx:port
	///////////////////////////////////////////////////////////////////////////////////////////////
    /*
	template<typename T>
	std::basic_string<T> IpToString(unsigned long ipv4, unsigned short port)
	{
		return FormatString(ConvertToString<T>("%d.%d.%d.%d:%d").c_str(), 
			(ipv4 >> 24) & 0xFF, 
			(ipv4 >> 16) & 0xFF, 
			(ipv4 >> 8) & 0xFF, 
			ipv4 & 0xFF, 
			port);
	}
    */

	// 转换字符串到ip地址
	///////////////////////////////////////////////////////////////////////////////////////////////
	inline unsigned long IpFromString(const char* ipString)
	{
		unsigned int ipchars[4] = { 0 };
#if defined( __GNUC__ )
        sscanf(ipString, "%u.%u.%u.%u", &ipchars[0], &ipchars[1], &ipchars[2], &ipchars[3]);
#elif defined( _WIN32 ) || defined( _WIN64 )
		sscanf_s(ipString, "%u.%u.%u.%u", &ipchars[0], &ipchars[1], &ipchars[2], &ipchars[3]);
#else
#      //todo
#endif
		return (ipchars[0] << 24 | ipchars[1] << 16 | ipchars[2] << 8 | ipchars[3]);
	}

}; // namespace ustd

