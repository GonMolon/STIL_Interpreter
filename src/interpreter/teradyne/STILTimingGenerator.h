//
// Created by Gonzalo Solera on 03/09/2017.
//

#ifndef STIL_CONVERTER_STILTIMINGGENERATOR_H
#define STIL_CONVERTER_STILTIMINGGENERATOR_H

#include <vector>
#include <fstream>
#include "definitions/TimeSet.h"

using namespace std;

class STILTimingGenerator {

private:
    ofstream output;
    vector<TimeSet> timesets;
    unordered_map<TimeSet, int, TimeSet::Hasher> cache;

public:

    STILTimingGenerator() {}

    STILTimingGenerator(string timing_file);

    int add_timeset(const TimeSet& timeset);

    void finish();
};


#endif //STIL_CONVERTER_STILTIMINGGENERATOR_H
