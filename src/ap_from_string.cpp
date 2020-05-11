#include "../include/ap_from_string.h"
#include "../include/ap_trim.h"
#include <memory>
#include <iostream>

namespace ap::ap_hidden
{
	void from_zero(const char * str, ap_size_t len, ap_integer_descriptor & out, ap_size_t estimated_size)
	{
		out.sign = ap_integer_descriptor::SIGN_NONE;
		out.size = 0;
	}

	template <ap_size_t _Literal_base_bits>
	void from_noremainder_base2(const char * str, ap_size_t len,
		ap_integer_descriptor & out, ap_size_t estimated_size)
	{
		static constexpr ap_size_t chars_in_base = ap_digit_traits::bit_size / _Literal_base_bits;
		ap_size_t full_digits = len / chars_in_base;
		ap_size_t last_digit_char_count = len % chars_in_base;
		const char * literal_chars = str + len;
		ap_digit_t out_capacity = out.size;
		out.size = std::min(out.size, full_digits);
		ap_digit_t buff = 0;
		for (ap_size_t i = 0; i < out.size; ++i)
		{
			literal_chars -= chars_in_base;
			buff = ap_hidden::unhex(literal_chars[0]);
			for (ap_size_t j = 1; j < chars_in_base; ++j)
			{
				buff <<= _Literal_base_bits;
				buff |= ap_hidden::unhex(literal_chars[j]);
			}
			out.digits[i] = buff;
		}
		ap_size_t digits_left = len - out.size * chars_in_base;
		if (out_capacity > full_digits && digits_left > 0)
		{
			digits_left = std::min(digits_left, chars_in_base);
			literal_chars -= digits_left;
			buff = ap_hidden::unhex(literal_chars[0]);
			for (ap_size_t j = 1; j < digits_left; ++j)
			{
				buff <<= _Literal_base_bits;
				buff |= ap_hidden::unhex(literal_chars[j]);
			}
			out.digits[out.size] = buff;
			++out.size;
		}
	}

	void from_bin(const char * str, ap_size_t len, ap_integer_descriptor & out, ap_size_t estimated_size)
	{
		return from_noremainder_base2<1>(str, len, out, estimated_size);
	}

	void from_oct(const char * str, ap_size_t len, ap_integer_descriptor & out, ap_size_t estimated_size)
	{
		static constexpr ap_size_t base_bits = 3;
		static constexpr ap_size_t step_shifts[base_bits] =
		{
			0,
			base_bits - ap_digit_traits::bit_size % base_bits,
			ap_digit_traits::bit_size % base_bits
		};
		static constexpr ap_size_t chars_per_step[base_bits] =
		{
			ap_digit_traits::bit_size / base_bits + 1,
			ap_digit_traits::bit_size / base_bits + step_shifts[1] % 2,
			ap_digit_traits::bit_size / base_bits
		};
		static constexpr ap_size_t triad_bits = ap_digit_traits::bit_size * base_bits;
		ap_size_t full_triads = len / ap_digit_traits::bit_size;
		ap_size_t out_full_triads = std::min(full_triads, out.size / 3);
		ap_size_t itrd = 0;
		ap_size_t iout = 0;
		const char * chars = str + len;
		ap_base_t buff = 0;
		ap_base_t queue = 0;
		for (itrd; itrd < out_full_triads; ++itrd)
		{
			for (ap_size_t i = 0; i < base_bits; ++i)
			{
				buff = 0;
				chars -= chars_per_step[i];
				for (ap_size_t j = 0; j < chars_per_step[i]; ++j)
				{
					buff <<= base_bits;
					buff |= (chars[j] - '0');
				}
				buff <<= step_shifts[i];
				queue |= buff;
				out.digits[iout] = queue;
				++iout;
				queue >>= ap_digit_traits::bit_size;
			}
		}
		ap_size_t chars_to_read;
		for (ap_size_t i = 0; i < base_bits; ++i)
		{
			if (iout == out.size || chars == str)
			{
				break;
			}
			buff = 0;
			chars_to_read = std::min(chars_per_step[i], static_cast<ap_size_t>(chars - str));
			chars -= chars_to_read;
			for (ap_size_t j = 0; j < chars_to_read; ++j)
			{
				buff <<= base_bits;
				buff |= (chars[j] - '0');
			}
			buff <<= step_shifts[i];
			queue |= buff;
			out.digits[iout] = queue;
			++iout;
			queue >>= ap_digit_traits::bit_size;
		}
		if (queue > 0 && iout < out.size)
		{
			out.digits[iout] = queue;
			++iout;
		}
		out.size = iout;
		trim(out);
	}

