#ifndef AP_INTEGER_DESCRIPTOR_H
#define AP_INTEGER_DESCRIPTOR_H

#include "../ap_defs.h"

namespace ap::ap_hidden
{
    struct ap_integer_descriptor
    {
        ap_digit_t * digits;
        ap_size_t sign : 1;
        ap_size_t size : ap_size_traits::bit_size - 1;

        static constexpr ap_size_t SIGN_MINUS = true;
        static constexpr ap_size_t SIGN_NONE = false;
    };
}

#endif
