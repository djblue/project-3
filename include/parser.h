#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

#include "lexer.h"
#include "semantic.h"

#define text(s) { \
    if (peek().text != s) { status = false; report(peek(), s); return false; } \
    else { shift(); /*cerr << "found " << s << endl; */}}

#define type(t) { \
    if (peek().type != t) { status = false; report(peek(), type_names[t]); return false; } \
    else if (peek().type == ID) { current_id = shift().text; } \
    else if (peek().type == KEYWORD) { \
        current_keyword = shift().text; } \
    else { shift(); /*cerr << "found " << type_names[t] << endl;*/ }}

using namespace std;

// This class implements a syntactical analyzer.
// It is a predictive-recursive-decent parser.
class parser {

private:

    // variables for semantic analysis
    string current_scope, current_id, current_keyword;

    struct param {
        string type;
        string name;
    };

    vector<param> params;
    vector<param>::iterator pit;

    int line_number;
    vector<token> tokens;
    vector<token>::size_type current_token;


    struct error {
        string expected;
        token  recieved;
    };

    vector<error> errors;
    vector<error>::iterator it;

    void report(token recieved, string expected);

    void error_recovery(string end);

    bool expect(string str, string recovery);
    bool expect(types t, string recover);

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

    semantic sm;

    parser () {};
    parser (vector<token> tokens);

    bool parse ();
    void print(ostream& out);

    friend void test_parser ();
    friend void test_semantic ();
};

bool parser::expect(string str, string recover) {
    if (peek().text != str) {
        report(peek(), str);
        error_recovery(recover);
        return false;
    } else {
        shift();
        return true;
    }
}

bool parser::expect(types t, string recover) {
    if (peek().type != t) {
        report(peek(), type_names[t]);
        error_recovery(recover);
        return false;
    } else {
        shift();
        return true;
    }
}

void parser::report(token recieved, string expected) {

    struct error e;
    e.expected = expected;
    e.recieved = recieved;

    errors.push_back(e);

}

