#ifndef AP_COPY_H
#define AP_COPY_H

#include "ap_integer_descriptor.h"

namespace ap::ap_hidden
{
    void copy(const ap_integer_descriptor & in, ap_integer_descriptor & out);

    void copy_nocheck(const ap_integer_descriptor & in, ap_integer_descriptor & out);
}

#endif