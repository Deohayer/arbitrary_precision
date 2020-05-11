#ifndef AP_COMPARISON_H
#define AP_COMPARISON_H

#include "ap_integer_descriptor.h"

namespace ap::ap_hidden
{
	struct ap_comparison_result
	{
		int result;
		ap_size_t diff_size;

		static constexpr int equal = 0;
		static constexpr int greater = 1;
		static constexpr int less = -greater;
	};

    int compare(
        const ap_integer_descriptor &left,
        const ap_integer_descriptor &right);

    ap_comparison_result absolute_compare(
        const ap_integer_descriptor &left,
        const ap_integer_descriptor &right);

    bool less(
        const ap_integer_descriptor &left,
        const ap_integer_descriptor &right);

    bool greater(
        const ap_integer_descriptor &left,
        const ap_integer_descriptor &right);

    bool equal(
        const ap_integer_descriptor &left,
        const ap_integer_descriptor &right);

    bool not_equal(
        const ap_integer_descriptor &left,
        const ap_integer_descriptor &right);

    bool not_greater(
        const ap_integer_descriptor &left,
        const ap_integer_descriptor &right);

    bool not_less(
        const ap_integer_descriptor &left,
        const ap_integer_descriptor &right);
}


#endif