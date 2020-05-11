#ifndef AP_INT_H
#define AP_INT_H

#include "include/ap_literal.h"

namespace ap
{
    using ap_hidden::ap_size_t;

    using ap_hidden::AP_MAX_DIGIT_COUNT;

    constexpr ap_size_t AP_BYTES_PER_DIGIT = ap_hidden::ap_digit_traits::byte_size;

    constexpr ap_size_t AP_MAX_BYTE_COUNT = AP_MAX_DIGIT_COUNT * AP_BYTES_PER_DIGIT;

    using ap_hidden::operator""_ap;

    template <ap_size_t _Bytes>
    using int_f = ap_hidden::ap_int_f<_Bytes / AP_BYTES_PER_DIGIT + (_Bytes % AP_BYTES_PER_DIGIT != 0)>;

    using int_d = ap_hidden::ap_int_d;
}

#endif