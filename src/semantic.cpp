#include "../include/semantic.h"

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
