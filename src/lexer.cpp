#include "../include/lexer.h"

// string representations of types
map<lexer::types, string> lexer::type_names;
map<int, map<string,int> > lexer::dfa;

lexer::lexer () {

    keywords.push_back("if");
    keywords.push_back("else");
    keywords.push_back("while");
    keywords.push_back("for");
    keywords.push_back("return");
    keywords.push_back("int");
    keywords.push_back("integer");
    keywords.push_back("float");
    keywords.push_back("void");
    keywords.push_back("char");
    keywords.push_back("string");
    keywords.push_back("boolean");

    type_names[UNDEFINED]   = "UNDEFINED";
    type_names[OPERATOR]    = "OPERATOR";
    type_names[DELIMITER]   = "DELIMITER";
    type_names[INTEGER]     = "INTEGER";
    type_names[FLOAT]       = "FLOAT";
    type_names[HEXADECIMAL] = "HEXADECIMAL";
    type_names[OCTAL]       = "OCTAL";
    type_names[STRING]      = "STRING";
    type_names[CHAR]        = "CHARACTER";
    type_names[ID]          = "IDENTIFIER";
    type_names[KEYWORD]     = "KEYWORD";
    type_names[INTEGER]     = "INTEGER";
    type_names[BOOL]        = "BOOL";

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

lexer::token lexer::lex (string str, int line) {

    token t;
    t.line = line;
    t.text = str;

    if (str == "true" || str == "false") {
        t.type = BOOL;
        return t;
    }

    // determine if the token is a keyword
    for (vector<string>::iterator it 
        = keywords.begin(); it != keywords.end(); it++) {
        if (str.compare(*it) == 0) {
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

lexer::token lexer::lex (string str) {
    return lex(str, 0);
}
