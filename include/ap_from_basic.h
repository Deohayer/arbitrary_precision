#ifndef AP_FROM_BASIC_H
#define AP_FROM_BASIC_H

#include "ap_integer_descriptor.h"
#include <type_traits>
#include "ap_estimation.h"
#include "ap_bitwise.h"
#include "ap_trim.h"

namespace ap::ap_hidden
{
	template <typename T>
	void from_basic_convertion(std::enable_if_t<(sizeof(T) <= ap_digit_traits::byte_size), T> value, 
		ap_integer_descriptor & seq)
	{
		seq.digits[0] = value;
		if (value < 0)
		{
			seq.digits[0] = (~seq.digits[0] + 1);
		}
		seq.size = 1;
		trim(seq);
	}

	template <typename T>
	void from_basic_convertion(std::enable_if_t<(sizeof(T) > ap_digit_traits::byte_size), T> value, 
		ap_integer_descriptor & seq)
	{
		ap_complementer from {value < 0};
		seq.size = std::min(seq.size, size_basic<T>());
		ap_size_t i = 0;
		do 
		{
			seq.digits[i] = from.complement(value);
			value >>= ap_digit_traits::bit_size;
			++i;
		} while (i < seq.size && value != 0);
		seq.size = i;
		trim(seq);
	}

	template <typename T>
	void from_basic_int(
		T value, 
		ap_integer_descriptor & seq)
	{
		seq.sign = (value < 0);
		ap_hidden::from_basic_convertion<T>(value, seq);
	}

}

#endif