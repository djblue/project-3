#ifndef CUBE_H
#define CUBE_H

#include <map>
#include <string>

using namespace std;


class cube {

private:

    // cube that describes the results of any operator with any two types.
    map<string, map<string, map<string, string> > > bcube; // binary cube
    map<string, map<string, string> > ucube; // unary cube

    // binary cube
    //  ["op"]["type1"]["type2"]   = "resulttype"

public:

    cube();

    string result(string op, string type);
    string result(string op, string type1, string type2);

};

cube::cube () {

    // addition
    bcube["+"]["int"]["int"]        = "int";
    bcube["+"]["float"]["int"]      = "float";
    bcube["+"]["int"]["float"]      = "float";
    bcube["+"]["float"]["float"]    = "float";

    bcube["+"]["string"]["string"]  = "string";

    bcube["+"]["string"]["int"]     = "string";
    bcube["+"]["string"]["float"]   = "string";
    bcube["+"]["string"]["char"]    = "string";
    bcube["+"]["string"]["bool"]    = "string";

    bcube["+"]["int"]["string"]     = "string";
    bcube["+"]["float"]["string"]   = "string";
    bcube["+"]["char"]["string"]    = "string";
    bcube["+"]["bool"]["string"]    = "string";

    // subtraction
    bcube["-"]["int"]["int"]      = "int";
    bcube["-"]["float"]["int"]    = "float";
    bcube["-"]["int"]["float"]    = "float";
    bcube["-"]["float"]["float"]  = "float";

    bcube["*"]["int"]["int"]      = "int";
    bcube["*"]["float"]["int"]    = "float";
    bcube["*"]["int"]["float"]    = "float";
    bcube["*"]["float"]["float"]  = "float";

    bcube["/"]["int"]["int"]      = "int";
    bcube["/"]["float"]["int"]    = "float";
    bcube["/"]["int"]["float"]    = "float";
    bcube["/"]["float"]["float"]  = "float";

    // logical operations
    bcube["|"]["bool"]["bool"]      = "bool";
    bcube["&"]["bool"]["bool"]      = "bool";

    bcube["<"]["int"]["int"]       = "bool";
    bcube["<"]["int"]["float"]     = "bool";
    bcube["<"]["float"]["int"]     = "bool";
    bcube["<"]["float"]["float"]   = "bool";

    bcube[">"]["int"]["int"]       = "bool";
    bcube[">"]["int"]["float"]     = "bool";
    bcube[">"]["float"]["int"]     = "bool";
    bcube[">"]["float"]["float"]   = "bool";

    // unary cube
    ucube["-"]["int"]               = "int";
    ucube["-"]["float"]             = "float";

    ucube["!"]["bool"]              = "bool";

    // Everything not listed will be an null/error!

}

string cube::result(string op, string type) {
    // sorry, you have an error
    if ( ucube.find(op)->second.find(type)
        == ucube.find(op)->second.end() ) {
        return "error";
    // yeah, we found it
    } else {
        return ucube[op][type];
    }
}

string cube::result(string op, string type1, string type2) {
    // sorry, you have an error
    if ( bcube.find(op)->second.find(type1)->second.find(type2)
        == bcube.find(op)->second.find(type1)->second.end() ) {
        return "error";
    // yeah, we found it
    } else {
        return bcube[op][type1][type2];
    }
}

#endif /* end of include guard: CUBE_H */
