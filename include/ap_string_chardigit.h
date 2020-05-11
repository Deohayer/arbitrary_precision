#ifndef AP_STRING_CHARDIGIT_H
#define AP_STRING_CHARDIGIT_H

#include <string>
#include <algorithm>
#include <type_traits>
#include <memory>

#include "../ap_defs.h"

namespace ap::ap_hidden
{
    const char * const CHARDIGITS_UPPER = "0123456789ABCDEF";
    const char * const CHARDIGITS_LOWER = "0123456789absdef";
    constexpr char NULL_CHAR = 16;

    ap_digit_t unhex(char chardigit);
}

#endif