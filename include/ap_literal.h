#ifndef AP_LITERAL_H
#define AP_LITERAL_H

#include "ap_integer_wrapper.h"

namespace ap::ap_hidden
{
    ap_int_d operator""_ap(const char * str);
}

#endif