void parser::print(ostream& out) {
    
    for (it = errors.begin(); it != errors.end(); it++) {
        out << "Line " 
            << (*it).recieved.line 
            << ": expected "
            << (*it).expected 
            << " received \'" 
            << (*it).recieved.text 
            << "\'"<< endl;
    }

}

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
    this->sm = semantic(&line_number);
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
        return t;
    }
}
void parser::unshift () {
    current_token--;
}
bool parser::parse () { return program(); }
bool parser::program () {

    if (tokens.size() == 0) {
        return true;
    }
    
    while (current_token < tokens.size()) {
        current_scope = "global"; // set default scope as global
        type(KEYWORD);
        type(ID);
        if (peek().text == "(") {
            function();
        } else {
            global();
        }
    }

    return errors.size() == 0;
}
bool parser::global () {
    while (shift().text == ",") {
        sm.insertGlobal(current_id, current_keyword);
        type(ID);
    }
    sm.insertGlobal(current_id, current_keyword);
    unshift();
    if (peek().text != ";")  {
        report(peek(), ";");
        error_recovery(";");
        return false;
    }
    shift();
    return true;
}
bool parser::function () {

    params.clear();
    string name = current_id;
    sm.table.functions.insert(name);
    string id = current_id + "_", return_type = current_keyword;

    bool status = true;
    text("(");
    if (!parameters()) {
        error_recovery(")");
        status = false;
    }
    text(")");


    for (pit = params.begin(); pit != params.end(); pit++) {
        id += (*pit).type;
        id += "_";
    }

    // insert funciton into symbol table after id is computed
    sm.insertFunction(id, name, return_type);

    // insert all of the parameters
    for (pit = params.begin(); pit != params.end(); pit++) {
        sm.insertLocal((*pit).name, (*pit).type, id);
    }

    current_scope = id;

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
            // insert param for future processing.
            struct param p; 
            type(KEYWORD);
            p.type = current_keyword;
            type(ID);
            p.name = current_id;
            params.push_back(p);
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
            unshift();
            unshift();
            if (assign()) {
                return true;
            }
        } else {
            unshift();
            unshift();
            if (call()) {
                sm.pop();
                text(";");
                return true;
            }
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
        sm.insertLocal(current_id, current_keyword, current_scope);
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
    type(ID);
    string id = current_id;
    text("=");
    if(!expression()) return false;
    sm.types(id, current_scope);
    text(";");
    return true;
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

    sm.conditions();

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

    sm.conditions();

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
    sm._return(current_scope);
    text(";");   
    return true;
}
bool parser::call() {

    params.clear();

    type(ID);
    string name = current_id, id = current_id + "_";
    text ("(");

    // not an empty function call
    if (peek().text != ")")  {
        do {
            if (!expression()) {
                error_recovery(")");
                shift();
                return false;
            }
            struct param p; 
            p.type = sm.pop();
            params.push_back(p);
        } while (shift().text == ",");
        unshift();
    }

    for (pit = params.begin(); pit != params.end(); pit++) {
        id += (*pit).type;
        id += "_";
    }

    sm.pushFunction(id,name);

    text(")");
    return true;
}
bool parser::expression () {
    bool once = false;
    do {
        if (once) sm.calculatTypeBinary("|");
        if (!_and()) return false; 
        once = true;
    } while (shift().text == "|");
    unshift();
    return true;
}
bool parser::_and () {
    bool once = false;
    do {
        if (once) sm.calculatTypeBinary("&");
        if (!_not()) return false; 
        once = true; 
    } while (shift().text == "&");
    unshift();
    return true;
}
bool parser::_not () {
    bool status = false;
    if (shift().text != "!") {
        unshift();
    } else {
        status = true;
    }
    if (!relational()) return false;
    if (status) sm.calculatTypeUnary("!");
    return true;
}
bool parser::relational () {

    string op = "";

    do {

        if (!sum()) return false;

        if (op != "") {
            sm.calculatTypeBinary(op);
        }

        if (shift().text == "<") {
            op = "<";
            continue;
        }
        unshift();
        if (shift().text == ">") {
            op = ">";
            continue;
        }
        unshift();

        break;

    } while (true);

    return true;
}
bool parser::sum () {

    string op = "";

    do {

        if (!product()) return false;

        if (op != "") {
            sm.calculatTypeBinary(op);
        }

        if (shift().text == "+") {
            op = "+";
            continue;
        }
        unshift();
        if (shift().text == "-") {
            op = "-";
            continue;
        }
        unshift();

        break;

    } while (true);

    return true;
}
bool parser::product () {
    string op = "";
    do {

        if (!sign()) return false;

        if (op != "") {
            sm.calculatTypeBinary(op);
        }

        if (shift().text == "*") {
            op = "*";
            continue;
        }
        unshift();
        if (shift().text == "/") {
            op = "/";
            continue;
        }
        unshift();
        if (shift().text == "%") {
            op = "%";
            continue;
        }
        unshift();

        break;

    } while (true);

    return true;
}
bool parser::sign () {
    if (shift().text == "-") {
        if (!terminal()) {
            return false;
        } else {
            sm.calculatTypeUnary("-");
        }
    } else {
        unshift();
        if (!terminal()) return false;
    }
    return true;
}
bool parser::terminal () {

    token t = shift();

    // for nested function calls
    if (t.type == ID) {
        if (peek().text == "(") {
            unshift(); 
            return call();
        }
    }

    // for nested parens
    if (t.text == "(") {
        if (!expression()) return false;
        text(")");
    } else {
        unshift();
        token t = peek();

        if (t.type < INTEGER || t.type > BOOL) { 
            report (t, string("value or identifier"));
            return false;
        } else if (t.type == ID) {
            sm.push(t.text, current_scope);
            shift();
        } else {
            sm.type_stack.push_back(type_names[t.type]);
            shift();
        }
    }
    return true;
}

#endif // PARSER_H
