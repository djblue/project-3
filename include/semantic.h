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

#endif /* end of include guard: SEMANTIC_H */
