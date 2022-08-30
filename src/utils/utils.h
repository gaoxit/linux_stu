#ifndef _UTILS_
#define _UTILS_

#define CHECK(method, attach...) \
    if (method(attach) < 0)      \
    {;}




#endif