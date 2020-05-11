#ifndef AP_INTEGER_STORAGE_DYNAMIC_H
#define AP_INTEGER_STORAGE_DYNAMIC_H

#include "ap_integer_storage_base.h"
#include "ap_copy.h"

namespace ap::ap_hidden
{
    class ap_integer_storage_dynamic : public ap_integer_storage_base
    {
    private:

        ap_size_t capacity = 0;

    public:
        static constexpr ap_size_t max_capacity = AP_MAX_DIGIT_COUNT;

        ap_integer_storage_dynamic() = default;

        ap_integer_storage_dynamic(const ap_integer_storage_dynamic & other);

        template <typename Storage>
        ap_integer_storage_dynamic(const Storage & other)
        {
            this->capacity = other.get_capacity();
            this->alloc(this->capacity);
            this->descriptor.size = this->capacity;
            ap_hidden::copy_nocheck(other.get_descriptor(), this->descriptor);
        }

        ap_integer_storage_dynamic(ap_integer_storage_dynamic && other);

        template <typename Storage>
        ap_integer_storage_dynamic(Storage && other) : 
            ap_integer_storage_base(std::move(other)), 
            capacity(other.get_capacity())
        {
        }

        ap_integer_storage_dynamic & operator=(const ap_integer_storage_dynamic & other);

        template <typename Storage>
        ap_integer_storage_dynamic & operator=(const Storage & other)
        {
			this->capacity = other.get_capacity();
			this->free();
			this->alloc(this->capacity);
			this->descriptor.size = this->capacity;
			ap_hidden::copy_nocheck(other.get_descriptor(), this->descriptor);
            return *this;
        }

        ap_integer_storage_dynamic & operator=(ap_integer_storage_dynamic && other);

        template <typename Storage>
        ap_integer_storage_dynamic & operator=(Storage && other)
		{
			this->capacity = other.get_capacity();
			this->ap_integer_storage_base::operator=(std::move(other));
            return *this;
        }

        ~ap_integer_storage_dynamic() = default;

        ap_size_t get_capacity() const;

        bool set_capacity(ap_size_t capacity);

        bool enlarge(ap_size_t capacity);
    };
}

#endif