#include "../include/ap_trim.h"

namespace ap::ap_hidden
{
	void trim(ap_integer_descriptor & out)
	{
		const ap_digit_t * const digits = out.digits - 1;
		for (ap_size_t i = out.size; i > 0; --i)
		{
			if (digits[i] != 0)
			{
				out.size = i;
				return;
			}
		}
		out.size = 0;
		out.sign = false;
	}
} // namespace ap
