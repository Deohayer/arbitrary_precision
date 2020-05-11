#ifndef AP_UNARY_H
#define AP_UNARY_H

#include "ap_integer_descriptor.h"

namespace ap::ap_hidden
{
	void increment(
		const ap_integer_descriptor & in,
		ap_integer_descriptor & out);

	void decrement(
		const ap_integer_descriptor & in,
		ap_integer_descriptor & out);

	void absolute_increment(
		const ap_integer_descriptor & in,
		ap_integer_descriptor & out);

	void absolute_decrement(
		const ap_integer_descriptor & in,
		ap_integer_descriptor & out);
}

#endif