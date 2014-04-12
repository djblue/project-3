#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <map>
#include <string>
#include <vector>

using namespace std;

class symboltable {

private:

    // what defines a symbol
    struct symbol {
        string type;
        string scope;
        string value;
    };

    // data-structure for storing the symbol table 
    map<string, vector<symbol> > table;
    vector<symbol>::iterator it;

public:

    // default constructor (empty symbol table)
    symboltable();

    // insert a new symbol into the table (return false if already in
    // symbol table)
    bool insert(string name, string type, string scope);
    // search for a symbol, if found returns the chum bold, else return
    // empty/dummy symbol
    symbol search(string name);
    
};

#endif
