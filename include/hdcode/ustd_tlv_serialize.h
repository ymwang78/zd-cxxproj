#pragma once

#include <map>
#include "ustd_memory.h"
#include "ustd_lexical_cast.h"

namespace ustd
{

class TLVWriter
{
public:
	// 添加TLV条目
	template<typename T>
	void AppendAttribute(int type, const T& value)
	{
		std::string valueString = ToString(value);
		ustd::WriteT(m_stream, type);
		ustd::WriteT(m_stream, valueString.size());
		ustd::WriteBuffer(m_stream, valueString);
	}

	// 添加多条TLV条目, 容器类必须有operator[]方法
	template<typename Container>
	void AppendAttributes(int type, const Container& entities)
	{
		for (int i = 0, count = entities.size(); i < count; i++)
		{
			AppendAttribute(type, entities[i]);
		}
	}

public:
	// 获得序列化后的数据流
	const std::string& GetStream() const { return m_stream; }

protected:
	std::string m_stream;
};

class TLVReader
{
public:
	// 解析序列化的数据流
	void Parse(const std::string& stream)
	{
		size_t offset = 0;
		while (true)
		{
			int type;
			unsigned int length;
			std::string value;

			// 类型
			size_t stepOffset = 0;
			stepOffset = ustd::ReadT(stream, offset, type);
			if (stepOffset != sizeof(type))
			{
				break;
			}
			offset += stepOffset;

			// 长度
			stepOffset = ustd::ReadT(stream, offset, length);
			if (stepOffset != sizeof(length))
			{
				break;
			}
			offset += stepOffset;

			// 值
			stepOffset = ustd::ReadBuffer(stream, offset, value, length);
			if (stepOffset != length)
			{
				break;
			}
			offset += stepOffset;

			m_tlvs.insert(std::pair<int, std::string>(type, value));
		}
	}

	// 根据type, 获得TLV条目
	template <typename T>
	bool GetAttribute(int type, T& value)
	{
		std::multimap<int, std::string>::const_iterator iter = m_tlvs.find(type);
		if (iter != m_tlvs.end())
		{
			return ustd::FromString(iter->second, value);
		}
		else
		{
			return false;
		}
	}

	// 根据type, 获得TLV条目列表, 容器类必须有push_back方法
	template <typename Container, typename Value_Type>
	bool GetAttributes(int type, Container& entities)
	{
		std::multimap<int, std::string>::const_iterator lower = m_tlvs.lower_bound(type);
		std::multimap<int, std::string>::const_iterator upper = m_tlvs.upper_bound(type);
		for ( ; lower != upper; lower++)
		{
			Value_Type value;
			if (FromString(lower->second, value))
			{
				entities.push_back(value);
			}
		}

		return true;
	}

public:
	const std::multimap<int, std::string>& GetTLVs() const { return m_tlvs; }

protected:
	std::multimap<int, std::string> m_tlvs;
};

};
