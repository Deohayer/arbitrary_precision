#ifndef AP_SUB_H
#define AP_SUB_H

#include "ap_integer_descriptor.h"

namespace ap::ap_hidden
{
	void sub(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	void sub_preparation(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	void sub_absolute(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out,
		ap_size_t diff_size);
}

#endif