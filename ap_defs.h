#ifndef AP_DEFS_H
#define AP_DEFS_H

#include "include/ap_basic_traits.h"
#include <algorithm>
#include <cstdint>

namespace ap::ap_hidden
{
    using ap_size_t = decltype(sizeof(int));
	using ap_digit_t = unsigned int;
	using ap_base_t = unsigned long long int;
	static_assert(sizeof(ap_base_t) == sizeof(ap_digit_t) * 2);

    using ap_size_traits = ap_basic_traits<ap_size_t>;
    using ap_digit_traits = ap_basic_traits<ap_digit_t>;
    using ap_base_traits = ap_basic_traits<ap_base_t>;

    constexpr ap_base_t AP_BASE = (ap_base_t{1} << ap_digit_traits::bit_size);
    constexpr ap_size_t AP_MAX_DIGIT_COUNT = (~ap_size_t{0}) >> (1 + ap_digit_traits::byte_size);
}

#endif