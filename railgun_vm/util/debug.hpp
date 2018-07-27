#ifndef _UTIL_DEBUG_HPP
#define _UTIL_DEBUG_HPP

#include <assert.h>

class Debug {
public:
    static void fatal(const char * reason) {
        printf("%s\n", reason);
        assert(false);
    }
};

#endif
