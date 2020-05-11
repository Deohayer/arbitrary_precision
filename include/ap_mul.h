#ifndef AP_MUL_H
#define AP_MUL_H

#include "ap_integer_descriptor.h"

namespace ap::ap_hidden
{
	void mul(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);
	
	void mul_preparation(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	void mul_absolute(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	void mul_step(
		const ap_integer_descriptor & left,
		ap_base_t right_digit,
		ap_integer_descriptor & out,
		ap_size_t out_capacity);
}

#endif 