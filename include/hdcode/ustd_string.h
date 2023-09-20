#pragma once

#include <string>

namespace ustd
{
	// ת���ַ���Ϊ��д
	template<typename T>
	std::basic_string<T> ToUpper(const std::basic_string<T>& source)
	{
		if (source.empty())
		{
			return std::basic_string<T>();
		}

		const std::ctype<T>& facet = std::use_facet<std::ctype<T> >(std::locale::classic());
		std::vector<T> result(source.size());
		memcpy(&result[0], source.data(), sizeof(T) * source.size());
		facet.toupper(&result[0], &result[0] + result.size());

		return std::basic_string<T>(&result[0], result.size());
	}

	// ת���ַ���ΪСд
	template<typename T>
	std::basic_string<T> ToLower(const std::basic_string<T>& source)
	{
		if (source.empty())
		{
			return std::basic_string<T>();
		}

		const std::ctype<T>& facet = std::use_facet<std::ctype<T> >(std::locale::classic());
		std::vector<T> result(source.size());
		memcpy(&result[0], source.data(), sizeof(T) * source.size());
		facet.tolower(&result[0], &result[0] + result.size());

		return std::basic_string<T>(&result[0], result.size());
	}

	// ���Դ�Сд�Ƚ��ַ���
	// ����size, ��Ҫ�Ƚϵĳ���
	// ����ֵ<0, string1 < string2
	// ����ֵ>0, string1 > string2
	// ����ֵ==0, string1 == string2
    /*
	template<typename T>
	int CompareStringNoCase(const std::basic_string<T>& string1, const std::basic_string<T>& string2, int size = 0)
	{
		if (size == 0)
		{
			size = Max(string1.size(), string2.size());
		}

		const std::ctype<T>& facet = std::use_facet<std::ctype<T> >(std::locale::classic());
		std::basic_string<T>::const_iterator iter1 = string1.begin();
		std::basic_string<T>::const_iterator iter2 = string2.begin();
		for (int i = 0; i < size; i++, iter1++, iter2++)
		{
			if (iter1 == string1.end() && iter2 != string2.end())
			{
				return (-1);
			}

			if (iter1 != string1.end() && iter2 == string2.end())
			{
				return (1);
			}

			T ch1 = facet.toupper(*iter1);
			T ch2 = facet.toupper(*iter2);
			if (ch1 < ch2)
			{
				return (-1);
			}
			else if (ch1 > ch2)
			{
				return (1);
			}
			else
			{
			}
		}
		
		return 0;
	}
    */
    /*
	template<typename T>
	void Replace(std::basic_string<T>& source, const T* pszToReplace, const T* pszReplace)
	{
		std::basic_string<T>::size_type nIndex = 0;
		
		while ( (nIndex = source.find(pszToReplace, nIndex)) != std::basic_string<T>::npos)
		{
			source.replace(nIndex, _tcslen(pszToReplace), pszReplace);
			nIndex += _tcslen(pszReplace);
		}		
	}
    */

};	// namespace ustd