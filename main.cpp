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

    int line_number = 1;
    lexer l;
    string temp;
    vector<string> test;
    vector<string>::size_type i;
    vector<token> tokens;

    // Read through the file line by line.
    while (std::getline(fin, temp)) {
        test = l.split(temp);
        for (i = 0; i < test.size(); i++) {
            token t = l.lex(test[i], line_number);
            tokens.push_back(l.lex(test[i], line_number));
            fout << type_names[t.type]
                 << "\t" << t.text << endl;
        }
        line_number++;
    }

    fin.close();
    fout.close();

    return 0;
}
