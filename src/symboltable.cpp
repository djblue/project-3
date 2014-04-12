#include "../include/symboltable.h"

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
