#ifndef AP_SHIFT_H
#define AP_SHIFT_H

#include "ap_integer_descriptor.h"

namespace ap::ap_hidden
{
	void bitwise_rshift(
		const ap_integer_descriptor & left,
		ap_size_t bits,
		ap_integer_descriptor & out);

	void bitwise_lshift(
		const ap_integer_descriptor & left,
		ap_size_t bits,
		ap_integer_descriptor & out);
}

#endif