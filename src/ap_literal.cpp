#include "../include/ap_literal.h"

namespace ap::ap_hidden
{
    ap_int_d operator""_ap(const char * str)
    {
        ap_int_d result;
        result.from_string(str);
        return result;
    }
}
