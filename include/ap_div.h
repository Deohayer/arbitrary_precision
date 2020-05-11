#ifndef AP_DIV_H
#define AP_DIV_H

#include "ap_integer_descriptor.h"
#include <memory>
#include "ap_memory.h"

namespace ap::ap_hidden
{
	using ap_normalizer_digit_ptr = std::unique_ptr<ap_digit_t[], decltype(&ap_hidden::ap_free)>;

	struct ap_normalized_integer
	{
		ap_normalizer_digit_ptr digits;
		ap_size_t size;
		ap_digit_t normalizer;

		ap_normalized_integer(ap_size_t capacity, ap_base_t normalizer);
	};
	
	void denormalize(
		const ap_normalized_integer & remainder,
		ap_integer_descriptor & out);
	
	ap_base_t normalize(
		const ap_integer_descriptor & in, 
		ap_base_t normalizer,
		ap_digit_t * out);

	void div(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out_quo,
		ap_integer_descriptor & out_rem);

	void div_quotient(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	void div_remainder(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	ap_normalized_integer div_single_digit(
		const ap_integer_descriptor & left,
		ap_digit_t digit,
		ap_integer_descriptor & out);

	ap_normalized_integer div_preparation(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	ap_normalized_integer div_absolute(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);
}

#endif