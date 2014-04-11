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

const char* ALL = DELIMITERS OPERATORS NUMERIC ALPHA "$. \'";

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
    T0, T1, T2, T3,
    T4, T5, T6, T7,
    T8, T9, T10, T11
};

const char *type_names[] = {
    "UNDEFINED",
    "OPERATOR",
    "DELIMITER",
    "INTEGER",
    "FLOAT",
    "HEXADECIMAL",
    "OCTAL",
    "STRING",
    "CHARACTER",
    "IDENTIFIER",
    "KEYWORD",
    "INTEGER"
};

const int numwords = 12;
const char *keywords[] = {
    "if",
    "else",
    "while",
    "for",
    "return",
    "int",
    "integer",
    "float",
    "void",
    "char",
    "string",
    "boolean"
};

// A struct for basic token attributes.
struct token {
    types type;
    string text;
    int line; 
};

// This class implements a lexical analyzer
class lexer {

private:

    // key->value pair to generate deterministic finite automata
    map<int, map<string,int> > dfa;
    map<string,int>::iterator it;
    int next_state (int state, char input);

public: 

    lexer ();
    vector<string> split (string str);
    token lex (string str);
    // for testing
    token lex (string str, int line);
    friend void test_lexer ();
};

lexer::lexer () {
    
    // delimeters
    dfa[UNDEFINED][DELIMITERS]  = DELIMITER;

    // operators
    dfa[UNDEFINED][OPERATORS]   = OPERATOR;

    // integers
    dfa[UNDEFINED][NONZERO]     = INTEGER;
    dfa[INTEGER][NUMERIC]       = INTEGER;

    // floating point numbers
    dfa[UNDEFINED]["."]         = T0;
    dfa[INTEGER]["."]           = T0;
    dfa[T0][NUMERIC]            = FLOAT;
    dfa[FLOAT][NUMERIC]         = FLOAT;

    // identifiers
    dfa[UNDEFINED][ALPHA]       = ID;
    dfa[UNDEFINED]["$_"]        = ID;
    dfa[ID][ALPHA]              = ID;
    dfa[ID]["$_"]               = ID;
    dfa[ID][NUMERIC]            = ID;

    // strings
    dfa[UNDEFINED]["\""]        = T1;
    dfa[T1]["\\"]               = T8;
    dfa[T8]["\""]               = T1;
    dfa[T8][ALL]                = T1;
    dfa[T1][ALL]                = T1;
    dfa[T1]["\""]               = STRING;

    // chars
    dfa[UNDEFINED]["\'"]        = T2;
    dfa[T2]["\\"]               = T6;
    dfa[T6][ALL]                = T7;
    dfa[T7]["\'"]               = CHAR;
    dfa[T2][ALL]                = T3;
    dfa[T3]["\'"]               = CHAR;

    dfa[UNDEFINED]["0"]         = ZERO;

    // hex numbers
    dfa[ZERO]["x"]              = T5;
    dfa[T5][HEXD]               = HEXADECIMAL;
    dfa[HEXADECIMAL][HEXD]      = HEXADECIMAL;

    // octal numbers
    dfa[ZERO][OCTALD]           = OCTAL;
    dfa[OCTAL][OCTALD]          = OCTAL;
}

int lexer::next_state (int state, char input) {
    for (it = dfa[state].begin(); it != dfa[state].end(); it++) {
        if (it->first.find(input) != string::npos) {
            return it->second;
        }
    }

    return -1; // all hope is lost, abandon ship!!!
}

vector<string> lexer::split (string str) {

    // a flag to determine if currently inside a quote
    bool  inQuotes = false;
    char quote, prev = ' ';

    vector<string> tokens;
    string temp = "", split = OPERATORS DELIMITERS " \n\r";
    string::iterator it;

    for (it = str.begin(); it != str.end(); it++) {
        // ignore normal splitting rules
        if (inQuotes) {
            temp += *it;
            // found the end of a string
            if (*it == quote && prev != '\\') {
                inQuotes = false;
                tokens.push_back(temp);
                temp.clear();
            }
        // found start of a string
        } else if (*it == '\"' || *it == '\'') {
                inQuotes = true;
                if (!temp.empty()) {
                    tokens.push_back(temp);
                    temp.clear();
                }
                quote = *it;
                temp += *it;
        // found a delimiter
        } else if (split.find(*it) != string::npos) {
            if (!temp.empty()) {
                tokens.push_back(temp);
                temp.clear();
            }
            if (*it != ' ' && *it != '\r' && *it != '\n') {
                temp += *it;
                tokens.push_back(temp);
                temp.clear();
            }
        // keep building the string
        } else {
            temp += *it;
        }

        prev = *it;
    }

    if (!temp.empty() && temp[0] != ' ') { 
        tokens.push_back(temp);
    }

    return tokens;
}

token lexer::lex (string str, int line) {

    token t;
    t.line = line;
    t.text = str;

    // determine if the token is a keyword
    for (int i = 0; i < numwords; i++) {
        if (str.compare(keywords[i]) == 0) {
            t.type = KEYWORD;
            return t;
        }
    }

    int state = 0;
    string::iterator it;

    for (it = str.begin(); it != str.end(); it++) {
        state = next_state(state, *it);
    }

    if (state == ZERO) {
        state = INTEGER;
    }

    t.type = static_cast<types>((state < T0 && state > 0)? state : 0); 
    return t;
}

token lexer::lex (string str) {
    return lex(str, 0);
}

#endif // LEXER_H
