#ifndef AP_MEMORY_H
#define AP_MEMORY_H

#include <cstdlib>
#include "../ap_defs.h"

namespace ap::ap_hidden
{
    template <typename T>
    T * ap_alloc(ap_size_t size)
    {
        return reinterpret_cast<T *>(std::malloc(size * sizeof(T)));
    }

    template <typename T>
    T * ap_realloc(T * ptr, ap_size_t new_size)
    {
        return reinterpret_cast<T *>(std::realloc(ptr, new_size * sizeof(T)));
    }

    void ap_free(void * ptr);
}

#endif