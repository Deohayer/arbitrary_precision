#ifndef AP_ESTIMATION_H
#define AP_ESTIMATION_H

#include "ap_integer_descriptor.h"

namespace ap::ap_hidden
{
	ap_size_t size_add(const ap_integer_descriptor & left, const ap_integer_descriptor & right);

	ap_size_t size_sub(const ap_integer_descriptor & left, const ap_integer_descriptor & right);

	ap_size_t size_mul(const ap_integer_descriptor & left, const ap_integer_descriptor & right);

	ap_size_t size_quotient(const ap_integer_descriptor & left, const ap_integer_descriptor & right);

	ap_size_t size_remainder(const ap_integer_descriptor & left, const ap_integer_descriptor & right);

	ap_size_t size_increment(const ap_integer_descriptor & in);

	ap_size_t size_decrement(const ap_integer_descriptor & in);

	ap_size_t size_bitwise(const ap_integer_descriptor & left, const ap_integer_descriptor & right);

	ap_size_t size_bitnot(const ap_integer_descriptor & in);

	ap_size_t size_rshift(const ap_integer_descriptor & left, const ap_integer_descriptor & right);

	ap_size_t size_lshift(const ap_integer_descriptor & left, const ap_integer_descriptor & right);

	ap_size_t size_copy(const ap_integer_descriptor & in);

	template <typename T>
	constexpr ap_size_t size_basic()
	{
		return (ap_basic_traits<T>::byte_size > ap_digit_traits::byte_size) ? 
            ap_basic_traits<T>::byte_size / ap_digit_traits::byte_size : 1;
	}
}

#endif