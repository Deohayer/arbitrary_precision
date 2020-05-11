#include "../include/ap_comparison.h"

namespace ap::ap_hidden
{
	int compare(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right)
	{
		ap_comparison_result result;
		if (left.sign == right.sign)
		{
			result = ap_hidden::absolute_compare(left, right);
			if (left.sign == ap_integer_descriptor::SIGN_MINUS)
			{
				result.result = -result.result;
			}
		}
		else
		{
			if (left.sign == ap_integer_descriptor::SIGN_MINUS)
			{
				result.result = ap_comparison_result::less;
			}
			else
			{
				result.result = ap_comparison_result::greater;
			}
		}
		return result.result;
	}

	ap_comparison_result absolute_compare(
		const ap_integer_descriptor & left, 
		const ap_integer_descriptor & right)
	{
		if (left.size > right.size)
		{
			return ap_comparison_result{ap_comparison_result::greater, left.size};
		}
		else if (left.size < right.size)
		{
			return ap_comparison_result{ap_comparison_result::less, right.size};
		}
		for (ap_size_t i = left.size; i > 0;)
		{
			--i;
			if (left.digits[i] > right.digits[i])
			{
				return ap_comparison_result{ap_comparison_result::greater, i + 1};
			}
			else if (left.digits[i] < right.digits[i])
			{
				return ap_comparison_result{ap_comparison_result::less, i + 1};
			}
		}
		return ap_comparison_result{ap_comparison_result::equal, 0};
	}

	bool less(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right)
	{
		return ap_hidden::compare(left, right) == ap_comparison_result::less;
	}

	bool greater(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right)
	{
		return ap_hidden::compare(left, right) == ap_comparison_result::greater;
	}

	bool equal(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right)
	{
		return ap_hidden::compare(left, right) == ap_comparison_result::equal;
	}

	bool not_equal(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right)
	{
		return ap_hidden::compare(left, right) != ap_comparison_result::equal;
	}

	bool not_greater(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right)
	{
		return ap_hidden::compare(left, right) != ap_comparison_result::greater;
	}

	bool not_less(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right)
	{
		return ap_hidden::compare(left, right) != ap_comparison_result::less;
	}
} // namespace ap
