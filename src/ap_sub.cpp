#include "../include/ap_sub.h"
#include "../include/ap_add.h"
#include "../include/ap_comparison.h"
#include "../include/ap_trim.h"

namespace ap::ap_hidden
{
	void sub(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		if (left.sign != right.sign)
		{
			ap_hidden::add_preparation(left, right, out);
		}
		else
		{
			ap_hidden::sub_preparation(left, right, out);
		}
	}

	void sub_preparation(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out)
	{
		ap_comparison_result comp_res = ap_hidden::absolute_compare(left, right);
		switch (comp_res.result)
		{
		case ap_comparison_result::greater:
			ap_hidden::sub_absolute(left, right, out, comp_res.diff_size);
			out.sign = left.sign;
			break;
		case ap_comparison_result::less:
			ap_hidden::sub_absolute(right, left, out, comp_res.diff_size);
			out.sign = left.sign + ap_integer_descriptor::SIGN_MINUS;
			break;
		default: // equal
			out.size = 0;
			out.sign = ap_integer_descriptor::SIGN_NONE;
			return;
		}
		ap_hidden::trim(out);
	}

	void sub_absolute(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out,
		ap_size_t diff_size)
	{
		ap_size_t right_stop_index = std::min(right.size, diff_size);
		ap_size_t left_stop_index = std::min(out.size, diff_size);
		ap_base_t borrow = 0;
		ap_size_t borrow_shift = ap_base_traits::bit_size - 1;
		ap_size_t i = 0;
		for (i; i < right_stop_index; ++i)
		{
			borrow = left.digits[i] - borrow;
			borrow -= right.digits[i];
			out.digits[i] = borrow;
			borrow >>= borrow_shift;
		}
		for (i; borrow != 0; ++i)
		{
			borrow = left.digits[i] - borrow;
			out.digits[i] = borrow;
			borrow >>= borrow_shift;
		}
		for (i; i < left_stop_index; ++i)
		{
			out.digits[i] = left.digits[i];
		}
		out.size = i;
	}
} // namespace ap