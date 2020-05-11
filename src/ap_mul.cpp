#include "../include/ap_integer_descriptor.h"
#include "../include/ap_mul.h"
#include "../include/ap_trim.h"
#include "../include/ap_estimation.h"

namespace ap::ap_hidden
{
	void mul(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		if (left.size >= right.size)
		{
			ap_hidden::mul_preparation(left, right, out);
		}
		else
		{
			ap_hidden::mul_preparation(right, left, out);
		}
	}

	void mul_preparation(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		if (right.size == 0)
		{
			out.size = 0;
			out.sign = ap_integer_descriptor::SIGN_NONE;
			return;
		}
		out.sign = left.sign + right.sign;
		mul_absolute(left, right, out);
		ap_hidden::trim(out);
	}

	void mul_absolute(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		ap_digit_t * out_digits_origin = out.digits;
		ap_size_t out_capacity = out.size;
		out.size = 0;
		for (ap_size_t i = 0; i < right.size; ++i)
		{
			mul_step(left, right.digits[i], out, out_capacity - i);
			++out.digits;
			--out.size;
		}
		out.size += right.size;
		out.digits = out_digits_origin;
	}

	void mul_step(
		const ap_integer_descriptor & left,
		ap_base_t right_digit,
		ap_integer_descriptor & out,
		ap_size_t out_capacity)
	{
		ap_size_t stop_index = std::min(left.size, out_capacity);
		ap_base_t carry = 0;
		ap_size_t i = 0;
		for (i = 0; i < out.size; ++i)
		{
			carry += out.digits[i];
			carry += right_digit * left.digits[i];
			out.digits[i] = carry;
			carry >>= ap_digit_traits::bit_size;
		}
		for (i; i < stop_index; ++i)
		{
			carry += right_digit * left.digits[i];
			out.digits[i] = carry;
			carry >>= ap_digit_traits::bit_size;
		}
		if (out_capacity > stop_index)
		{
			out.digits[i] = carry;
			++i;
		}
		out.size = i;
	}
}