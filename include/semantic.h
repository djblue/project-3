#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <vector>
#include <fstream>
#include <iostream>

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
    void report (int line, error_types type, string info);

    // cube for calculating results of operations
    cube c;

    string current_scope;
    ofstream fout; // for error logging.

    // current line in source code file (for error reposing purposes)
    int line;

public:

    string pop ();

    // type stack
    // to push onto stack use: type_stack.push_back();
    // to pop from stack use:  type_stack.pop_back();
    vector<string> type_stack;
    
    symboltable table;

    // constructor which initialized log file
    semantic ();
    semantic (string output);

    // 1 variable per scope
    void declaration_and_unicity ();
    // types of variables match to values assigned to them.
    void types ();
    // conditions evaluate to booleans
    void coditions (int line);
    // return value matches function definitions
    void _return ();
    // number and types match definition
    void paramaters ();


    // tyep checking
    void calculatTypeBinary (string op);
    void calculatTypeUnary (string op);

    // report any errors
    void print (ostream out);

    friend void test_semantic ();

};

semantic::semantic () {
    fout.open("output.txt");
}

semantic::semantic (string output) {
    fout.open(output.c_str());
}

void semantic::report (int line, error_types type, string info) {
    struct error e;
    e.line = line;
    e.type = type;
    e.info = info;
    errors.push_back(e);
}

void semantic::print (ostream out) {

    for (it = errors.begin(); it != errors.end(); it++) {
        switch ((*it).type) {

            case DUPLICATE_VARIABLE:
            case DUPLICATE_METHOD:
            case VARIABLE_NOT_FOUND:
            case METHOD_NOT_FOUND:
            case TYPE_MISMATCH:

            case BOOLEAN_EXPECTED:
                out << "Boolean expression expected on line "
                    << (*it).line << endl;
                break;

            case RETURN_MISMATCH:
            case INCORRECT_PARAMETERS:
            default:
                break;
        }
    }
}

void semantic::declaration_and_unicity  () {

    string name;

    /* body */

    if (true) {
        fout << "Duplicated variable " << name << " in line " << line << endl;
    } else if (true) {
        fout << "Duplicated method " << name << " in line " << line << endl;
    } else if (true) {
        fout << "Variable " << name << " not found" << endl;
    } else if (true) {
        fout << "Method " << name << " not found" << endl;
    }
}

void semantic::types () {

    /* body */

    if (true) {
        fout << "Type mismatch in line " << line << endl;
    } 
}

void semantic::coditions (int line) {
    string temp = pop();
    if (temp != "BOOL") {
        report(line, BOOLEAN_EXPECTED, "");
    }
}

void semantic::_return () {

    /* body */

    if (true) {
        fout << "Return type mismatch in line " << line << endl;
    }
}

void semantic::paramaters () {

    string method;

    /* body */

    if (true) {
        fout << "Type or number of parameters in method " 
             << method << " are incorrect in line  " << line << endl;
    }
}

string semantic::pop () {
    if (type_stack.size() > 0 ) {
        string temp = type_stack.back();
        type_stack.pop_back();
        return temp;
    }
    return "ERROR";
}

void semantic::calculatTypeUnary (string op) {
    type_stack.push_back(c.result(op, pop()));
}

void semantic::calculatTypeBinary (string op) {
    type_stack.push_back(c.result(op, pop(), pop()));
}


#endif /* end of include guard: SEMANTIC_H */
