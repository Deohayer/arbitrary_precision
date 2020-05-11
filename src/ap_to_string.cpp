#include "../include/ap_to_string.h"
#include "../include/ap_trim.h"
#include "../include/ap_string_chardigit.h"
#include <memory>
#include <iostream>

namespace ap::ap_hidden
{	
	template <ap_size_t Bit_count, char Base_symbol>
	std::string to_noremainder_base2(const ap_integer_descriptor & in, bool uppercase = true)
	{
		static constexpr ap_size_t base_bits = Bit_count;
		static constexpr ap_digit_t base_mask = static_cast<ap_digit_t>(~(static_cast<ap_digit_t>(~ap_digit_t{0}) << base_bits));
		static constexpr ap_size_t chars_in_digit = ap_digit_traits::bit_size / base_bits;
		if (in.size == 0)
		{
			return { '0', Base_symbol, '0' };
		}
		const char * hex_chars = (uppercase) ? CHARDIGITS_UPPER : CHARDIGITS_UPPER;
		std::string result;
		ap_size_t i = 0;
		result.resize(in.size * chars_in_digit + 3);
		if (in.sign)
		{
			result[i] = '-';
			++i;
		}
		result[i] = '0';
		++i;
		result[i] = Base_symbol;
		++i;
		ap_digit_t first_digit_shift = ap_digit_traits::bit_size - base_bits;
		const ap_size_t last_index = in.size - 1;
		ap_digit_t buff = in.digits[last_index];
		while ((buff >> first_digit_shift) == 0)
		{
			first_digit_shift -= base_bits;
		}
		while (first_digit_shift != 0)
		{
			result[i] = hex_chars[(buff >> first_digit_shift) & base_mask];
			++i;
			first_digit_shift -= base_bits;
		}
		result[i] = hex_chars[buff & base_mask];
		++i;
		i += last_index * chars_in_digit;
		result.resize(i);
		buff = 0;
		for (ap_size_t j = 0; j < last_index; ++j)
		{
			buff = in.digits[j];
			--i;
			result[i] = hex_chars[base_mask & buff];
			for (ap_size_t k = 1; k < chars_in_digit; ++k)
			{
				buff >>= base_bits;
				--i;
				result[i] = hex_chars[base_mask & buff];
			}
		}
		return result;
	}

	std::string to_bin(const ap_integer_descriptor & in)
	{
		return to_noremainder_base2<1, 'b'>(in);
	}

	std::string to_oct(const ap_integer_descriptor & in)
	{
		static constexpr ap_size_t base_bits = 3;
		static constexpr ap_base_t base_mask = 0b111;
		static constexpr ap_size_t step_shifts[base_bits] =
		{
			0,
			ap_digit_traits::bit_size % base_bits,
			base_bits - ap_digit_traits::bit_size % base_bits
		};
		static constexpr ap_size_t chars_per_step[base_bits] =
		{
			ap_digit_traits::bit_size / base_bits,
			ap_digit_traits::bit_size / base_bits + step_shifts[2] % 2,
			ap_digit_traits::bit_size / base_bits + 1,
		};
		if (in.size == 0)
		{
			return "00";
		}
		const ap_size_t complete_triads = (in.size - 1) / base_bits;
		const ap_size_t leftover = in.size - complete_triads * base_bits;
		std::string result((complete_triads + 1) * ap_digit_traits::bit_size + 2, '\0');
		ap_size_t istr = 0; // string index, for writing chars
		ap_size_t idig = 0; // iterate over digits in number
		ap_size_t istp = 0; // step index, 0, 1 or 2, determines step shift and amount of chars to proceed
		ap_base_t buff = 0;
		ap_base_t bit_queue = 0;

		if (in.sign == 1)
		{
			result[istr] = '-';
			++istr;
		}
		result[istr] = '0';
		++istr;
		idig = complete_triads * base_bits;
		for (istp = 0; istp < leftover - 1; ++istp)
		{
			buff = in.digits[idig + istp];
			buff <<= step_shifts[istp];
			bit_queue |= buff;
			for (ap_size_t i = 0; i < chars_per_step[istp]; ++i)
			{
				result[istr] = (bit_queue & base_mask) + '0';
				++istr;
				bit_queue >>= base_bits;
			}
		}
		buff = in.digits[idig + istp];
		buff <<= step_shifts[istp];
		bit_queue |= buff;
		do
		{
			result[istr] = (bit_queue & base_mask) + '0';
			bit_queue >>= base_bits;
			++istr;
		} while (bit_queue > 0);
		char * oct_digits = result.data() + 1 + in.sign;
		buff = static_cast<ap_size_t>(istr - 1 - in.sign) / 2;
		for (ap_size_t i = 0; i < buff; ++i)
		{
			std::swap(oct_digits[i], result[istr - i - 1]);
		}
		for (idig; idig > 0;)
		{
			idig -= base_bits;
			istr += ap_digit_traits::bit_size;
			for (istp = 0; istp < base_bits; ++istp)
			{
				buff = in.digits[idig + istp];
				buff <<= step_shifts[istp];
				bit_queue |= buff;
				for (ap_size_t i = 0; i < chars_per_step[istp]; ++i)
				{
					--istr;
					result[istr] = (bit_queue & base_mask) + '0';
					bit_queue >>= base_bits;
				}
			}
			istr += ap_digit_traits::bit_size;
		}
		result.resize(istr);
		return result;
	}

