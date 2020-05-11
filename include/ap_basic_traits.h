#ifndef AP_BASIC_TRAITS_H
#define AP_BASIC_TRAITS_H

#include <limits.h>

namespace ap::ap_hidden
{
    template <typename T>
    struct ap_basic_traits
    {
        static constexpr decltype(sizeof(int)) byte_size = sizeof(T);
        static constexpr decltype(sizeof(int)) bit_size = sizeof(T) * CHAR_BIT;
    };
}

#endif