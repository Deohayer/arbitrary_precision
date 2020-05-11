#ifndef AP_TO_BASIC_H
#define AP_TO_BASIC_H

#include "ap_integer_descriptor.h"
#include <type_traits>
#include "ap_estimation.h"
#include <algorithm>

namespace ap::ap_hidden
{
	template <typename T>
	typename std::enable_if_t<(sizeof(T) <= ap_digit_traits::byte_size), T> to_basic_convertion(const ap_integer_descriptor & seq)
	{
		T result{ 0 };
		for (ap_size_t i = 1; i > 0;)
		{
			--i;
			result |= seq.digits[i];
		}
		return result;
	}

	template <typename T>
	typename std::enable_if_t<(sizeof(T) > ap_digit_traits::byte_size), T> to_basic_convertion(const ap_integer_descriptor & seq)
	{
		ap_size_t count = std::min(seq.size, ap_hidden::size_basic<T>());
		T result{ 0 };
		for (ap_size_t i = count; i > 0;)
		{
			--i;
			result <<= ap_digit_traits::bit_size;
			result |= seq.digits[i];
		}
		return result;
	}

	template <typename T>
	typename std::enable_if_t<std::is_integral_v<T>, T>
	to_basic_int(const ap_integer_descriptor & seq)
	{
		T result = ap_hidden::to_basic_convertion<T>(seq);
		if (seq.sign)
		{
			result = T{0} - result;
		}
		return result;
	}
}

#endif