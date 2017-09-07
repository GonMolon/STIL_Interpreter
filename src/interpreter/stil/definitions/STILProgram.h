//
// Created by Gonzalo Solera on 03/08/2017.
//

#ifndef STIL_INTERPRETER_STILPROGRAM_H
#define STIL_INTERPRETER_STILPROGRAM_H

#include <string>
#include <unordered_map>
#include <tree/ParseTree.h>
#include <STILParser.h>
#include "Signal.h"
#include "SignalGroup.h"
#include "PatternBurst.h"
#include "WaveFormTable.h"
#include "../../teradyne/STILConfig.h"

using namespace std;
using namespace antlr4;
using namespace tree;
using namespace parser;

#define GLOBAL_DEF "global"

typedef unordered_map<string, STILParser::Pattern_execContext*> PatternExecs;
typedef unordered_map<string, STILParser::Inst_listContext*> Patterns;
typedef unordered_map<string, STILParser::Inst_listContext*> Procedures;
typedef unordered_map<string, STILParser::Inst_listContext*> Macros;
typedef unordered_map<string, Procedures> ProcedureBlocks;
typedef unordered_map<string, Macros> MacroBlocks;

class STILProgram {

public:
    PatternExecs patternExecs;
    PatternBursts patternBursts;
    Patterns patterns;
    ProcedureBlocks procedures;
    MacroBlocks macros;
    Signals signals;
    SignalGroups signalGroups;
    WaveFormTables waveFormTables;

    STILConfig config;

    STILProgram(STILConfig config) : config(config) {}
};


#endif //STIL_INTERPRETER_STILPROGRAM_H
