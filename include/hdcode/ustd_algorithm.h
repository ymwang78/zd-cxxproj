#pragma once

namespace ustd
{
	template<typename T>
	inline const T& Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	inline const T& Max(const T& a, const T& b)
	{
		return a < b ? b : a;
	}

	template<typename T, size_t size>
	void ZeroArray(T (&_array)[size], const T& value)
	{
		for (size_t i = 0; i < size; i++)
		{
			_array[i] = value;
		}
	}
}; // namespace ustd

