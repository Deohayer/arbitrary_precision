#include "../include/ap_integer_storage_dynamic.h"
#include "../include/ap_trim.h"

namespace ap::ap_hidden
{
	ap_integer_storage_dynamic::ap_integer_storage_dynamic(
		ap_integer_storage_dynamic && other) :
		ap_integer_storage_base(std::move(other)),
		capacity(other.get_capacity())
	{
	}

	ap_integer_storage_dynamic::ap_integer_storage_dynamic(const ap_integer_storage_dynamic & other)
	{
		this->capacity = other.get_capacity();
		this->alloc(this->capacity);
		this->descriptor.size = this->capacity;
		ap_hidden::copy_nocheck(other.get_descriptor(), this->descriptor);
	}

	ap_integer_storage_dynamic & ap_integer_storage_dynamic::operator=(const ap_integer_storage_dynamic & other)
	{
		if (this != &other)
		{
			if (this->capacity < other.capacity)
			{
				this->capacity = other.get_capacity();
				this->realloc(this->capacity);
			}
			this->descriptor.size = this->capacity;
			ap_hidden::copy_nocheck(other.get_descriptor(), this->descriptor);
		}
		return *this;
	}

	ap_integer_storage_dynamic & ap_integer_storage_dynamic::operator=(ap_integer_storage_dynamic && other)
	{
		if (this != &other)
		{
			this->capacity = other.get_capacity();
			this->ap_integer_storage_base::operator=(std::move(other));
		}
		return *this;
	}

	ap_size_t ap_integer_storage_dynamic::get_capacity() const
	{
		return this->capacity;
	}

	bool ap_integer_storage_dynamic::set_capacity(ap_size_t _capacity)
	{
		this->capacity = _capacity;
		this->descriptor.size = std::min(this->capacity, this->descriptor.size);
		if (this->realloc(this->capacity))
		{
			ap_hidden::trim(this->get_descriptor());
			return true;
		}
		return false;
	}

	bool ap_integer_storage_dynamic::enlarge(ap_size_t _capacity)
	{
		if (_capacity > this->get_capacity())
		{
			this->capacity = _capacity;
			return this->realloc(this->capacity);
		}
		return true;
	}
}