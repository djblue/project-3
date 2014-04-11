#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>

int __test_count = 0; 
std::stringstream __string_buffer, __title_buffer, __error_buffer;
std::vector<std::string> __test_errors;
std::vector<std::string>::iterator it;

#define __title(text) { \
    __test_count = 0; \
    __title_buffer << \
        "\033[0;34m" << text << "\033[0;0m "; }

// a comment 

#define __end() { \
    __title_buffer << "(" << __test_count << ")"; \
    std::cerr << __title_buffer.str() << std::endl << \
        ">> " << __error_buffer.str() << std::endl; \
    for (it = __test_errors.begin(); it != __test_errors.end(); it++) { \
        std::cerr << *it << std::endl; \
    } \
    __test_errors.clear(); \
    __error_buffer.str(""); \
    __string_buffer.str(""); \
    __title_buffer.str(""); }

#define GDOT "\033[0;32m*\033[0;0m" // green dot
#define RDOT "\033[0;31m*\033[0;0m" // red dot

// custom assert macro
#define assert(result, message) { int __line = __LINE__; \
    __test_count++; \
    if ((result) == true) { \
        __error_buffer << GDOT; \
    } else { \
        __error_buffer << RDOT; \
        __string_buffer << "\033[0;31m" << __line << ": \033[0;0m" << message; \
        __test_errors.push_back(__string_buffer.str()); __string_buffer.str(""); \
    } }

#endif
