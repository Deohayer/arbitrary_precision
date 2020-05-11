#include "../include/ap_unary.h"
#include "../include/ap_trim.h"
#include <iostream>
namespace ap::ap_hidden
{
	void increment(
		const ap_integer_descriptor & in,
		ap_integer_descriptor & out)
	{
		if (in.sign != 0)
		{
			ap_hidden::absolute_decrement(in, out);
		}
		else
		{
			ap_hidden::absolute_increment(in, out);
		}
	}

	void decrement(
		const ap_integer_descriptor & in,
		ap_integer_descriptor & out)
	{
		if (in.sign != 0)
		{
			ap_hidden::absolute_increment(in, out);
		}
		else
		{
			ap_hidden::absolute_decrement(in, out);
		}
	}

	void absolute_increment(
		const ap_integer_descriptor & in,
		ap_integer_descriptor & out)
	{
		if (in.size == 0)
		{
			out.digits[0] = 1;
			out.sign = 0;
			out.size = 1;
			return;
		}
		ap_base_t carry = 1;
		ap_size_t carry_shift = ap_digit_traits::bit_size;
		ap_size_t i = 0;
		for (i; i < in.size && carry != 0; ++i)
		{
			carry += in.digits[i];
			out.digits[i] = carry;
			carry >>= carry_shift;
		}
		for (i; i < in.size; ++i)
		{
			out.digits[i] = in.digits[i];
		}
		if (carry != 0)
		{
			if (out.size > i)
			{
				out.digits[i] = 1;
				++i;
			}
			else
			{
				out.size = i;
				out.sign = in.sign;
				ap_hidden::trim(out);
				return;
			}
		}
		out.size = i;
		out.sign = in.sign;
	}

	void absolute_decrement(
		const ap_integer_descriptor & in,
		ap_integer_descriptor & out)
	{
		if (in.size == 0)
		{
			out.digits[0] = 1;
			out.sign = 1;
			out.size = 1;
			return;
		}
		ap_base_t carry = 1;
		ap_size_t carry_shift = ap_base_traits::bit_size - 1;
		ap_size_t i = 0;
		for (i; i < in.size && carry != 0; ++i)
		{
			carry = in.digits[i]  - carry;
			out.digits[i] = carry;
			carry >>= carry_shift;
		}
		for (i; i < in.size; ++i)
		{
			out.digits[i] = in.digits[i];
		}
		out.size = i;
		out.sign = in.sign;
		ap_hidden::trim(out);
	}
}