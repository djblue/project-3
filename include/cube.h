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
    bcube["+"]["INTEGER"]["INTEGER"]  = "INTEGER";
    bcube["+"]["FLOAT"]["INTEGER"]    = "FLOAT";
    bcube["+"]["INTEGER"]["FLOAT"]    = "FLOAT";
    bcube["+"]["FLOAT"]["FLOAT"]      = "FLOAT";

    bcube["+"]["STRING"]["STRING"]    = "STRING";

    bcube["+"]["STRING"]["INTEGER"]   = "STRING";
    bcube["+"]["STRING"]["FLOAT"]     = "STRING";
    bcube["+"]["STRING"]["CHARACTER"] = "STRING";
    bcube["+"]["STRING"]["BOOL"]      = "STRING";

    bcube["+"]["INTEGER"]["STRING"]   = "STRING";
    bcube["+"]["FLOAT"]["STRING"]     = "STRING";
    bcube["+"]["CHARACTER"]["STRING"] = "STRING";
    bcube["+"]["BOOL"]["STRING"]      = "STRING";

    // subtraction
    bcube["-"]["INTEGER"]["INTEGER"]  = "INTEGER";
    bcube["-"]["FLOAT"]["INTEGER"]    = "FLOAT";
    bcube["-"]["INTEGER"]["FLOAT"]    = "FLOAT";
    bcube["-"]["FLOAT"]["FLOAT"]      = "FLOAT";

    bcube["*"]["INTEGER"]["INTEGER"]  = "INTEGER";
    bcube["*"]["FLOAT"]["INTEGER"]    = "FLOAT";
    bcube["*"]["INTEGER"]["FLOAT"]    = "FLOAT";
    bcube["*"]["FLOAT"]["FLOAT"]      = "FLOAT";

    bcube["/"]["INTEGER"]["INTEGER"]  = "INTEGER";
    bcube["/"]["FLOAT"]["INTEGER"]    = "FLOAT";
    bcube["/"]["INTEGER"]["FLOAT"]    = "FLOAT";
    bcube["/"]["FLOAT"]["FLOAT"]      = "FLOAT";

    // logical operations
    bcube["|"]["BOOL"]["BOOL"]        = "BOOL";
    bcube["&"]["BOOL"]["BOOL"]        = "BOOL";

    bcube["<"]["INTEGER"]["INTEGER"]  = "BOOL";
    bcube["<"]["INTEGER"]["FLOAT"]    = "BOOL";
    bcube["<"]["FLOAT"]["INTEGER"]    = "BOOL";
    bcube["<"]["FLOAT"]["FLOAT"]      = "BOOL";

    bcube[">"]["INTEGER"]["INTEGER"]  = "BOOL";
    bcube[">"]["INTEGER"]["FLOAT"]    = "BOOL";
    bcube[">"]["FLOAT"]["INTEGER"]    = "BOOL";
    bcube[">"]["FLOAT"]["FLOAT"]      = "BOOL";

    // unary cube
    ucube["-"]["INTEGER"]             = "INTEGER";
    ucube["-"]["FLOAT"]               = "FLOAT";

    ucube["!"]["BOOL"]                = "BOOL";

    // Everything not listed will be an null/error!

}

string cube::result(string op, string type) {
    // sorry, you have an error
    if (ucube[op][type] == "") {
        return "ERROR";
    // yeah, we found it
    } else {
        return ucube[op][type];
    }
}

string cube::result(string op, string type1, string type2) {
    // sorry, you have an error
    if (bcube[op][type1][type2] == "") {
        return "ERROR";
    // yeah, we found it
    } else {
        return bcube[op][type1][type2];
    }
}

#endif /* end of include guard: CUBE_H */
