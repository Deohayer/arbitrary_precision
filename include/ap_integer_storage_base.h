#ifndef AP_INTEGER_STORAGE_BASE_H
#define AP_INTEGER_STORAGE_BASE_H

#include "ap_integer_descriptor.h"

namespace ap::ap_hidden
{
    class ap_integer_storage_base
    {
    protected:
        
        ap_integer_descriptor descriptor;

    protected:

        bool alloc(ap_size_t capacity);

        bool realloc(ap_size_t capacity);

        void free();

        ap_integer_storage_base();

        ap_integer_storage_base(ap_size_t capacity);

        ap_integer_storage_base(const ap_integer_storage_base & other) = delete;

        ap_integer_storage_base(ap_integer_storage_base && other);

        ap_integer_storage_base & operator=(const ap_integer_storage_base & other) = delete;

        ap_integer_storage_base & operator=(ap_integer_storage_base && other);

        ~ap_integer_storage_base();

    public:

        ap_integer_descriptor get_descriptor() const;

        ap_integer_descriptor & get_descriptor();

        bool is_negative() const;

        bool is_zero() const;

        bool is_positive() const;

        void flip_sign();

        void set_sign(bool has_minus);

        ap_size_t size() const;
    };
}

#endif