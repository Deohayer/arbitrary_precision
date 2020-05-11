#ifndef AP_INTEGER_STORAGE_FIXED_H
#define AP_INTEGER_STORAGE_FIXED_H

#include "ap_integer_storage_base.h"
#include "ap_copy.h"
#include "ap_trim.h"

namespace ap::ap_hidden
{
    template <ap_size_t Capacity>
    class ap_integer_storage_fixed : public ap_integer_storage_base
    {
        static_assert(Capacity > 0, "Byte count must be greater than zero.");
        static_assert(Capacity <= AP_MAX_DIGIT_COUNT, "Byte count must be less than AP_MAX_BYTE_COUNT");
    public:
        static constexpr ap_size_t max_capacity = Capacity;

        ap_integer_storage_fixed()
        {
            this->alloc(Capacity);
        }

        ap_integer_storage_fixed(const ap_integer_storage_fixed & other)
        {
            this->descriptor.size = Capacity;
            this->alloc(Capacity);
            ap_hidden::copy_nocheck(other.get_descriptor(), this->descriptor);
        }

        template <typename Storage>
        ap_integer_storage_fixed(const Storage & other)
        {
            this->descriptor.size = Capacity;
            this->alloc(Capacity);
            ap_hidden::copy(other.get_descriptor(), this->descriptor);
        }

        template <typename Storage>
        ap_integer_storage_fixed(Storage && other) : ap_integer_storage_base(std::move(other))
        {
            this->descriptor.size = std::min(Capacity, this->descriptor.size);
            this->realloc(Capacity);
            ap_hidden::trim(this->descriptor);
        }

        ap_integer_storage_fixed(ap_integer_storage_fixed && other) : ap_integer_storage_base(std::move(other))
        {
        }

        template <typename Storage>
        ap_integer_storage_fixed & operator=(const Storage & other)
        {
			ap_hidden::copy(other.get_descriptor(), this->descriptor);
            return *this;
        }

        ap_integer_storage_fixed & operator=(const ap_integer_storage_fixed & other)
        {
            if (this != &other)
            {
                ap_hidden::copy_nocheck(other.get_descriptor(), this->descriptor);
            }
            return *this;
        }

        template <typename Storage>
        ap_integer_storage_fixed & operator=(Storage && other)
        {
			this->ap_integer_storage_base::operator=(std::move(other));
			this->realloc(Capacity);
			this->descriptor.size = std::min(Capacity, this->descriptor.size);
			ap_hidden::trim(this->get_descriptor());
            return *this;
        }

        ap_integer_storage_fixed & operator=(ap_integer_storage_fixed && other)
        {
            if (this != &other)
            {
                this->ap_integer_storage_base::operator=(std::move(other));
            }
            return *this;
        }

        ~ap_integer_storage_fixed() = default;

        ap_size_t get_capacity() const
        {
            return Capacity;
        }

        bool set_capacity(ap_size_t capacity)
        {
            return true;
        }

        bool enlarge(ap_size_t capacity)
        {
            return true;
        }
    };
}

#endif