#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "lexer.h"

#define text(s) { \
    if (shift().text != s) { error(s); return false; } \
    else { cerr << "found " << s << endl; }}

#define type(t) { \
    if (shift().type != t) { error(type_names[t]); return false; } \
    else { cerr << "found " << type_names[t] << endl; }}

using namespace std;

// This class implements a syntactical analyzer.
// It is a predictive-recursive-decent parser.
class parser {

private:
    vector<token> tokens;
    vector<token>::size_type current_token;

    struct error {
        string expected;
        token  recieved;
    };
    vector<error> errors;
    vector<error>::iterator it;


    token shift ();
    token peek ();
    void unshift ();


    // grammar rules
    bool program ();

    bool global ();
    bool function ();
    bool parameters ();

    bool line ();

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
    void error (string);

public: 

    parser () {};
    parser (vector<token> tokens);

    bool parse ();
    string error_report();

    friend void test_parser ();
};

// Report the errors son, don't be a fool.
void parser::error (string str) {
    struct error e;
    e.expected = str;
    e.recieved = tokens[current_token-1];
    errors.push_back(e);
    cerr << "Line " 
         << tokens[current_token-1].line 
         << ": expected "
         << str 
         << " recieved \'" 
         << tokens[current_token-1].text 
         << "\'"<< endl;
}

string parser::error_report() {
    stringstream ss;
    for (it = errors.begin(); it != errors.end(); it++) {
        ss << "Line " 
           << (*it).recieved.line 
           << ": expected "
           << (*it).expected 
           << " recieved \'" 
           << (*it).recieved.line 
           << "\'"<< endl;
    }
    return ss.str();
}

parser::parser (vector<token> tokens) {
    current_token = 0;
    this->tokens = tokens;
}
token parser::shift () {
    if (current_token < tokens.size()) {
        token t = tokens[current_token];
        current_token++;
        return t;
    } else {
        token t;
        t.type =  UNDEFINED;
        t.text = "#";
        //cout << "(" << tokens.size() << ","<< current_token << ")" << endl;
        return t;
    }
}
token parser::peek () {
    if (current_token < tokens.size()) {
        token t = tokens[current_token];
        return t;
    } else {
        token t;
        t.type =  UNDEFINED;
        t.text = "#";
        //cout << "(" << tokens.size() << ","<< current_token << ")" << endl;
        return t;
    }
}
void parser::unshift () {
    current_token--;
}
bool parser::parse () {

    if (program() && current_token == tokens.size() - 1) {
        cout << "Parse Successful" << endl;
        return true;
    } else {
        cout << "Parse Failure" << endl;
        return false;
    }
}
bool parser::program () {

    if (tokens.size() == 0) {
        return true;
    }

    while (current_token < tokens.size()) {
        if (shift().type != KEYWORD) return false;
        if (shift().type != ID) return false;
        if (peek().text == "(") {
            function();
        } else {
            global();
        }
    }

    return true;
}
bool parser::global () {
    while (shift().text == ",") {
        if (shift().type != ID)  return false;
    }
    unshift();
    if (shift().text != ";")  {
        unshift();
        return false;
    }
    return true;
}
bool parser::function () {
    text("(");
    parameters();
    text(")");
    text("{");
    while(line());
    text("}");
    return true;
}
bool parser::parameters () {
    do {
        if (shift().type != KEYWORD) {
            unshift();
            return false;
        }
        if (shift().type != ID) {
            unshift();
            return false;
        }
    } while (shift().text == ",");
    unshift();
    return true;
}
bool parser::line () {

    token next = shift();
    
    if (next.text == "if") {
        unshift();
        if (_if()) return true;
    }

    else if (next.type == KEYWORD) {
        if (local()) return true;
    }

    else if (next.text == "return") {
        if (_return()) return true;
    }

    else if (next.type == ID) {
        token t = shift();
        
        if (t.text == "=") {
            if (expression() && shift().text == ";") return true;
            else {
                unshift();
                unshift();
            }
        } else {
            call();
        }
    }
    unshift();

    return false;
}
bool parser::local () {
    do {
        type(ID);
        if (shift().text == "=") {
            if(!expression()) return false;
        } else {
            unshift();
        }
    } while (shift().text == ",");
    unshift();
    text(";");
    return true;
}
bool parser::assign () {
    return false; 
}
bool parser::_if () {
    
    text("if");

    text("(");
    if (!expression()) return false;
    text(")");
    if (peek().text == "{") {
        text("{");
        while(line());
        text("}");
    } else {
        line();
    }

    if (peek().text == "else") {
        text("else");
        if (peek().text == "{") {
            text("{");
            while(line());
            text("}");
        } else {
            line();
        }
    }

    return true;
}
bool parser::_while () {
    
}
bool parser::_return () {
    if (!expression()) return false;
    text(";");   
    return true;
}
bool parser::call() {
    type(ID);
    text ("(");

    do {
        if (!expression()) {
            return false;
        }
    } while (shift().text == ",");
    unshift();

    text(")");
    text(";");
    return true;
}
bool parser::expression () {
    do {
        if (!_and()) return false; 
    } while (shift().text == "|");
    unshift();
    return true;
}
bool parser::_and () {
    do {
        if (!_not()) return false; 
    } while (shift().text == "&");
    unshift();
    return true;
}
bool parser::_not () {
    if (shift().text != "!") {
        unshift();
    }
    if (!relational()) return false;;

    return true;
}
bool parser::relational () {

    if (!sum()) return false ;

    return true;
}
bool parser::sum () {

    do {
        //cout << "BEGIN ADD" << endl; 
        if (!product()) return false;
        //cout << "END ADD" << endl; 

        if (shift().text == "+") continue;
        unshift();
        if (shift().text == "-") continue;
        unshift();

        break;
    } while (true);

    return true;
}
bool parser::product () {
    do {
        //cout << "BEGIN MUL" << endl; 
        if (!sign()) return false;
        //cout << "END MUL" << endl; 
        if (shift().text == "*") continue;
        unshift();
        if (shift().text == "/") continue;
        unshift();
        if (shift().text == "%") continue;
        unshift();

        break;

    } while (true);

    return true;
}
bool parser::sign () {
    //cout << "BEGIN NEG" << endl; 
    if (shift().text == "-") {
        if (!terminal()) {
            return false;
        }
    } else {
        unshift();
        if (!terminal()) return false;
    }
    //cout << "END NEG" << endl; 
    return true;
}
bool parser::terminal () {
    //cout << "BEGIN TERM" << endl; 
    if (shift().text == "(") {
        expression();
        if (shift().text != ")") {
            error(")");
            //cout << "ERROR: expected )" << endl; 
            return false;
        }
    } else {
        unshift();
        types t = shift().type;
        if (t != INTEGER && t != CHAR && t != STRING && t != FLOAT && t != ID) {
            error ("value or identifier");
            unshift();
            return false;
        }
        cerr << "found " << type_names[t] << endl;
    }
    //cout << "END TERM" << endl; 
    return true;
}

#endif // PARSER_H
