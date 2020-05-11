#include "../include/ap_memory.h"
   
namespace ap::ap_hidden
{
    void ap_free(void * ptr)
    {
        std::free(ptr);
    }
}