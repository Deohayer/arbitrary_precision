#include "../include/ap_integer_storage_base.h"
#include "../include/ap_memory.h"

namespace ap::ap_hidden
{

	bool ap_integer_storage_base::alloc(ap_size_t capacity)
    {
        this->descriptor.digits = ap_hidden::ap_alloc<ap_digit_t>(capacity);
        return (this->descriptor.digits != nullptr);
    }

	bool ap_integer_storage_base::realloc(ap_size_t capacity)
    {
        this->descriptor.digits = ap_hidden::ap_realloc<ap_digit_t>(this->descriptor.digits, capacity);
        return (this->descriptor.digits != nullptr);        
    }

	void ap_integer_storage_base::free()
    {
        ap_hidden::ap_free(this->descriptor.digits);
    }

	ap_integer_storage_base::ap_integer_storage_base() : descriptor(ap_integer_descriptor{nullptr, 0, 0})
    {
    }

	ap_integer_storage_base::ap_integer_storage_base(ap_size_t capacity) : ap_integer_storage_base()
    {
        this->alloc(capacity);
    }

	ap_integer_storage_base::ap_integer_storage_base(ap_integer_storage_base && other) : descriptor(other.descriptor)
    {
        other.descriptor.digits = nullptr;
    }

	ap_integer_storage_base & ap_integer_storage_base::operator=(ap_integer_storage_base && other)
    {
        if (this != &other)
        {
            this->free();
            this->descriptor = other.descriptor;
            other.descriptor.digits = nullptr;
        }
        return *this;
    }

	ap_integer_storage_base::~ap_integer_storage_base()
    {
        this->free();
    }

	ap_integer_descriptor ap_integer_storage_base::get_descriptor() const
    {
        return this->descriptor;
    }

	ap_integer_descriptor & ap_integer_storage_base::get_descriptor()
    {
        return this->descriptor;
    }

	bool ap_integer_storage_base::is_negative() const
    {
        return this->descriptor.sign;
    }

	bool ap_integer_storage_base::is_zero() const
    {
        return this->descriptor.size == 0;
    }

	bool ap_integer_storage_base::is_positive() const
    {
        return !(this->is_negative() || this->is_zero());
    }

	void ap_integer_storage_base::flip_sign()
    {
        ++this->descriptor.sign;
    }

	void ap_integer_storage_base::set_sign(bool has_minus)
    {
        this->descriptor.sign = has_minus;
    }
	ap_size_t ap_integer_storage_base::size() const
	{
		return this->descriptor.size;
	}
} // namespace ap::ap_hidden
