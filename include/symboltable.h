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

symboltable::symboltable() {}


// try to insert symbol into table
bool symboltable::insert(string name, string type, string scope) {

    // the new symbole to consider
    symbol s;
    s.type = type;
    s.scope = scope;

    // couldn't find and existing symbol within table 
    if (table.find(name) == table.end()) {
        table[name].push_back(s); // insert into symbol table
        return true;

    // use iterator to search through symbols
    } else {
        for (it = table[name].begin(); it != table[name].end(); it++) {
            if ((*it).scope == scope) {
                // can't have two variables with same name in the same scope
                return false;
            } 
        }
    }

    // we made it through the for-loop / no matching variable in same
    // scope, insert symbol and return success.
    table[name].push_back(s);
    return true;
}

#endif
