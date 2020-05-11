#include "../include/ap_bitwise.h"
#include "../include/ap_trim.h"
#include "../include/ap_unary.h"

namespace ap::ap_hidden
{
    ap_digit_t ap_complementer::complement_negative(ap_digit_t digit)
    {
        carry += static_cast<ap_digit_t>(~digit);
        digit = carry;
        carry >>= ap_digit_traits::bit_size;
        return digit;
    }

	ap_digit_t ap_complementer::complement_positive(ap_digit_t digit)
    {
        return digit;
    }

    ap_complementer::ap_complementer(bool negative)
    {
        if (negative)
        {
            this->complement_method = &ap_complementer::complement_negative;
            carry = 1;
        }
        else
        {
            complement_method = &ap_complementer::complement_positive;
            carry = 0;
        }
    }
		
	ap_digit_t ap_complementer::complement(ap_digit_t digit)
    {
        return (this->*complement_method)(digit);
    }

	void bitwise_or(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
    {
        bitwise_operation_preparation(left, right, out, [](ap_digit_t l, ap_digit_t r)
        {
            return static_cast<ap_digit_t>(l | r);
        });
    }

	void bitwise_and(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
    {
        bitwise_operation_preparation(left, right, out, [](ap_digit_t l, ap_digit_t r)
        {
            return static_cast<ap_digit_t>(l & r);
        });
    }

	void bitwise_xor(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
    {
        bitwise_operation_preparation(left, right, out, [](ap_digit_t l, ap_digit_t r)
        {
            return static_cast<ap_digit_t>(l ^ r);
        });
    }

	void bitwise_operation_preparation(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out,
		ap_binary_bitwise_operation operation)
    {
        if (left.size > right.size)
        {
            bitwise_operation_absolute(left, right, out, operation);
        }
        else
        {
            bitwise_operation_absolute(right, left, out, operation);
        }
    }

	void bitwise_operation_absolute(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out,
		ap_binary_bitwise_operation operation)
    {
        out.size = std::min(left.size, out.size);
        out.sign = operation(left.sign, right.sign);
        ap_complementer c_to_left{left.sign == 1};
        ap_complementer c_to_right{right.sign == 1};
        ap_complementer c_from{out.sign == 1};
        ap_digit_t zero = right.sign ? -1 : 0;
        ap_size_t i = 0;
        for (i; i < right.size; ++i)
        {
            out.digits[i] = c_from.complement(operation(
                c_to_left.complement(left.digits[i]),
                c_to_right.complement(right.digits[i])
            ));
        }
        for (i; i < out.size; ++i)
        {
            out.digits[i] = c_from.complement(operation(
                c_to_left.complement(left.digits[i]),
                zero
            ));
        }
        ap_hidden::trim(out);
    }

    void bitwise_not(
		const ap_integer_descriptor & in,
		ap_integer_descriptor & out)
    {
        if (in.size == 0)
        {
            out.size = 1;
            out.digits[0] = 1;
            out.sign = 1;
            return;
        }
        ap_complementer c_to{in.sign == 1};
        ap_complementer c_from{ in.sign != 1 };
        for (ap_size_t i = 0; i < in.size; ++i)
        {
            out.digits[i] = c_from.complement(~c_to.complement(in.digits[i]));
        }
        out.size = in.size;
        out.sign = (in.sign + 1);
        trim(out);
    }
} 
