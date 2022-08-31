#ifndef _UTILS_
#define _UTILS_

#include <stdint.h>
#include <stdbool.h>

#define CHECK(method, attach...) \
    if (method(attach) < 0)      \
    {;}




#endif