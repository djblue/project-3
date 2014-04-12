#include <vector>

#include "../include/test.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/semantic.h"

// return semantic result after evaluation
#define eval(expr,type,rule,message) { \
    lexer l; \
    vector<string> split = l.split(expr); \
    vector<string>::size_type i; \
    vector<token> tokens; \
    for (i = 0; i < split.size(); i++) \
        tokens.push_back(l.lex(split[i])); \
    parser p(tokens); p.rule(); \
    /*cerr << p.sm.type_stack.back() << endl; */ \
    assert(p.sm.type_stack.size() > 0 && \
        p.sm.type_stack.back() == type, message) }

void test_semantic () {

    __title("Testing Type Evaluation");

    // terminal types
    eval("2", "INTEGER", sum,"should be INTEGER");
    eval("3.14", "FLOAT", sum,"should be float");
    eval("2+2", "INTEGER", sum,"sum of two ints is an int");

    // addition with floats
    eval("2.2+2", "FLOAT", sum,"float + int = float");
    eval("2+2.2", "FLOAT", sum,"int + float = float");
    eval("2.2+2.2", "FLOAT", sum,"float + float = float");

    // string concatenation
    eval("\"hello\"+2", "STRING", sum,"string + * = string");
    eval("\"hello\"+2.2", "STRING", sum,"string + * = string");
    eval("\"hello\"+\'a\'", "STRING", sum,"string + * = string");
    eval("\"hello\"+\"a\"", "STRING", sum,"string + * = string");

    // relational operators
    eval("1 < 2", "BOOL", relational, "relational returns bool");
    eval("1.0 < 2", "BOOL", relational, "relational returns bool");
    eval("1 > 2.0", "BOOL", relational, "relational returns bool");
    eval("1.0 > 2.0", "BOOL", relational, "relational returns bool");

    // not operator
    eval("true", "BOOL", _not, "it should be bool");
    eval("!true", "BOOL", _not, "it should be bool");
    eval("!(1 < 2)", "BOOL", _not, "it should be bool");

    // and/or operators
    eval("true & false", "BOOL", _and, "it should be bool");
    eval("true & 1 < 2", "BOOL", _and, "it should be bool");
    eval("true | true ", "BOOL", expression, "it should be bool");
    eval("(true & false) | true", "BOOL", expression, "it should be bool");

    // negative stuff
    eval("-3.14", "FLOAT", expression, "-PI");
    eval("-1", "INTEGER", expression, "-1");

    // crazy expressions
    eval("(2 + 1)/(3 + 1) ", "INTEGER", expression, "it should be an integer.");
    eval("(1 < 2) | (2 < 1)", "BOOL", expression, "it should be bool");

    __end();

}
