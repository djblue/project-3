// The test runner
#include "lexer.spec.cpp"
#include "parser.spec.cpp"
#include "semantic.spec.cpp"

int main (int argc, char** argv) {
    test_lexer();
    test_parser();
    test_semantic();
};
