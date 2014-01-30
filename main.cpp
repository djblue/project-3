#include <fstream>
#include <vector>
#include <iostream>

#include "include/lexer.h"

using namespace std;

int main (int argc, char** argv) {

    if (argc != 3) {
        cerr << "Usage: " << argv[0] 
             << " inputfile outputfile" << endl;
        return -1;
    }

    ifstream fin(argv[1]);
    ofstream fout(argv[2]);

    if (!fin) {
        cerr << "File: " << argv[1] 
             << " could not be opened." << endl;
        return -2;
    }

    if (!fout) {
        cerr << "File: " << argv[2] 
             << " could not be opened." << endl;
        return -2;
    }

    lexer l;
    string temp;
    vector<string> test;
    vector<string>::size_type i;

    // Read through the file line by line.
    while (std::getline(fin, temp)) {
        //cout << temp; 
        test = l.split(temp);
        /*
        for (int i = 0; i < test.size() ; i++) {
            cout << i << " " << test[i] << endl;
        }*/
        for (i = 0; i < test.size(); i++) {
            fout << type_names[l.lex(test[i])]
                << '\t' << test[i] << endl;
        }
    }

    fin.close();
    fout.close();

    return 0;
}
