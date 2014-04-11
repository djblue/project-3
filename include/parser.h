#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "error.h"
#include "lexer.h"
#include "semantic.h"

#define text(s) { \
    if (peek().text != s) { status = false; e.report(peek(), s); return false; } \
    else { shift(); /*cerr << "found " << s << endl; */}}

#define type(t) { \
    if (peek().type != t) { status = false; e.report(peek(), type_names[t]); return false; } \
    else if (peek().type == ID) { current_id = shift().text; } \
    else if (peek().type == KEYWORD) { current_keyword = shift().text; } \
    else { shift(); /*cerr << "found " << type_names[t] << endl;*/ }}

using namespace std;

// This class implements a syntactical analyzer.
// It is a predictive-recursive-decent parser.
class parser {

private:
    semantic sm;
    string current_scope, current_id, current_keyword;
    int line_number;
    vector<token> tokens;
    vector<token>::size_type current_token;

    error e;
    void error_recovery(string end);

    token shift ();
    token peek ();
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
    parser (vector<token> tokens);

    bool parse ();
    string error_report();

    friend void test_parser ();
};

void parser::error_recovery(string end) {
    int line = peek().line;
    while (tokens.size() > current_token) {
        if (peek().text == end){
            break;
        } else if (line < peek().line) {
            break; 
        } else {
            shift();
        }
    }
}

parser::parser (vector<token> tokens) {
    status = true;
    current_token = 0;
    line_number = 0;
    this->tokens = tokens;
}
token parser::shift () {
    token t = peek();
    current_token++;
    return t;
}
token parser::peek () {
    if (current_token < tokens.size()) {
        token t = tokens[current_token];
        line_number = t.line;
        return t;
    } else {
        token t;
        t.line = line_number;
        t.type = UNDEFINED;
        t.text = "";
        //cout << "(" << tokens.size() << ","<< current_token << ")" << endl;
        return t;
    }
}
void parser::unshift () {
    current_token--;
}
bool parser::parse () {
    program();
    return status;
}
bool parser::program () {

    bool status = true;
    current_scope = "global"; // set default scope as global

    if (tokens.size() == 0) {
        return true;
    }
    
    while (current_token < tokens.size()) {
        type(KEYWORD);
        type(ID);
        if (peek().text == "(") {
            if (!function()) {
                status = false;
            }
        } else {
            if (!global()) {
                status = false;
            }
        }
    }

    return status;
}
bool parser::global () {
    while (shift().text == ",") {
        sm.table.insert(current_id, current_keyword, current_scope);
        type(ID);
    }
    sm.table.insert(current_id, current_keyword, current_scope);
    unshift();
    if (peek().text != ";")  {
        error_recovery(";");
        return false;
    }
    shift();
    return true;
}
bool parser::function () {
    bool status = true;
    text("(");
    if (!parameters()) {
        error_recovery(")");
        status = false;
    }
    text(")");
    status = status && block();
    return status;
}
bool parser::block () {
    text("{");
    while(peek().text != "}") {
        if (peek().text == "") break;
        if (!line()) {
            return false;
        }
    }
    text("}");
    return true;
}
bool parser::parameters () {
    do {
        if (peek().text != ")") {
            type(KEYWORD);
            type(ID);
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

    else if (next.text == "while") {
        unshift();
        if (_while()) return true;
    }

    else if (next.text == "return") {
        unshift();
        if (_return()) return true;
    }

    else if (next.type == KEYWORD) {
        unshift();
        if (local()) return true;
    }

    else if (next.type == ID) {
        token t = shift();
        
        if (t.text == "=") {
            if (expression() && shift().text == ";") return true;
        } else {
            unshift();
            unshift();
            if (call()) return true;
        }
    }

    unshift();

    error_recovery(";");

    return false;
}
bool parser::local () {
    type(KEYWORD);
    do {
        type(ID);
        if (peek().text == "=") {
            shift();
            if(!expression()) return false;
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
    
    bool status = true;
    text("if");

    text("(");
    if (!expression()) {
        error_recovery(")");
        status = false;
    }
    text(")");
    if (peek().text == "{") {
        if (!block()) return false;
    } else {
        line();
    }

    if (peek().text == "else") {
        text("else");
        if (peek().text == "{") {
            if (!block()) return false;
        } else {
            line();
        }
    }

    return status;
}
bool parser::_while () {

    bool status = true;;

    text("while");

    text("(");
    if (!expression()) {
        error_recovery(")");
        status = false;
    }
    text(")");

    if (peek().text == ";") {
        return true;
    } else if (peek().text == "{") {
        if (!block()) return false;
    } else {
        if (!line()) return false;
    }

    return status;
}
bool parser::_return () {
    text("return");
    if (!expression()) return false;
    text(";");   
    return true;
}
bool parser::call() {
    bool status = true;
    type(ID);
    text ("(");

    do {
        if (!expression()) {
            error_recovery(")");
            shift();
            return false;
        }
    } while (shift().text == ",");
    unshift();

    text(")");
    text(";");
    return status;
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

    do {
        if (!sum()) return false;

        if (shift().text == "<") continue;
        unshift();
        if (shift().text == ">") continue;
        unshift();

        break;
    } while (true);

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
        if (!expression()) return false;
        text(")");
    } else {
        unshift();
        token t = peek();
        if (t.type != INTEGER && t.type != HEXADECIMAL && t.type != OCTAL
        && t.type != CHAR &&
        t.type != STRING && t.type != FLOAT && t.type != ID) {
            e.report (t, string("value or identifier"));
            return false;
        } else {
            shift();
        }
        //cerr << "found " << type_names[t] << endl;
    }
    //cout << "END TERM" << endl; 
    return true;
}

#endif // PARSER_H
