#ifndef AP_ADD_H
#define AP_ADD_H

#include "ap_integer_descriptor.h"

namespace ap::ap_hidden
{
	void add(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	void add_preparation(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	void add_absolute(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);
}

#endif