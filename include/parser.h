#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "lexer.h"
#include "semantic.h"

#define text(s) { \
    if (peek().text != s) { report(peek(), s); return false; } \
    else { shift(); /*cerr << "found " << s << endl; */}}

#define type(t) { \
    if (peek().type != t) { report(peek(), lexer::type_names[t]); return false; } \
    else if (peek().type == lexer::ID) { current_id = shift().text; } \
    else if (peek().type == lexer::KEYWORD) { current_keyword = shift().text; } \
    else { shift(); /*cerr << "found " << type_names[t] << endl;*/ }}

using namespace std;

// This class implements a syntactical analyzer.
// It is a predictive-recursive-decent parser.
class parser {

private:

    semantic sm;
    string current_scope, current_id, current_keyword;
    int line_number;
    vector<lexer::token> tokens;
    vector<lexer::token>::size_type current_token;


    struct error {
        string expected;
        lexer::token  recieved;
    };

    vector<error> errors;
    vector<error>::iterator it;

    void report(lexer::token recieved, string expected);

    void error_recovery(string end);

    bool expect(string str, string recovery);
    bool expect(lexer::types t, string recover);

    lexer::token shift ();
    lexer::token peek ();
    void unshift ();

    bool status;

    // grammar rules
    bool program ();

    bool global ();
    bool function ();
    bool parameters ();

    bool line ();

    bool block ();
    bool local ();
    bool assign ();
    bool _if ();
    bool _while ();
    bool _return ();
    bool call();

    bool expression ();

    bool _and ();
    bool _or ();
    bool _not ();
    bool relational ();

    bool sum ();
    bool product ();
    bool sign ();

    bool terminal ();

public: 

    parser () {};
    parser (vector<lexer::token> tokens);

    bool parse ();
    void print(ostream& out);

    friend void test_parser ();
    friend void test_semantic ();
};

#endif // PARSER_H