	void from_dec(const char * str, ap_size_t len, ap_integer_descriptor & out, ap_size_t estimated_size)
	{
        static constexpr ap_base_t half_base_mask = static_cast<ap_base_t>(~ap_base_t{0}) >> ap_digit_traits::bit_size;
        ap_base_t buff = 0;
        auto result_digits = std::make_unique<ap_digit_t[]>(estimated_size + 1);
        auto unhexed_str = std::make_unique<char[]>(len);
		ap_size_t first_divident_len;
        ap_size_t j = 0;
        ap_size_t i = 0;
        // first division, str contains chars
		buff = 0;
		while (buff < AP_BASE && i < len)
		{
			buff *= 10;
			buff += unhex(str[i]);
			++i;
		}
		first_divident_len = i;
		unhexed_str[first_divident_len - 1] = (static_cast<ap_digit_t>(buff >> ap_digit_traits::bit_size));
		buff &= half_base_mask;
		while (i < len)
		{
			buff *= 10;
			buff += unhex(str[i]);
			++i;
			while (buff < AP_BASE && i < len)
			{
				buff *= 10;
				buff += unhex(str[i]);
				unhexed_str[i - 1] = 0;
				++i;
			}
			unhexed_str[i - 1] = (static_cast<ap_digit_t>(buff >> ap_digit_traits::bit_size));
			buff &= half_base_mask;
		};
		result_digits[j] = buff;
		++j;
		i = first_divident_len;
        // second division, unhexed_str contains values
        while (i < len)
        {
			--i;
			buff = 0;
			while (buff < AP_BASE && i < len)
			{
				buff *= 10;
				buff += unhexed_str[i];
				++i;
			}
			first_divident_len = i;
			unhexed_str[first_divident_len - 1] = (static_cast<ap_digit_t>(buff >> ap_digit_traits::bit_size));
			buff &= half_base_mask;
			while (i < len)
			{
				buff *= 10;
				buff += unhexed_str[i];
				++i;
				while (buff < AP_BASE && i < len)
				{
					buff *= 10;
					buff += unhexed_str[i];
					unhexed_str[i - 1] = 0;
					++i;
				}
				unhexed_str[i - 1] = (static_cast<ap_digit_t>(buff >> ap_digit_traits::bit_size));
				buff &= half_base_mask;
			};
			result_digits[j] = buff;
			++j;
			i = first_divident_len;
		}
		if (unhexed_str[len - 1] != 0)
		{
			result_digits[j] = unhexed_str[len - 1];
			++j;
		}
        // copy to result
        out.size = std::min(out.size, j);
        for (j = 0; j < out.size; ++j)
        {
            out.digits[j] = result_digits[j];
        }
		trim(out);
	}

	void from_hex(const char * str, ap_size_t len, ap_integer_descriptor & out, ap_size_t estimated_size)
	{
		return from_noremainder_base2<4>(str, len, out, estimated_size);
	}

	ap_converter::ap_converter(const char * cstr)
	{
		if (*cstr == '-')
		{
			this->sign = ap_integer_descriptor::SIGN_MINUS;
			++cstr;
		}
		else
		{
			this->sign = ap_integer_descriptor::SIGN_NONE;
		}

		if (*cstr == '0')
		{
			++cstr;
			if (*cstr != '\0')
			{
				this->raw_digits_size = 0;
				if (*cstr == 'x')
				{
					++cstr;
					this->raw_digits = cstr;
					this->convertion_function = ap_hidden::from_hex;
					while(ap_hidden::unhex(*cstr) != NULL_CHAR)
					{
						++this->raw_digits_size;
						++cstr;
					}
					this->estimated_size = this->raw_digits_size / (ap_digit_traits::bit_size / 4) + 1;
				}
				else if (*cstr == 'b')
				{
					++cstr;
					this->raw_digits = cstr;
					this->convertion_function = ap_hidden::from_bin;
					while(*cstr == '0' || *cstr == '1')
					{
						++this->raw_digits_size;
						++cstr;
					}
					this->estimated_size = this->raw_digits_size / (ap_digit_traits::bit_size) + 1;
				}
				else if (*cstr >= '0' && *cstr <= '7')
				{
					this->raw_digits = cstr;
					++cstr;
					++this->raw_digits_size;
					this->convertion_function = ap_hidden::from_oct;
					while(*cstr >= '0' && *cstr <= '7')
					{
						++this->raw_digits_size;
						++cstr;
					}
					this->estimated_size = (this->raw_digits_size / (ap_digit_traits::bit_size) + 1) * 3;
				}
				else
				{
					this->raw_digits = "0";
					this->raw_digits_size = 1;
					this->estimated_size = 0;
					this->convertion_function = ap_hidden::from_zero;
				}
			}
			else
			{
				this->raw_digits = "0";
				this->raw_digits_size = 1;
				this->estimated_size = 0;
				this->convertion_function = ap_hidden::from_zero;
			}
			
		}
		else if (isdigit(*cstr))
		{
			this->raw_digits = cstr;
			++cstr;
			this->raw_digits_size = 1;
			this->convertion_function = ap_hidden::from_dec;
			while (isdigit(*cstr))
			{
				++this->raw_digits_size;
				++cstr;
			}
			// It is required to use log10(16) ~ 1.205 decimal digit per 4 binary digits.
			this->estimated_size = (10 * (this->raw_digits_size + 1)) / (12 * (ap_digit_traits::bit_size / 4)) + 1;
		}
		else
		{
			this->raw_digits = "0";
			this->raw_digits_size = 1;
			this->estimated_size = 0;
			this->convertion_function = ap_hidden::from_zero;
		}
	}

	void ap_converter::convert(ap_integer_descriptor & out) const
	{
		out.sign = this->sign;
		this->convertion_function(this->raw_digits, this->raw_digits_size, out, this->estimated_size);
		ap_hidden::trim(out);
	}
} // namespace ap