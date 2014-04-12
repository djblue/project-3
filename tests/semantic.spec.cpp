#include <vector>

#include "../include/test.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/semantic.h"

// return semantic result after evaluation
#define lexparse(expr) \
    lexer l; \
    vector<string> split = l.split(expr); \
    vector<string>::size_type i; \
    vector<token> tokens; \
    for (i = 0; i < split.size(); i++) \
        tokens.push_back(l.lex(split[i])); \
    parser p(tokens);

// return semantic result after evaluation
#define eval(expr,type,rule,message) { \
    lexparse(expr); p.rule(); \
    /*cerr << p.sm.type_stack.size() << endl; \*/ \
    assert(p.sm.type_stack.size() > 0 && \
        p.sm.type_stack.back() == type, message) }

// test if variables end up in symbol table
#define symbol(expr,id,__type,__scope,rule,message) { \
    lexparse(expr); p.rule(); \
    assert(p.sm.table.table[id].size() > 0 && \
        p.sm.table.table[id][0].type == __type && \
        p.sm.table.table[id][0].scope == __scope, message) }

// evaluate for errors
#define error(expr,error,rule,message) { \
    lexparse(expr); p.rule(); \
    if (p.sm.errors.size() > 0) { /*cerr << p.sm.errors[0].type << endl;*/ } \
    assert(p.sm.errors.size() > 0 && \
        p.sm.errors[0].type == semantic::error, message) }

#define xerror(expr,error,rule,message) { }


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

    __title("Testing Symbol Table");

    // global variables
    symbol("int i;", "i", "int", "global", program, "insert globals");
    symbol("int j;", "j", "int", "global", program, "insert globals");

    // functions
    symbol("int fib () {}", "fib_", "int", "function", program, "insert functions");
    symbol("float fib (int x) {}", "fib_int_", "float", "function", program, "insert functions");
    symbol("void strcpy (string from, string to) {}", 
        "strcpy_string_string_", "void", "function", program, "insert functions");

    // local variables scoped to function
    symbol("int fun () { int a; }", "a", "int", "fun_", program, "insert local");
    symbol("void main () { float i,j; }", "j", "float", "main_", program, "insert local");
    symbol( "void main (int arcg, string argv ) {"
                "float i,j; }", 
            "j", "float", "main_int_string_", program, "complex insert local");

    // local variables
    symbol("int i;", "i", "int", "", local, "local variables");
    symbol("float a;", "a", "float", "", local, "local variables");

    __end();

    __title("Testing Semantic Errors");

    // duplicates
    error("int i, i;", DUPLICATE_VARIABLE, program, "duplicate globals.");
    error("int i; float i;", DUPLICATE_VARIABLE, program, "duplicate globals.");
    error("int fib(int n, int n) {}", DUPLICATE_VARIABLE, program, "duplicate parameters.");
    error("int fib() {} int fib() {}", DUPLICATE_METHOD, program, "duplicate functions");

    // while expects boolean
    error("while () {}", BOOLEAN_EXPECTED, _while, "expects boolean.");
    error("while (1 + 2) {}", BOOLEAN_EXPECTED, _while, "expects boolean.");
    error("while (2*3) {}", BOOLEAN_EXPECTED, _while, "expects boolean.");

    // if expects boolean
    error("if () {}", BOOLEAN_EXPECTED, _if, "expects boolean.");
    error("if (2+4) {}", BOOLEAN_EXPECTED, _if, "expects boolean.");
    error("if (3*9) {}", BOOLEAN_EXPECTED, _if, "expects boolean.");

    error("a + b", VARIABLE_NOT_FOUND, expression, "expects not found.");
    error("int fun () { a = a + 3.14; }", VARIABLE_NOT_FOUND, program, "expects not found.");
    error("int fun () { int a; b = a; }", VARIABLE_NOT_FOUND, program, "expects not found.");

    // with variables
    error("int fun () { int a; a = a + 3.14; }", TYPE_MISMATCH, program, "expects not found.");
    error("int fun () { int a; a = a + \"hello\"; }", TYPE_MISMATCH, program, "expects not found.");

    // with function calls
    error("int fun () {} int main () { float i; i = fun(); }", TYPE_MISMATCH, program, "expects not found.");
    error("int fun () { main(); }", METHOD_NOT_FOUND, program, "expects not found.");

    // with return types
    error("int fun () { return 3.14; }", RETURN_MISMATCH, program, "expects not found.");
    error("void fun () { return 3.14; }", RETURN_MISMATCH, program, "expects not found.");

    // incorrect parameters
    error("int main () {} int foo () { main (\"hello\"); }",
        INCORRECT_PARAMETERS, program, "incorrect parameters");

    __end();

}
