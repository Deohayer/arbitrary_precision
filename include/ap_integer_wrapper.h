#ifndef AP_INTEGER_STORAGE_WRAPPER_H
#define AP_INTEGER_STORAGE_WRAPPER_H

#include <string>
#include <iostream>
#include "ap_integer_descriptor.h"
#include "ap_integer_storage_fixed.h"
#include "ap_integer_storage_dynamic.h"
#include "ap_estimation.h"
#include "ap_basic_conversions.h"
#include "ap_add.h"
#include "ap_sub.h"
#include "ap_mul.h"
#include "ap_div.h"
#include "ap_bitwise.h"
#include "ap_shift.h"
#include "ap_unary.h"
#include "ap_comparison.h"
#include "ap_string_conversions.h"

namespace ap::ap_hidden
{
    template <typename S1, typename S2, bool First_wider>
    struct ap_widest_storage;

    template <typename S1, typename S2>
    struct ap_widest_storage<S1, S2, true>
    {
        using storage = S1;
    };

    template <typename S1, typename S2>
    struct ap_widest_storage<S1, S2, false>
    {
        using storage = S2;
    };

    template <typename S1, typename S2>
    using ap_widest_storage_t = typename ap_widest_storage<S1, S2, 
        ((S1::max_capacity >= S2::max_capacity))>::storage;

    template <typename T>
    struct ap_division_result
    {
        T quotient;
        T remainder;
    };


    template <typename S>
    class ap_integer_wrapper
    {
        template <typename>
        friend class ap_integer_wrapper;

    private:

        S storage;

        ap_integer_descriptor get_readonly_descriptor() const
        {
            return this->storage.get_descriptor();
        }

        ap_integer_descriptor & get_writeonly_descriptor(ap_size_t capacity)
        {
            this->storage.enlarge(capacity);
            ap_integer_descriptor & d = this->storage.get_descriptor();
            d.size = this->storage.get_capacity();
            return d;
        }

    public:

        ap_integer_wrapper() = default;

        template <typename T>
        ap_integer_wrapper(T value)
        {
            ap_hidden::from_basic_int<T>(value, this->get_writeonly_descriptor(ap_hidden::size_basic<T>()));
        }

        template <typename T>
        ap_integer_wrapper & operator=(T value)
        {
            ap_hidden::from_basic_int<T>(value, this->get_writeonly_descriptor(ap_hidden::size_basic<T>()));
            return *this;
        }

        ap_integer_wrapper(const std::string & value)
        {
            this->from_string(value);
        }

        ap_integer_wrapper(const char * value)
        {
            this->from_string(value);
        }

        ap_integer_wrapper & operator=(const std::string & value)
        {
            this->from_string(value);
            return *this;
        }

        ap_integer_wrapper & operator=(const char * value)
        {
            this->from_string(value);
            return *this;
        }

        template <typename S2>
        ap_integer_wrapper(ap_integer_wrapper<S2> && other) 
            : storage(std::move(other.storage))
        {
        }

        ap_integer_wrapper(ap_integer_wrapper && other)
            : storage(std::move(other.storage))
        {
        }

        template <typename S2>
        ap_integer_wrapper(const ap_integer_wrapper<S2> & other)
            : storage(other.storage)
        {
        }

        ap_integer_wrapper(const ap_integer_wrapper & other)
            : storage(other.storage)
        {
        }

        template <typename S2>
        ap_integer_wrapper & operator=(ap_integer_wrapper<S2> && other)
        {
            this->storage = std::move(other.storage);
            return *this;
        }

        ap_integer_wrapper & operator=(ap_integer_wrapper && other)
        {
            if (this != &other)
            {
                this->storage = std::move(other.storage);
            }
            return *this;
        }

        template <typename S2>
        ap_integer_wrapper & operator=(const ap_integer_wrapper<S2> & other)
        {
            this->storage = other.storage;
            return *this;
        }

        ap_integer_wrapper & operator=(const ap_integer_wrapper & other)
        {
            if (this != &other)
            {
                this->storage = other.storage;
            }
            return *this;
        }

        ~ap_integer_wrapper() = default;

        template <typename S2>
        decltype(auto) operator+(const ap_integer_wrapper<S2> & other) const
        {
            ap_integer_wrapper<ap_widest_storage_t<S, S2>> result;
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = result.get_writeonly_descriptor(ap_hidden::size_add(left, right));
            ap_hidden::add(left, right, out);
            return result;
        }

