#include <fstream>
#include <vector>
#include <iostream>

#include "../include/lexer.h"
#include "../include/parser.h"

using namespace std;

int main (int argc, char** argv) {

    // 0 - default
    int mode = 0;

    ifstream fin;
    ofstream fout;
    
    // parse command line arguments
    // -l lexer output (mode = 1)
    // -p parser output (mode = 2)
    if (argc == 4) {
        if (argv[1][1] == 'l') {
            mode = 1;
            fin.open(argv[2]);
            fout.open(argv[3]);
        } else if (argv[1][1] == 'p') {
            mode = 2;
            fin.open(argv[2]);
            fout.open(argv[3]);
        } else {
            cerr << "Invalid option '" <<  argv[1][1] << "'" << endl;
            return -2;
        }
    } else if (argc != 3) {
        cerr << "Usage: " << argv[0]
             << " [opts] input output" << endl;
        return -1;
    } else {
        fin.open(argv[1]);
        fout.open(argv[2]);
    }


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
    vector<lexer::token> tokens;

    // Read through the file line by line.
    while (getline(fin, temp)) {
        test = l.split(temp);
        for (i = 0; i < test.size(); i++) {
            lexer::token t = l.lex(test[i], line_number);
            tokens.push_back(l.lex(test[i], line_number));
            // show lexer output
            if (mode == 1) {
                fout << lexer::type_names[t.type]
                     << "\t" << t.text << endl;
            }
        }
        line_number++;
    }

    if (mode == 2) {
        parser p (tokens);
        if (p.parse()) {
            cout << "Build Successful" << endl;
        } else {
            cout << "Build Failed" << endl;
            p.print(fout);
        }
    }

    fin.close();
    fout.close();

    return 0;
}
