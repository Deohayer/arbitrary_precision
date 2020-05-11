#ifndef AP_FROM_STRING_H
#define AP_FROM_STRING_H

#include "ap_integer_descriptor.h"
#include "ap_div.h"
#include "ap_copy.h"
#include "ap_string_chardigit.h"
#include <string>

namespace ap::ap_hidden
{
	using ap_from_base_function = void(*)(const char *, ap_size_t, ap_integer_descriptor &, ap_size_t);

	struct ap_converter
	{
		ap_from_base_function convertion_function;
		const char * raw_digits;
		ap_size_t raw_digits_size;
		ap_size_t estimated_size;
		bool sign;

		ap_converter(const char * cstr);

		void convert(ap_integer_descriptor & out) const;
	};

	void from_zero(const char * str, ap_size_t len, ap_integer_descriptor & out, ap_size_t estimated_size);

	void from_bin(const char * str, ap_size_t len, ap_integer_descriptor & out, ap_size_t estimated_size);

	void from_oct(const char * str, ap_size_t len, ap_integer_descriptor & out, ap_size_t estimated_size);

	void from_dec(const char * str, ap_size_t len, ap_integer_descriptor & out, ap_size_t estimated_size);

	void from_hex(const char * str, ap_size_t len, ap_integer_descriptor & out, ap_size_t estimated_size);
} 

#endif