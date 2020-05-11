#ifndef AP_BITWISE_H
#define AP_BITWISE_H

#include "ap_integer_descriptor.h"

namespace ap::ap_hidden
{
	using ap_binary_bitwise_operation = ap_digit_t(*)(ap_digit_t left, ap_digit_t right);

	struct ap_complementer
	{
	private:
		ap_digit_t complement_negative(ap_digit_t digit);

		ap_digit_t complement_positive(ap_digit_t digit);

		ap_digit_t (ap_complementer::* complement_method)(ap_digit_t);

		ap_base_t carry;
	public:
		ap_complementer(bool negative);
		
		ap_digit_t complement(ap_digit_t digit);
	};

	void bitwise_or(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	void bitwise_and(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	void bitwise_xor(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out);

	void bitwise_operation_preparation(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out,
		ap_binary_bitwise_operation operation); 

	void bitwise_operation_absolute(
		const ap_integer_descriptor & left,
		const ap_integer_descriptor & right,
		ap_integer_descriptor & out,
		ap_binary_bitwise_operation operation);

	void bitwise_not(
		const ap_integer_descriptor & in,
		ap_integer_descriptor & out);

}

#endif