//
// Created by Gonzalo Solera on 04/08/2017.
//

#include "WaveFormTable.h"

WaveFormTable::WaveFormTable(string id, float period, WaveForms& waveforms) : Identifiable(id) {
    this->period = period;
    this->waveforms = waveforms;
}

string WaveFormTable::get_event_seq(string signal_id, char wfc, SignalGroups& signalGroups) {
    bool found = false;
    int waveform = 0;
    while(waveform < waveforms.size() && !found) {
        if(signalGroups[waveforms[waveform].id].contains(signal_id) && wfc == waveforms[waveform].wfc) {
            found = true;
        } else {
            ++waveform;
        }
    }
    return waveforms[waveform].event_seq();
}
