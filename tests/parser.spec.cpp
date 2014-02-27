#include "../include/parser.h"
#include "../include/test.h"

#include <vector>
#include <string>

// assert lex-parse macro
#define alp(expr, state, rule, message) { \
    lexer l; \
    std::vector<std::string> split = l.split(expr); \
    std::vector<std::string>::size_type i; \
    std::vector<token> tokens; \
    for (i = 0; i < split.size(); i++) \
        tokens.push_back(l.lex(split[i])); \
    parser p(tokens); \
    assert(p.rule() == state, message); }

void test_parser () {

    try {
    
    title("Testing the Parser");

    alp("",          true, program, "Empty string is a valid program.");
    alp("int i;",    true, program, "Valid global");
    alp("int i, j;", true, program, "Valid global");

    alp("int fib () {}", true, program, "Valid function");
    alp("int fib (int n) {}", true, 
        program, "Valid function with parameters.");
    alp("int max (int x, int y) {}", true, 
        program, "Valid function with multiple parameters");

    alp("void one () { int i; }", true,
        program, "Valid function with one declaration.");
    alp("void one () { int i, j; }", true,
        program, "Valid function with two declaration.");

    alp("void one () { return 1 + 2; }", true,
        program, "Valid function with return expression");

    alp("i = 0;", true, assign, "Valid assignment");

    alp("if (x < 0)", true, _if, "Basic if statement");

    // error causing expressions 
    alp("1*2+", false, program, "Malformed Expression");

    end();

    title("Testing Expressions");

    alp("0", true, expression, "0 is a valid expression");

    alp("true && false", true, expression, "Valid logical expression.");

    alp("'a'",  true, terminal, "Valid character terminal.");
    alp("\"string\"",  true, terminal, "Valid string terminal.");
    alp("0",    true, terminal, "Valid integer terminal.");
    alp("1000", true, terminal, "Valid integer terminal.");
    alp("3.14", true, terminal, "Valid float terminal.");
    alp("hello",true, terminal, "Valid id terminal.");
    alp("true", true, terminal, "Valid true terminal.");
    alp("false",true, terminal, "Valid false terminal.");

    alp("a<b", true, relational, "Valid relational statement1.")
    alp("a>b", true, relational, "Valid relational statement2.")

    alp("a==b", true, relational, "Valid relational statement3.")
    alp("a!=b", true, relational, "Valid relational statement4.")

    alp("1+2", true, sum, "We can add 2 numbers.");
    alp("1-2", true, sum, "We can substract 2 numbers.");
    alp("1/2", true, product, "We can divide 2 numbers.");
    alp("1*2", true, product, "We can multiply 2 numbers.");
    alp("1%2", true, product, "We can mod 2 numbers.");

    alp("3 % 2 == 1", true, expression, "Complex expression.");

    end();

    } catch (token t) {
        end();
        std::cout << "ERROR: " << t.text << endl;
    }
}
