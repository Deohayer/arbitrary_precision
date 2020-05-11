#include "../include/ap_string_chardigit.h"

namespace ap::ap_hidden
{
    ap_digit_t unhex(char chardigit)
    {
        if (chardigit >= '0' && chardigit <= '9')
        {
            return chardigit - '0';
        }
        else if (chardigit >= 'a' && chardigit <= 'f')
        {
            return chardigit - 'a' + 10;
        }
        else if (chardigit >= 'A' && chardigit <= 'F')
        {
            return chardigit - 'A' + 10;
        }
        else
        {
            return NULL_CHAR;
        }
	}
}
