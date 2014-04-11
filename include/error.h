#ifndef ERROR_H
#define ERROR_H

#include "iostream"


class error {

protected:

    struct instance {
        string expected;
        token  recieved;
    };

    vector<instance> errors;
    vector<instance>::iterator it;

public:

    error() {};

    void report(token recieved, string expected);
    void print(ostream& out);

    friend void test_lexer ();
    friend void test_parser ();
    friend void test_semantic ();

};

void error::report(token recieved, string expected) {

    struct instance e;
    e.expected = expected;
    e.recieved = recieved;

    errors.push_back(e);

}

void error::print(ostream& out) {
    
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


#endif /* end of include guard: ERROR_H */
