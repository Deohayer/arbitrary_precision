#include "../include/ap_div.h"
#include "../include/ap_trim.h"
#include <iostream>

namespace ap::ap_hidden
{
	ap_normalized_integer::ap_normalized_integer(ap_size_t capacity, ap_base_t _normalizer) : 
		digits(ap_normalizer_digit_ptr(ap_alloc<ap_digit_t>(capacity), ap_free)),
		size(capacity),
		normalizer(_normalizer)
	{
	}

	void denormalize(
		const ap_normalized_integer & remainder,
		ap_integer_descriptor & out)
	{
		ap_integer_descriptor result;
		result.digits = remainder.digits.get();
		result.size = remainder.size;
		ap_hidden::div_single_digit(result, remainder.normalizer, out);
		trim(out);
	}

	ap_base_t normalize(
		const ap_integer_descriptor & in, 
		ap_base_t digit,
		ap_digit_t * out)
	{
		ap_base_t carry = 0;
		ap_size_t i = 0;
		for (i; i < in.size; ++i)
		{
			carry += digit * in.digits[i];
			out[i] = carry;
			carry >>= ap_digit_traits::bit_size;
		}
		return carry;
	}

	void div(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out_quo,
		ap_integer_descriptor & out_rem)
	{
		ap_normalized_integer remainder = ap_hidden::div_preparation(left, right, out_quo);
		ap_hidden::denormalize(remainder, out_rem);
		out_quo.sign = ((out_quo.size != 0) ? (left.sign + right.sign) : ap_integer_descriptor::SIGN_NONE);
		out_rem.sign = ((out_rem.size != 0) ? left.sign : ap_integer_descriptor::SIGN_NONE);
	}

	void div_quotient(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		ap_hidden::div_preparation(left, right, out);
		out.sign = ((out.size != 0) ? (left.sign + right.sign) : ap_integer_descriptor::SIGN_NONE);
	}

	void div_remainder(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		ap_normalized_integer remainder = ap_hidden::div_preparation(left, right, out);
		ap_hidden::denormalize(remainder, out);
		out.sign = ((out.size != 0) ? left.sign : ap_integer_descriptor::SIGN_NONE);
	}

	ap_normalized_integer div_preparation(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		if (right.size == 0)
		{
			throw 0;
		}
		else if (right.size == 1)
		{
			return ap_hidden::div_single_digit(left, right.digits[0], out);
		}
		else if (left.size < right.size)
		{
			out.size = 0;
			ap_normalized_integer remainder{ right.size, 1 };
			for (size_t i = 0; i < left.size; ++i)
			{
				remainder.digits[i] = left.digits[i];
			}
			remainder.size = left.size;
			return remainder;
		}
		else
		{
			return ap_hidden::div_absolute(left, right, out);
		}
	}

	ap_normalized_integer div_single_digit(
		const ap_integer_descriptor & left,
		ap_digit_t digit,
		ap_integer_descriptor & out)
	{
		ap_normalized_integer remainder{ 1, 1 };
		if (left.size == 0)
		{
			out.size = 0;
			remainder.size = 0;
			return remainder;
		}
		ap_size_t j = left.size - 1;
		ap_base_t carry;
		ap_size_t carry_shift = ap_digit_traits::bit_size;
		if (left.digits[j] < digit)
		{
			out.size = j;
			carry = left.digits[j];
		}
		else
		{
			out.size = left.size;
			carry = left.digits[j] % digit;
			out.digits[j] = left.digits[j] / digit;
		}
		for (j; j > 0;)
		{
			--j;
			carry <<= carry_shift;
			carry |= left.digits[j];
			out.digits[j] = carry / digit;
			carry %= digit;
		}
		remainder.digits[0] = carry;
		return remainder;
	}

