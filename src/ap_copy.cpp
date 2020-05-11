#include "../include/ap_copy.h"
#include "../include/ap_trim.h"

namespace ap::ap_hidden
{
    void copy(const ap_integer_descriptor & in, ap_integer_descriptor & out)
    {
        out.size = std::min(out.size, in.size);
        for (ap_size_t i = 0; i < out.size; ++i)
        {
            out.digits[i] = in.digits[i];
        }
        out.sign = in.sign;
        ap_hidden::trim(out);
    }

    void copy_nocheck(const ap_integer_descriptor & in, ap_integer_descriptor & out)
    {
        for (ap_size_t i = 0; i < in.size; ++i)
        {
            out.digits[i] = in.digits[i];
        }
        out.sign = in.sign;
        out.size = in.size;
    }
}
