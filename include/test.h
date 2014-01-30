#ifndef TEST_H
#define TEST_H

#include <iostream>

#define title(text) \
    std::cerr << \
        "\033[0;34m" << text << "\033[0;0m" \
            << std::endl << ">> " ;

#define end() \
    std::cerr << std::endl;

#define GDOT "\033[0;32m•\033[0;0m"
#define RDOT "\033[0;31m•\033[0;0m"

// custom assert macro
#define assert(result, message) \
    if ((result) == true) { \
        std::cerr << GDOT; \
    } else { \
        std::cerr << RDOT << std::endl \
            << message << std::endl; \
    }

#endif
