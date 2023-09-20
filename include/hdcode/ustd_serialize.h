#pragma once

#include <string>
#include <stddef.h>

namespace ustd
{

class SerailizeArchive
{
public:
	template<typename T>
	bool ReadT(T& value)
	{
		size_t length;
		if (sizeof(size_t) == ustd::ReadT(m_buffer, m_offset, length) && 
			sizeof(T) == length)
		{
			m_offset += sizeof(size_t);
			if (sizeof(T) == ustd::ReadT(m_buffer, m_offset, value))
			{
				m_offset += sizeof(T);
				return true;
			}
		}

		return false;
	}

	bool Read(std::string& value)
	{
		size_t length;
		if (sizeof(size_t) == ustd::ReadT(m_buffer, m_offset, length))
		{
			if (length > 0)
			{
				m_offset += sizeof(size_t);
				if (length == ReadBuffer(m_buffer, m_offset, value, length))
				{
					m_offset += length;
					return true;
				}
			}
			else
			{
				 value.clear();
				 return true;
			}
		}

		return false;
	}

	template<typename T>
	void WriteT(const T& value)
	{
		size_t length = sizeof(T);
		ustd::WriteT(m_buffer, length);
		ustd::WriteT(m_buffer, value);
	}

	void Write(const std::string& value)
	{
		ustd::WriteT(m_buffer, value.size());
		WriteBuffer(m_buffer, value);
	}

	const std::string& GetData() const { return m_buffer; }
	void SetData(const std::string& data) { m_buffer = data, m_offset = 0; }

private:
	std::string m_buffer;
	size_t m_offset; // ¶ÁµÄÎ»ÖÃ
};

};	// namespace ustd
