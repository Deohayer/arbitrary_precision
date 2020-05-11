#include "../include/ap_add.h"
#include "../include/ap_sub.h"
#include "../include/ap_trim.h"

namespace ap::ap_hidden
{
	void add(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		if (left.sign != right.sign)
		{
			ap_hidden::sub_preparation(left, right, out);
		}
		else
		{
			ap_hidden::add_preparation(left, right, out);
		}
	}

	void add_preparation(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		if (left.size >= right.size)
		{
			ap_hidden::add_absolute(left, right, out);
		}
		else
		{
			ap_hidden::add_absolute(right, left, out);
		}
		out.sign = left.sign;
		trim(out);
	}

	void add_absolute(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		ap_base_t carry = 0;
		ap_size_t i = 0;
		ap_size_t end_size = std::min(left.size, out.size);
		for (i; i < right.size; ++i)
		{
			carry += left.digits[i];
			carry += right.digits[i];
			out.digits[i] = carry;
			carry >>= ap_digit_traits::bit_size;
		}
		for (i; i < end_size && carry != 0; ++i)
		{
			carry += left.digits[i];
			out.digits[i] = carry;
			carry >>= ap_digit_traits::bit_size;
		}
		for (i; i < end_size; ++i)
		{
			out.digits[i] = left.digits[i];
		}
		if (carry != 0)
		{
			if (out.size > left.size)
			{
				out.digits[i] = 1;
				++i;
			}
		}
		out.size = i;
	}
} // namespace ap
