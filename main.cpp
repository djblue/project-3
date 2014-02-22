#include <fstream>
#include <vector>
#include <iostream>

#include "include/lexer.h"
#include "include/parser.h"

using namespace std;

int main (int argc, char** argv) {

    int output = 0;
    
    // parse command line arguments
    // -p parser output
    // -l lexer output
    if (argc == 4) {
        if (argv[1][1] == 'l') {
            output = 1;
        }
    } else if (argc != 3) {
        cerr << "Usage: " << argv[0]
             << " [opts] input output" << endl;
        return -1;
    }

    ifstream fin(argv[1+output]);
    ofstream fout(argv[2+output]);

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
            // show lexer output
            if (output == 1) {
                fout << type_names[t.type]
                     << "\t" << t.text << endl;
            }
        }
        line_number++;
    }

    fin.close();
    fout.close();

    return 0;
}
