#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

#include "cube.h"
#include "symboltable.h"

using namespace std;

class semantic {

private: 

    // types of errors available for testing
    enum error_types {
        DUPLICATE_VARIABLE,
        DUPLICATE_METHOD,
        VARIABLE_NOT_FOUND,
        METHOD_NOT_FOUND,
        TYPE_MISMATCH,
        BOOLEAN_EXPECTED,
        RETURN_MISMATCH,
        INCORRECT_PARAMETERS
    };

    // error struct to inspect for testing/ printing
    struct error {
        int line;
        error_types type;
        string info;
    };

    vector<error> errors;
    vector<error>::iterator it;
    void report (error_types type, string info);

    // cube for calculating results of operations
    cube c;

    string current_scope;

    // current line in source code file (for error reposing purposes)
    int *line;

public:

    string pop ();

    // type stack
    // to push onto stack use: type_stack.push_back();
    // to pop from stack use:  type_stack.pop_back();
    vector<string> type_stack;
    
    symboltable table;

    // constructor which initialized log file
    semantic ();
    semantic (int *line);

    // 1 variable per scope
    void declaration_and_unicity ();
    // types of variables match to values assigned to them.
    void types (string id, string scope);
    // conditions evaluate to booleans
    void conditions ();
    // return value matches function definitions
    void _return (string scope);
    // number and types match definition
    void paramaters ();

    void insertFunction (string id, string name, string type);
    void insertGlobal (string name, string type);
    void insertLocal (string name, string type, string scope);

    void push (string id, string scope);
    void pushFunction (string id, string name);

    // tyep checking
    void calculatTypeBinary (string op);
    void calculatTypeUnary (string op);

    // report any errors
    void print (ostream& out);

    friend void test_semantic ();

};

semantic::semantic () {
}

semantic::semantic (int* line) {
    this->line = line;
}

void semantic::insertFunction (string id, string name, string type) {
    if (!table.insert(id,type,"function")) {
        report(DUPLICATE_METHOD, name); 
    }
}

void semantic::insertGlobal (string name, string type) {
    if (!table.insert(name,type,"global")) {
        report(DUPLICATE_VARIABLE, name); 
    }
}

void semantic::insertLocal (string name, string type, string scope) {
    if (!table.insert(name,type,scope)) {
        report(DUPLICATE_VARIABLE, name); 
    }
}

void semantic::pushFunction (string id, string name) {
    string type = table.search(id, "function").type;
    if (type != "") {
        type_stack.push_back(type);
    } else if (table.functions.find(name) != table.functions.end()) {
        report(INCORRECT_PARAMETERS, name); 
        type_stack.push_back("error");
    } else {
        report(METHOD_NOT_FOUND, name); 
        type_stack.push_back("error");
    }
}

void semantic::push (string id, string scope) {
    string type = table.search(id, scope).type;
    if (type == "") {
        report(VARIABLE_NOT_FOUND, id); 
        type_stack.push_back("error");
    } else {
        type_stack.push_back(type);
    }
}


void semantic::types (string id, string scope) {
    // search for variable type
    string type = table.search(id, scope).type;

    if (type == "") {
        report(VARIABLE_NOT_FOUND, id); 
        pop();
    } else if (type != pop()) {
        report(TYPE_MISMATCH, ""); 
    }
}

void semantic::conditions () {
    if (pop() != "bool") {
        report(BOOLEAN_EXPECTED, "");
    }
}

void semantic::_return (string scope) {
    string type = table.search(scope, "function").type;
    if (type != pop()) {
        report(RETURN_MISMATCH, "");
    }
}

string semantic::pop () {
    if (type_stack.size() > 0 ) {
        string temp = type_stack.back();
        type_stack.pop_back();
        return temp;
    }
    return "error";
}

void semantic::calculatTypeUnary (string op) {
    type_stack.push_back(c.result(op, pop()));
}

void semantic::calculatTypeBinary (string op) {
    /*
    string type1 = pop(); 
    string type2 = pop();
    string result = c.result(op, type1, type2);
    cerr << type1 << "  " << op << "  " << type2 
         << " = " << result << endl;
    type_stack.push_back(result);
    */
    type_stack.push_back(c.result(op, pop(), pop()));
}

void semantic::print (ostream& out) {

    int line;
    string info;

    for (it = errors.begin(); it != errors.end(); it++) {

        line = (*it).line;
        info = (*it).info;

        switch ((*it).type) {

            case DUPLICATE_VARIABLE:
                out << "Duplicated variable " << info << " on line " << line;
                    break;

            case DUPLICATE_METHOD:
                out << "Duplicated method " << info << " on line " << line;
                    break;

            case VARIABLE_NOT_FOUND:
                out << "Variable " << info << " not found on line " << line;
                    break;

            case METHOD_NOT_FOUND:
                out << "Method " << info << " not found on line " << line;
                    break;

            case TYPE_MISMATCH:
                out << "Type mismatch on line " << line;
                    break;

            case BOOLEAN_EXPECTED:
                out << "Boolean expression expected on line " << line;
                    break;

            case RETURN_MISMATCH:
                out << "Return type mismatch on line " << line;
                    break;

            case INCORRECT_PARAMETERS:
                out << "Type or number of parameters in method "
                    << info << " are incorrect on line " << line;
                    break;
            default:
                    break;
        }

        out << endl;
    }
}

void semantic::report (error_types type, string info) {
    struct error e;
    e.line = *line;
    e.type = type;
    e.info = info;
    errors.push_back(e);
}

#endif /* end of include guard: SEMANTIC_H */