        template <typename S2>
        ap_integer_wrapper & operator+=(const ap_integer_wrapper<S2> & other)
        {
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_add(left, right));
            left.digits = out.digits;
            ap_hidden::add(left, right, out);
            return *this;
        }

        template <typename S2>
        decltype(auto) operator-(const ap_integer_wrapper<S2> & other) const
        {
            ap_integer_wrapper<ap_widest_storage_t<S, S2>> result;
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = result.get_writeonly_descriptor(ap_hidden::size_sub(left, right));
            ap_hidden::sub(left, right, out);
            return result;            
        }

        template <typename S2>
        ap_integer_wrapper & operator-=(const ap_integer_wrapper<S2> & other)
        {
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_sub(left, right));
            left.digits = out.digits;
            ap_hidden::sub(left, right, out);
            return *this;
        }

        template <typename S2>
        decltype(auto) operator*(const ap_integer_wrapper<S2> & other) const
        {
            ap_integer_wrapper<ap_widest_storage_t<S, S2>> result;
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = result.get_writeonly_descriptor(ap_hidden::size_mul(left, right));
            ap_hidden::mul(left, right, out);
            return result;   
        }

        template <typename S2>
        ap_integer_wrapper & operator*=(const ap_integer_wrapper<S2> & other)
        {
            (*this) = (*this) * other;
            return *this;   
        }

        template <typename S2>
        decltype(auto) operator/(const ap_integer_wrapper<S2> & other) const
        {
            ap_integer_wrapper<ap_widest_storage_t<S, S2>> result;
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = result.get_writeonly_descriptor(ap_hidden::size_quotient(left, right));
            ap_hidden::div_quotient(left, right, out);
            return result; 
        }

        template <typename S2>
        ap_integer_wrapper & operator/=(const ap_integer_wrapper<S2> & other)
        {
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_quotient(left, right));
            left.digits = out.digits;
            ap_hidden::div_quotient(left, right, out);
            return *this;
        }

        template <typename S2>
        decltype(auto) operator%(const ap_integer_wrapper<S2> & other) const
        {
            ap_integer_wrapper<ap_widest_storage_t<S, S2>> result;
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = result.get_writeonly_descriptor(ap_hidden::size_remainder(left, right));
            ap_hidden::div_remainder(left, right, out);
            return result;            
        }

        template <typename S2>
        ap_integer_wrapper & operator%=(const ap_integer_wrapper<S2> & other)
        {
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_remainder(left, right));
            left.digits = out.digits;
            ap_hidden::div_remainder(left, right, out);
            return *this;
        }

        template <typename S2>
        decltype(auto) divide(const ap_integer_wrapper<S2> & other) const
        {
            ap_division_result<ap_integer_wrapper<ap_widest_storage_t<S, S2>>> result;
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out_quo = result.quotient.get_writeonly_descriptor(ap_hidden::size_quotient(left, right));
            ap_integer_descriptor & out_rem = result.remainder.get_writeonly_descriptor(ap_hidden::size_remainder(left, right));
            ap_hidden::div(left, right, out_quo, out_rem);
            return result;
        }

        template <typename S2>
        decltype(auto) operator&(const ap_integer_wrapper<S2> & other) const
        {
            ap_integer_wrapper<ap_widest_storage_t<S, S2>> result;
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = result.get_writeonly_descriptor(ap_hidden::size_bitwise(left, right));
            ap_hidden::bitwise_and(left, right, out);
            return result;            
        }

        template <typename S2>
        ap_integer_wrapper & operator&=(const ap_integer_wrapper<S2> & other)
        {
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_bitwise(left, right));
            left.digits = out.digits;
            ap_hidden::bitwise_and(left, right, out);
            return *this;       
        }

        template <typename S2>
        decltype(auto) operator^(const ap_integer_wrapper<S2> & other) const
        {
            ap_integer_wrapper<ap_widest_storage_t<S, S2>> result;
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = result.get_writeonly_descriptor(ap_hidden::size_bitwise(left, right));
            ap_hidden::bitwise_xor(left, right, out);
            return result;                
        }

        template <typename S2>
        ap_integer_wrapper & operator^=(const ap_integer_wrapper<S2> & other)
        {
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_bitwise(left, right));
            left.digits = out.digits;
            ap_hidden::bitwise_xor(left, right, out);
            return *this; 
        }

        template <typename S2>
        decltype(auto) operator|(const ap_integer_wrapper<S2> & other) const
        {
            ap_integer_wrapper<ap_widest_storage_t<S, S2>> result;
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = result.get_writeonly_descriptor(ap_hidden::size_bitwise(left, right));
            ap_hidden::bitwise_or(left, right, out);
            return result;
        }

        template <typename S2>
        ap_integer_wrapper & operator|=(const ap_integer_wrapper<S2> & other)
        {
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_integer_descriptor right = other.get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_bitwise(left, right));
            left.digits = out.digits;
            ap_hidden::bitwise_or(left, right, out);
            return *this; 
        }

