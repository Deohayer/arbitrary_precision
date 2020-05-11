#include "../include/ap_estimation.h"
#include "../include/ap_basic_conversions.h"

namespace ap::ap_hidden
{
    ap_size_t size_add(const ap_integer_descriptor & left, const ap_integer_descriptor & right)
    {
        return std::max(left.size, right.size) + 1;
    }

	ap_size_t size_sub(const ap_integer_descriptor & left, const ap_integer_descriptor & right)
    {
        return std::max(left.size, right.size) + 1;
    }

	ap_size_t size_mul(const ap_integer_descriptor & left, const ap_integer_descriptor & right)
    {
        return left.size + right.size;
    }

	ap_size_t size_quotient(const ap_integer_descriptor & left, const ap_integer_descriptor & right)
    {
        return left.size;
    }

	ap_size_t size_remainder(const ap_integer_descriptor & left, const ap_integer_descriptor & right)
    {
        return std::max(left.size, right.size);
    }

	ap_size_t size_increment(const ap_integer_descriptor & in)
    {
        return in.size + 1;
    }

	ap_size_t size_decrement(const ap_integer_descriptor & in)
    {
        return in.size + 1;
    }

	ap_size_t size_bitwise(const ap_integer_descriptor & left, const ap_integer_descriptor & right)
    {
        return std::max(left.size, right.size);
    }

	ap_size_t size_bitnot(const ap_integer_descriptor & in)
    {
        return in.size != 0 ? in.size : 1;
    }

    ap_size_t size_rshift(const ap_integer_descriptor & left, const ap_integer_descriptor & right)
    {
        if (right.size * ap_digit_traits::byte_size > ap_size_traits::byte_size)
		{
			return left.sign;
		}
		ap_size_t digit_shift_count = ap_hidden::to_basic_int<ap_size_t>(right) / ap_digit_traits::bit_size;
		return (digit_shift_count >= left.size) ? left.sign : left.size - digit_shift_count;
    }

    ap_size_t size_lshift(const ap_integer_descriptor & left, const ap_integer_descriptor & right)
    {
        if (right.size * ap_digit_traits::byte_size > ap_size_traits::byte_size)
		{
			return 0;
		}
		ap_size_t bit_shift_count = ap_hidden::to_basic_int<ap_size_t>(right);
		return left.size + bit_shift_count / ap_digit_traits::bit_size + (bit_shift_count % ap_digit_traits::bit_size != 0);
    }

    ap_size_t size_copy(const ap_integer_descriptor & in)
    {
        return in.size;
    }
}
