#pragma once

#include <assert.h>
#include <string>

#include "ustd_format_string.h"

namespace ustd
{

template<typename T>
std::string ToString(const T& value);

template<typename T>
bool FromString(const std::string& source, T& value);

// std::string转换
///////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string ToString(const std::string& value)
{
	return value;
}

template<>
inline bool FromString(const std::string& source, std::string& value)
{
	value = source;
	return true;
}

// int转换
///////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string ToString(const int& value)
{
	return ustd::FormatString<char>("%d", value);
}

template<>
inline bool FromString(const std::string& source, int& value)
{
#if defined( __GNUC__ )
        return (1 == sscanf(source.c_str(), "%d", &value));
#elif defined( _WIN32 ) || defined( _WIN64 )
		return (1 == sscanf_s(source.c_str(), "%d", &value));
#else
        return false;
#endif
}

// unsigned int转换
///////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string ToString(const unsigned int& value)
{
	return ustd::FormatString<char>("%u", value);
}

template<>
inline bool FromString(const std::string& source, unsigned int& value)
{
#if defined( __GNUC__ )
        return (1 == sscanf(source.c_str(), "%u", &value));
#elif defined( _WIN32 ) || defined( _WIN64 )
		return (1 == sscanf_s(source.c_str(), "%u", &value));
#else
        return false;
#endif
}

// long转换
///////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string ToString(const long& value)
{
	return ustd::FormatString<char>("%d", value);
}

template<>
inline bool FromString(const std::string& source, long& value)
{
#if defined( __GNUC__ )
        return (1 == sscanf(source.c_str(), "%d", &value));
#elif defined( _WIN32 ) || defined( _WIN64 )
		return (1 == sscanf_s(source.c_str(), "%d", &value));
#else
        return false;
#endif
}

// unsigned long转换
///////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string ToString(const unsigned long& value)
{
	return ustd::FormatString<char>("%u", value);
}

template<>
inline bool FromString(const std::string& source, unsigned long& value)
{
#if defined( __GNUC__ )
        return (1 == sscanf(source.c_str(), "%u", &value));
#elif defined( _WIN32 ) || defined( _WIN64 )
		return (1 == sscanf_s(source.c_str(), "%u", &value));
#else
        return false;
#endif
}

// short转换
///////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string ToString(const short& value)
{
	return ustd::FormatString<char>("%d", value);
}

template<>
inline bool FromString(const std::string& source, short& value)
{
	int tempValue;
	if (FromString(source, tempValue))
	{
		value = (0xFFFF & tempValue);
		return true;
	}
	else
	{
		return false;
	}
}

// unsigned short转换
///////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string ToString(const unsigned short& value)
{
	return ustd::FormatString<char>("%d", value);
}

template<>
inline bool FromString(const std::string& source, unsigned short& value)
{
	int tempValue;
	if (FromString(source, tempValue))
	{
		value = (0xFFFF & tempValue);
		return true;
	}
	else
	{
		return false;
	}
}

// bool转换
///////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string ToString(const bool& value)
{
	return (value ? "true" : "false");
}

template<>
inline bool FromString(const std::string& source, bool& value)
{
	value = (source[0] == 't' || source[0] == 'T');
	return true;
}

// long long
///////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string ToString(const long long& value)
{
	return ustd::FormatString("%lld", value);
}

template<>
inline bool FromString(const std::string& source, long long& value)
{
#if defined( __GNUC__ )
        return (1 == sscanf(source.c_str(), "%lld", &value));
#elif defined( _WIN32 ) || defined( _WIN64 )
		return (1 == sscanf_s(source.c_str(), "%lld", &value));
#else
        return false;
#endif
}

// unsigned long long转换
///////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string ToString(const unsigned long long& value)
{
	return ustd::FormatString<char>("%llu", value);
}

template<>
inline bool FromString(const std::string& source, unsigned long long& value)
{
#if defined( __GNUC__ )
        return (1 == sscanf(source.c_str(), "%llu", &value));
#elif defined( _WIN32 ) || defined( _WIN64 )
		return (1 == sscanf_s(source.c_str(), "%llu", &value));
#else
        return false;
#endif
}

// double
///////////////////////////////////////////////////////////////////////////////////////////////////
template<>
inline std::string ToString(const double& value)
{
	return ustd::FormatString("%lf", value);
}

template<>
inline bool FromString(const std::string& source, double& value)
{
#if defined( __GNUC__ )
        return (1 == sscanf(source.c_str(), "%lf", &value));
#elif defined( _WIN32 ) || defined( _WIN64 )
		return (1 == sscanf_s(source.c_str(), "%lf", &value));
#else
        return false;
#endif
}
};	// namespace ustd;