private:
        decltype(auto) rshift(ap_integer_descriptor right) const
        {
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_size_t estimated_size = ap_hidden::size_rshift(left, right);
            if(estimated_size == 0)
            {
                return ap_integer_wrapper{};
            }
            ap_integer_wrapper result;
            ap_base_t shift = ap_hidden::to_basic_int<ap_base_t>(right);
            ap_hidden::bitwise_rshift(left, shift, result.get_writeonly_descriptor(estimated_size));
            return result;
        }

        decltype(auto) lshift(ap_integer_descriptor right) const
        {
            ap_integer_descriptor left = this->get_readonly_descriptor();
            ap_size_t estimated_size = ap_hidden::size_lshift(left, right);
            if(estimated_size == 0)
            {
                return ap_integer_wrapper{};
            }
            ap_integer_wrapper result;
            ap_base_t shift = ap_hidden::to_basic_int<ap_base_t>(right);
            ap_hidden::bitwise_lshift(left, shift, result.get_writeonly_descriptor(estimated_size));
            return result;     
        }

public:
        template <typename S2>
        decltype(auto) operator>>(const ap_integer_wrapper<S2> & other) const
        {
            ap_integer_descriptor right = other.get_readonly_descriptor();
            if (other.is_negative())
            {
                right.sign = 0;
                return this->lshift(right);
            }
            else
            {
                return this->rshift(right);
            }
        }

        template <typename S2>
        ap_integer_wrapper & operator>>=(const ap_integer_wrapper<S2> & other)
        {
            (*this) = ((*this) >> other);
            return *this;
        }

        template <typename S2>
        decltype(auto) operator<<(const ap_integer_wrapper<S2> & other) const
        {
            ap_integer_descriptor right = other.get_readonly_descriptor();
            if (other.is_negative())
            {
                right.sign = 0;
                return this->rshift(right);
            }
            else
            {
                return this->lshift(right);
            }     
        }

        template <typename S2>
        ap_integer_wrapper &  operator<<=(const ap_integer_wrapper<S2> & other)
        {
            (*this) = ((*this) << other);
            return *this;           
        }

        ap_integer_wrapper & operator++()
        {
            ap_integer_descriptor in = this->get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_increment(in));
            in.digits = out.digits;
            ap_hidden::increment(in, out);
            return *this;
        }

        ap_integer_wrapper operator++(int)
        {
            ap_integer_wrapper result = *this;
            ap_integer_descriptor in = this->get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_increment(in));
            in.digits = out.digits;
            ap_hidden::increment(in, out);
            return result;
        }

        ap_integer_wrapper & operator--()
        {
            ap_integer_descriptor in = this->get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_decrement(in));
            in.digits = out.digits;
            ap_hidden::decrement(in, out);
            return *this;            
        }

        ap_integer_wrapper operator--(int)
        {
            ap_integer_wrapper result = *this;
            ap_integer_descriptor in = this->get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_decrement(in));
            in.digits = out.digits;
            ap_hidden::decrement(in, out);
            return result;            
        }

        ap_integer_wrapper operator~() const
        {
            ap_integer_wrapper result;
            ap_integer_descriptor in = this->get_readonly_descriptor();
            ap_integer_descriptor & out = result.get_writeonly_descriptor(ap_hidden::size_bitnot(in));
            ap_hidden::bitwise_not(in, out);
            return result;  
        }

        ap_integer_wrapper operator-() const &
        {
            ap_integer_wrapper result = *this;
            return result.uminus();
        }

        ap_integer_wrapper & operator-() &&
        {
            return this->uminus();
        }

        ap_integer_wrapper operator+() const &
        {
            return *this;
        }

        ap_integer_wrapper & operator+() &&
        {
            return *this;
        }

        ap_integer_wrapper & uminus()
        {
            if (!this->is_zero())
            {
                this->storage.flip_sign();
            }
            return *this;
        }

        ap_integer_wrapper & uplus()
        {
            return *this;
        }

        ap_integer_wrapper & unot()
        {
            ap_integer_descriptor in = this->get_readonly_descriptor();
            ap_integer_descriptor & out = this->get_writeonly_descriptor(ap_hidden::size_bitnot(in));
            in.digits = out.digits;
            ap_hidden::bitwise_not(in, out);
            return *this;
        }

        ap_integer_wrapper & set_sign(bool negative)
        {
            if (!this->is_zero())
            {
                this->storage.set_sign(negative);
            }
            return *this;
        }

        template <typename S2>
        bool operator==(const ap_integer_wrapper<S2> & other) const
        {
            return ap_hidden::equal(
                this->get_readonly_descriptor(),
                other.get_readonly_descriptor()
            );
        }

        template <typename S2>
        bool operator!=(const ap_integer_wrapper<S2> & other) const
        {
            return ap_hidden::not_equal(
                this->get_readonly_descriptor(),
                other.get_readonly_descriptor()
            );
        }  

        template <typename S2>
        bool operator<=(const ap_integer_wrapper<S2> & other) const
        {
            return ap_hidden::not_greater(
                this->get_readonly_descriptor(),
                other.get_readonly_descriptor()
            );
        }

        template <typename S2>
        bool operator>=(const ap_integer_wrapper<S2> & other) const
        {
            return ap_hidden::not_less(
                this->get_readonly_descriptor(),
                other.get_readonly_descriptor()
            );
        }

        template <typename S2>
        bool operator<(const ap_integer_wrapper<S2> & other) const
        {
            return ap_hidden::less(
                this->get_readonly_descriptor(),
                other.get_readonly_descriptor()
            );
        }

        template <typename S2>
        bool operator>(const ap_integer_wrapper<S2> & other) const
        {
            return ap_hidden::greater(
                this->get_readonly_descriptor(),
                other.get_readonly_descriptor()
            );
        }

        template <typename S2>
        int compare(const ap_integer_wrapper<S2> & other) const
        {
            return ap_hidden::compare(
                this->get_readonly_descriptor(),
                other.get_readonly_descriptor()
            );
        }

        bool is_negative() const
        {
            return this->storage.is_negative();
        }

        bool is_positive() const
        {
            return this->storage.is_positive();
        }

        bool is_zero() const
        {
            return this->storage.is_zero();
        }

        std::string to_hex(bool uppercase = true) const
        {
            return ap_hidden::to_hex(
                this->get_readonly_descriptor(),
                uppercase
            );
        }

        std::string to_dec() const
        {
            return ap_hidden::to_dec(
                this->get_readonly_descriptor()
            );
        }

        std::string to_oct() const
        {
            return ap_hidden::to_oct(
                this->get_readonly_descriptor()
            );
        }

        std::string to_bin() const
        {
            return ap_hidden::to_bin(
                this->get_readonly_descriptor()
            );
        }

        ap_integer_wrapper & from_string(const char * str)
        {
            ap_converter converter{str};
            converter.convert(
                this->get_writeonly_descriptor(converter.estimated_size)
            );
            return *this;
        }

        ap_integer_wrapper & from_string(const std::string & str)
        {
            return this->from_string(str.c_str());
        }

        template <typename T>
        explicit operator T() const
        {
            return ap_hidden::to_basic_int<T>(this->get_readonly_descriptor());
        }

        operator bool() const
        {
            return !this->is_zero();
        }
    };

    template <typename T>
	std::ostream & operator<<(std::ostream & os, const ap_integer_wrapper<T> & in)
	{
		return os << in.to_dec();
	}

    template <typename T>
	std::istream & operator>>(std::istream & is, ap_integer_wrapper<T> & out)
	{
		std::string buff;
		is >> buff;
		out.from_cstring(buff);
		return is;
	}

	template <ap_size_t Digits_count>
    using ap_int_f = ap_integer_wrapper<ap_integer_storage_fixed<Digits_count>>;

    using ap_int_d = ap_integer_wrapper<ap_integer_storage_dynamic>;
}

#endif