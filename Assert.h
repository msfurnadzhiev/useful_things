#include <iostream>

#ifndef _ASSERT_H
#define _ASSERT_H

void assertion_failed(const char* filename, int line, const char* code) {
    std::cerr
        << "test: "
        << filename << ":" << line << ":"
        << " Assertion '" << code << "' failed." << std::endl;
    std::exit(1);
}

#ifdef NDEBUG
#define assert(condition)
#else
#define assert(condition)                                       \
      ((condition)              						        \
        ? (void)0							                    \
        : assertion_failed(__FILE__, __LINE__, #condition))
#endif

#endif
