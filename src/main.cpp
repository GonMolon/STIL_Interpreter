#include <iostream>
#include <STILBaseVisitor.h>

#include "interpreter/STILInterpreter.h"

using namespace parser;
using namespace antlr4;
using namespace std;

string get_file_name(string file) {
    int i = file.size() - 1;
    while(i >= 0 && file[i] != '.') {
        --i;
    }
    return file.substr(0, i);
}

int main(int num_args, char* args[]) {

    if(num_args != 2 && num_args != 3) {
        cerr << "Incorrect number of parameters." << endl;
        cout << "Usage: stil_converter input_file.stil [pattern_exec_name]" << endl;
        cout << "Output: $input_file.pat $input_file.atp" << endl;
        exit(1);
    }

    string path = string(args[1]);
    path = get_file_name(path);

    ifstream stil_input;
    ofstream vector_output;
    ofstream timing_output;

    stil_input.open(args[1]);
    vector_output.open(path + ".pat");
    timing_output.open(path + ".atp");


    STILInterpreter interpreter(stil_input);

    interpreter.run(vector_output, timing_output);

    if(num_args == 2) {
    } else {
        string pattern_exec(args[2]);
        interpreter.run(vector_output, timing_output, pattern_exec);
    }

    return 0;
}