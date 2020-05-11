#ifndef AP_TO_STRING_H
#define AP_TO_STRING_H

#include "ap_integer_descriptor.h"
#include "ap_div.h"
#include "ap_copy.h"
#include <string>

namespace ap::ap_hidden
{
	std::string to_bin(const ap_integer_descriptor & in);

	std::string to_oct(const ap_integer_descriptor & in);

	std::string to_hex(const ap_integer_descriptor & in, bool uppercase = true);

	std::string to_dec(const ap_integer_descriptor & in);
} // namespace ap

#endif