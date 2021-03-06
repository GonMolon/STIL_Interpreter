//
// Created by Gonzalo Solera on 03/09/2017.
//

#ifndef STIL_CONVERTER_TIMESET_H
#define STIL_CONVERTER_TIMESET_H

#include <vector>
#include "WaveSet.h"

using namespace std;

// This class represents a full Teradyne timeset: A WaveSet describing how the signal's waveform should be for each signal.
// The WaveSets will be reduced to an arbitrary chosen representative whenever the timeset generation finishes and there are multiple options
// in one signal's cell.

class TimeSet {

public:

	int id;
    float period;
    vector<pair<WaveSet, WaveSet>> wavesets;

    TimeSet() {}

    TimeSet(float period);

    void add_waveset(const WaveSet& waveset);

    bool merge(const TimeSet& timeset);

    void reduce();

    string to_string(vector<string> signal_names) const;

    bool operator==(const TimeSet& other) const;
};


#endif //STIL_CONVERTER_TIMESET_H
