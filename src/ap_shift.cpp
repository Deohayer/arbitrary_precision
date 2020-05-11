#include "../include/ap_bitwise.h"
#include "../include/ap_trim.h"
#include "../include/ap_to_basic.h"
#include "../include/ap_bitwise.h"
namespace ap::ap_hidden
{
	void bitwise_rshift(
		const ap_integer_descriptor & left,
		ap_size_t count,
		ap_integer_descriptor & out)
	{
		ap_size_t digit_shift = count / ap_digit_traits::bit_size;
		ap_size_t bit_shift = count % ap_digit_traits::bit_size;
		ap_size_t half_shift = ap_digit_traits::bit_size;
		ap_size_t buff_shift = half_shift - bit_shift;
		ap_size_t out_capacity = out.size;
		if (left.size == 0 || digit_shift >= left.size)
		{
			if (left.sign)
			{
				out.size = 1;
				out.digits[0] = 1;
				out.sign = 1;
				return;
			}
			else
			{
				out.size = 0;
				out.sign = 0;
				return;
			}
		}
		out.sign = left.sign;
		ap_complementer to{left.sign == 1};
		ap_complementer from{out.sign == 1};
		out.size = std::min(out_capacity, left.size - digit_shift);
		for (ap_size_t i = 0; i < digit_shift; ++i)
		{
			to.complement(left.digits[i]);
		}
		auto digits = left.digits + digit_shift;
		ap_base_t buff;
		ap_base_t queue = (to.complement(digits[0]) >> bit_shift);
		for (ap_size_t i = 1; i < out.size; ++i)
		{
			buff = to.complement(digits[i]);
			buff <<= buff_shift;
			queue |= buff;
			out.digits[i - 1] = from.complement(queue);
			queue >>= ap_digit_traits::bit_size;
		}
		if (queue != 0)
		{
			ap_base_t fill = (out.sign ? ((AP_BASE - 1) << (ap_digit_traits::bit_size - bit_shift)) : 0);
			out.digits[out.size - 1] = from.complement(queue | fill);
		}
		else
		{
			--out.size;
		}
		trim(out);
	}

	void bitwise_lshift(
		const ap_integer_descriptor & left,
		ap_size_t count,
		ap_integer_descriptor & out)
	{
		ap_size_t digit_shift = count / ap_digit_traits::bit_size;
		ap_size_t bit_shift = count % ap_digit_traits::bit_size;
		ap_size_t half_shift = ap_digit_traits::bit_size;
		ap_size_t out_capacity = out.size;
		if (left.size == 0 || digit_shift >= out_capacity)
		{
			out.size = 0;
			out.sign = 0;
			return;
		}
		out.size = std::min(left.size + digit_shift, out_capacity);
		out.sign = left.sign;
		ap_size_t digit_count = out.size - digit_shift;
		auto digits = out.digits + digit_shift;
		ap_base_t buff;
		ap_base_t queue = left.digits[0];
		queue <<= bit_shift;
		digits[0] = queue;
		queue >>= half_shift;
		for (ap_size_t i = 1; i < digit_count; ++i)
		{
			buff = left.digits[i];
			buff <<= bit_shift;
			queue |= buff;
			digits[i] = queue;
			queue >>= half_shift;
		}
		if (queue != 0)
		{
			if (out.size < out_capacity)
			{
				out.digits[out.size] = queue;
				++out.size;
			}
		}
		for (ap_size_t i = out.size; i > digit_shift;)
		{
			--i;
			if (out.digits[i] != 0)
			{
				// fill first part with zeros
				for (ap_size_t i = 0; i < digit_shift; ++i)
				{
					out.digits[i] = 0;
				}
				return;
			}
		}
		out.size = 0;
		out.sign = 0;
		trim(out);
	}
} // namespace ap
