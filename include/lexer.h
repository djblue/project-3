#ifndef LEXER_H
#define LEXER_H

#include <map>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

// character types
#define DELIMITERS ":;()[]{},"
#define OPERATORS  "+-*/%<>=!&|"
#define NUMERIC    "0123456789"
#define OCTALD     "01234567"
#define HEXD       "0123456789ABCDEFabcdef"
#define NONZERO    "123456789"
#define ALPHA      "abcdefghijklmnopqrstuvwxyz" \
                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define ALL        "" DELIMITERS OPERATORS NUMERIC ALPHA "$. \'"


// This class implements a lexical analyzer
class lexer {

private:

    // vector of all keywords
    vector<string> keywords;
    // key->value pair to generate deterministic finite automata
    static map<int, map<string,int> > dfa;
    map<string,int>::iterator it;
    int next_state (int state, char input);

public: 

    enum types {
        UNDEFINED,
        OPERATOR,
        DELIMITER,
        INTEGER,
        FLOAT,
        HEXADECIMAL,
        OCTAL,
        STRING,
        CHAR,
        ID,
        KEYWORD,
        ZERO,
        BOOL,
        T0, T1, T2, T3,
        T4, T5, T6, T7,
        T8, T9, T10, T11
    };

    // string representations of types
    static map<types, string> type_names;

    // A struct for basic token attributes.
    struct token {
        types type;
        string text;
        int line; 
    };

    lexer ();
    vector<string> split (string str);
    token lex (string str);
    // for testing
    token lex (string str, int line);
    friend void test_lexer ();
};

#endif // LEXER_H