	std::string to_hex(const ap_integer_descriptor & in, bool uppercase)
	{
		return to_noremainder_base2<4, 'x'>(in, uppercase);
	}

	template <typename T>
	constexpr T get_largest_10()
	{
		T max = -1;
		T result = 1;
		while (max > 10)
		{
			result *= 10;
			max /= 10;
		}
		return result;
	}

	template <typename T>
	constexpr T get_largest_10_pow()
	{
		T max = -1;
		T result = 0;
		while (max > 10)
		{
			++result;
			max /= 10;
		}
		return result;
	}

	std::string to_dec(const ap_integer_descriptor & in)
	{
		static constexpr ap_digit_t largest_10 = ap_hidden::get_largest_10<ap_digit_t>();
		static constexpr ap_digit_t largest_10_pow = ap_hidden::get_largest_10_pow<ap_digit_t>();
		static constexpr ap_digit_t chars_in_digit = largest_10_pow + 1;
		if (in.size == 0)
		{
			return std::string{'0'};
		}
		ap_integer_descriptor buff;
		buff.digits = new ap_digit_t[in.size];
		buff.size = in.size;
		ap::ap_hidden::copy(in, buff);
		ap_digit_traits::bit_size;
		ap_size_t size = in.size + in.size / largest_10_pow + 1;
		ap_digit_t * digits_10 = new ap_digit_t[ size ];
		ap_size_t i = 0;
		ap_digit_t d;
		while (buff.size != 0)
		{
			d = ap_hidden::div_single_digit(buff, largest_10, buff).digits[0];
			digits_10[i] = d;
			++i;
		}
		--i;
		std::string result;
		result.resize(chars_in_digit + 1);
		ap_size_t j = 0;
		if (in.sign)
		{
			result[0] = '-';
			++j;
		}
		while (digits_10[i] > 0)
		{
			result[j] = digits_10[i] % 10 + '0';
			digits_10[i] /= 10;
			++j;
		}
		for (ap_size_t c = in.sign; c < (j + in.sign) / 2; ++c)
		{
			std::swap(result[c], result[j - c - 1 + in.sign]);
		}
		result.resize(j + i * largest_10_pow);
		for (i; i > 0;)
		{
			--i;
			j += largest_10_pow;
			for (ap_size_t k = 0; k < largest_10_pow; ++k)
			{
				--j;
				result[j] = digits_10[i] % 10 + '0';
				digits_10[i] /= 10;
			}
			j += largest_10_pow;
		}
		delete[] digits_10;
		delete[] buff.digits;
		return result;
	}
} // namespace ap