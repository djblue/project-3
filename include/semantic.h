#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <vector>
#include <fstream>

#include "cube.h"
#include "symboltable.h"


using namespace std;

class semantic {

private: 
    
    // type stack
    // to push onto stack use: type_stack.push_back();
    // to pop from stack use:  type_stack.pop_back();
    vector<string> type_stack;

    // cube for calculating results of operations
    cube c;

    string current_scope;
    ofstream fout; // for error logging.

    // current line in source code file (for error reposing purposes)
    int line;

public:
    
    symboltable table;

    // constructor which initialized log file
    semantic ();
    semantic (string output);

    // 1 variable per scope
    void declaration_and_unicity ();
    // types of variables match to values assigned to them.
    void types ();
    // conditions evaluate to booleans
    void coditions ();
    // return value matches function definitions
    void _return ();
    // number and types match definition
    void paramaters ();


    // tyep checking
    void calculatTypeBinary (string type1, string type2, string op);
    void calculatTypeUnary (string type, string op);

    friend void test_semantic ();

};

semantic::semantic () {
    fout.open("output.txt");
}

semantic::semantic (string output) {
    fout.open(output.c_str());
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

void semantic::coditions () {


    /* body */

    if (true) {
        fout << "Boolean expression expected in line " << line << endl;
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

void semantic::calculatTypeUnary (string type, string op) {
    type_stack.push_back(c.result(op,type));
}

void semantic::calculatTypeBinary (string type1, string type2, string op) {
    type_stack.push_back(c.result(op,type1,type2));
}


#endif /* end of include guard: SEMANTIC_H */
