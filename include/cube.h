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

#endif /* end of include guard: CUBE_H */
