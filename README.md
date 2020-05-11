# Arbitrary Precision (AP)
Cross-platform C++17 arbitrary precision arithmetic library.
Currently it offers integer types that behave as if they are built-in int, but can store arbitrarily huge values.

## Usage
1. Clone the repo, it contains only the sources.
2. include ap_int.h header.

## Interface and features
Two types are available out of the box: </br>
int_d (dynamic-size integer) - automatically enlarges to store the result.</br>
int_f<SIZE> (fixed-size integer) - ensures that size is at least SIZE bytes (might be slightly more).</br>
Overflow is undefined behavior. No exceptions are thrown, but the result is not reliable. Wrapping is intended, but not tested properly.</br>
Move operators/constructors are used, so in most cases obtaining the result is a cheap oparation.

### Initialization
Integer can be initialized from any built-in integer type (bool is not guaranteed to work, please cast to int).</br>
Integer can be assigned or constructed from binary, octal, decimal and hexadecimal strings. Prefixes must be present.</br>
Also, instead of integer, you can use _ap literal:</br>
int_d a = 12345_ap;</br>
_ap creates dynamic integer. 

### Operators
All arithmetic, comparison and bitwise operators are available (as for int).</br>
The behavior is as follows:
* comparison, increment, decrement, sign change, addition, subtraction, multiplication behave as expected and return the result of the corresponding operation.
* quotient/remainder operators throws 0 (int) on zero division.
* bitwise operations mock two's complement behavior.
Please note, sign is stored separately, but is a part of bit pattern. Thus, if you have 8-byte int_f, and byte is 8 bits, integer is 65-bit.</br>
int_d also can be used for bitwise operations. In particular, 0 is all zeros, -1 is all ones.</br>
Left shift for negative values is arithmetic. Spared bits are filled with 1.</br>
In all other cases spared bits are filled with 0.

## Future updates
* Test wrapping.
* Add methods to access separate bit.
* Improve cout to check flags.
* Improve addition and subtraction with asm.
* Improve multiplication (Karatsuba algorithm)
* Add useful methods/functions like gcd, pow, factorial, etc.
* Add arbitrary precision float types.

