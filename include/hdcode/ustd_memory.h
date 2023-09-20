#pragma once

#include <string>
#include "ustd_algorithm.h"

namespace ustd
{
	template<typename T>
	inline size_t ReadT(void* data, size_t length, T& value)
	{
		if (sizeof(T) <= length)
		{
			memcpy(&value, data, sizeof(T));
			return sizeof(T);
		}
		else
		{
			return 0;
		}
	}

	template<typename T>
	inline size_t WriteT(void* buffer, size_t size, T value)
	{
		if (sizeof(T) <= size)
		{
			memcpy(buffer, &value, sizeof(T));
			return sizeof(T);
		}
		else
		{
			return 0;
		}
	}

	inline size_t ReadBuffer(void* data, size_t dataLength, void* value, size_t valueSize)
	{
		size_t copySize = Min(dataLength, valueSize);
		memcpy(value, data, copySize);
		return copySize;
	}

	inline size_t WriteBuffer(void* buffer, size_t bufferSize, const void* value, size_t valueLength)
	{
		size_t copySize = Min(bufferSize, valueLength);
		memcpy(buffer, value, copySize);
		return copySize;
	}

	template<typename T>
	inline size_t ReadT(const std::string& buffer, size_t offset, T& value)
	{
        size_t buffer_size;
        buffer_size = sizeof(T);
		size_t copySize = buffer.size() - offset;
		if (sizeof(T) <= copySize)
		{
            return buffer.copy((char*)&value, sizeof(T), offset);
		}
		else
		{
			return 0;
		}
	}

	template<typename T>
	inline size_t WriteT(std::string& buffer, const T& value)
	{
		buffer.append((char*)&value, sizeof(T));
		return sizeof(T);
	}

	inline size_t ReadBuffer(const std::string& buffer, size_t offset, void* value, size_t valueSize)
	{
		size_t copySize = buffer.size() - offset;
		copySize = Min(valueSize, copySize);
        return buffer.copy((char*)value, copySize, offset);
	}

	inline size_t ReadBuffer(const std::string& buffer, size_t offset, std::string& value, size_t readSize)
	{
		size_t copySize = buffer.size() - offset;
		copySize = Min(readSize, copySize);
		value.assign(buffer, offset, copySize);
		return copySize;
	}

	inline size_t WriteBuffer(std::string& buffer, const char* value, size_t length)
	{
		buffer.append(value, length);
		return length;
	}

	inline size_t WriteBuffer(std::string& buffer, const std::string& value)
	{
		buffer.append(value);
		return value.size();
	}
}; // namespace ustd

