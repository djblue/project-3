#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

std::stringstream string_buffer;
std::vector<std::string> test_errors;
std::vector<std::string>::iterator it;

#define title(text) \
    std::cerr << \
        "\033[0;34m" << text << "\033[0;0m" \
            << std::endl << ">> " ;

#define end() \
    std::cerr << std::endl; \
    for (it = test_errors.begin(); it != test_errors.end(); it++) { \
        std::cerr << *it << std::endl; \
    } \
    test_errors.clear();

#define GDOT "\033[0;32m*\033[0;0m" // green dot
#define RDOT "\033[0;31m*\033[0;0m" // red dot

// custom assert macro
#define assert(result, message) \
    if ((result) == true) { \
        std::cerr << GDOT; \
    } else { \
        std::cerr << RDOT; \
        string_buffer << message; \
        test_errors.push_back(string_buffer.str()); \
        string_buffer.str(""); \
    }

#endif