	ap_base_t normalize(const ap_integer_descriptor & in, ap_digit_t * out, ap_base_t digit)
	{
		ap_base_t carry = 0;
		ap_size_t i = 0;
		for (i; i < in.size; ++i)
		{
			carry += digit * in.digits[i];
			out[i] = carry;
			carry >>= ap_digit_traits::bit_size;
		}
		return carry;
	}

	ap_normalized_integer div_absolute(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		static constexpr ap_size_t carry_sub_shift = ap_base_traits::bit_size - 1;
		ap_size_t nl_size = left.size + 1;
		ap_size_t nr_size = right.size;
		ap_base_t normalizer = (AP_BASE / 2);
		normalizer = normalizer / right.digits[right.size - 1] + (normalizer % right.digits[right.size - 1] != 0);
		auto n_left = ap_normalized_integer(nl_size, normalizer);
		auto n_right = ap_normalized_integer(nr_size, normalizer);
		// normalize
		n_left.digits[nl_size - 1] = ap_hidden::normalize(left, n_left.digits.get(), normalizer); // forcefully set carry, even if it's 0
		ap_hidden::normalize(right, n_right.digits.get(), normalizer); // carry always zero
		// calculate quotient
		ap_base_t quotient;
		ap_base_t remainder;
		ap_digit_t u2; // "3" in "3210 / 456"
		ap_digit_t u1; // "2" in "3210 / 456"
		ap_digit_t u0; // "1" in "3210 / 456"
		const ap_digit_t v1 = n_right.digits[right.size - 1]; // "4" in "3210 / 456"
		const ap_digit_t v0 = n_right.digits[right.size - 2]; // "5" in "3210 / 456"
		ap_size_t size_diff = left.size - right.size; 
		ap_base_t carry_sub;
		ap_base_t carry_mul;
		ap_size_t i;
		for (size_t j = size_diff + 1; j > 0;)
		{
			--j;
			i = j + right.size;
			u2 = n_left.digits[i];
			u1 = n_left.digits[i - 1];
			u0 = n_left.digits[i - 2];
			quotient = u2;
			quotient <<= ap_digit_traits::bit_size;
			quotient += u1;
			remainder = quotient % v1;
			quotient /= v1;
			if (quotient == AP_BASE || quotient * v0 > (AP_BASE * remainder + u0))
			{
				--quotient;
				remainder += v1;
				if (remainder < AP_BASE)
				{
					if (quotient == AP_BASE || quotient * v0 > (AP_BASE * remainder + u0))
					{
						--quotient;
						remainder += v1;
					}
				}
			}
			i = j;
			carry_sub = 0;
			carry_mul = 0;
			for (ap_size_t k = 0; k < right.size; ++k, ++i)
			{
				carry_sub = n_left.digits[i] - carry_sub;
				carry_mul += n_right.digits[k] * quotient;
				carry_sub -= static_cast<ap_digit_t>(carry_mul);
				n_left.digits[i] = carry_sub;
				carry_sub >>= carry_sub_shift;
				carry_mul >>= (ap_digit_traits::bit_size);
			}
			carry_sub += carry_mul;
			if (carry_sub != 0)
			{
				if (n_left.digits[i] >= carry_sub)
				{
					n_left.digits[i] -= carry_sub;
				}
				else // unlikely
				{
					--quotient;
					i = j;
					carry_mul = 0;
					for (size_t k = 0; k < right.size; ++k, ++i)
					{
						carry_mul += n_left.digits[i];
						carry_mul += n_right.digits[k];
						n_left.digits[i] = carry_mul;
						carry_mul >>= ap_digit_traits::bit_size;
					}
					n_left.digits[i] += carry_mul;
					n_left.digits[i] -= carry_sub;
				}
			}
			out.digits[j] = quotient;
		}
		out.size = size_diff;
		if (out.digits[out.size] != 0)
		{
			++out.size;
		}
		n_left.size = i;
		// out.digits is denormalized quotient. n_left.digits is normalized remainder
		return n_left;
	}
}