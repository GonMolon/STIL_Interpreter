//
// Created by Gonzalo Solera on 08/08/2017.
//

#ifndef STIL_INTERPRETER_SIGNALSTATE_H
#define STIL_INTERPRETER_SIGNALSTATE_H

#include <string>
#include <vector>
#include "program/STILProgram.h"

using namespace std;

class SignalState {

private:

    STILProgram* program;

public:

    typedef pair<string, string> Assig;

    Signals next_vector;
    string waveform_table;

    SignalState() {}

    SignalState(STILProgram* program);

    void execute_assigs(list<Assig> assigs);

    void clock_cycle(ostream& output);
};


#endif //STIL_INTERPRETER_SIGNALSTATE_H
