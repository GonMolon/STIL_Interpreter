//
// Created by Gonzalo Solera on 03/08/2017.
//

#include "STILProgramVisitor.h"
#include "definitions/WaveFormTable.h"

antlrcpp::Any STILProgramVisitor::visitProgram(STILParser::ProgramContext* ctx) {
    visit(ctx->signals());
    visit(ctx->signal_groups());
    visit(ctx->timing());
    visit(ctx->pattern_burst_l());
    visit(ctx->pattern_exec_l());
    visit(ctx->pattern_burst_l());
    visit(ctx->pattern_l());
    visit(ctx->procedures_l());
    visit(ctx->macros_l());
    return NULL;
}

antlrcpp::Any STILProgramVisitor::visitSignals(STILParser::SignalsContext* ctx) {
    for(int i = 0; i < ctx->signal().size(); ++i) {
        visit(ctx->signal(i));
    }
    return NULL;
}

antlrcpp::Any STILProgramVisitor::visitSignal(STILParser::SignalContext* ctx) {
    string id = visit(ctx->id());
    signal_dir dir = visit(ctx->signal_dir());
    signal_scan_dir scan_dir = NONE;
    if(ctx->signal_attributes() != NULL && ctx->signal_attributes()->signal_scan() != NULL) {
        signal_scan_dir aux = visit(ctx->signal_attributes()->signal_scan());
        scan_dir = aux;
    }
    program.signals[id] = Signal(id, dir, scan_dir);
    program.signalGroups[id] = SignalGroup(program.signals[id]); // Adding a default signalgroup
    return NULL;
}

antlrcpp::Any STILProgramVisitor::visitSignal_dir(STILParser::Signal_dirContext* ctx) {
    if(ctx->getText() == "In") {
        return IN;
    } else if(ctx->getText() == "Out") {
        return OUT;
    } else {
        return INOUT;
    }
}

antlrcpp::Any STILProgramVisitor::visitSignal_scan(STILParser::Signal_scanContext* ctx) {
    if(ctx->getText() == "ScanIn") {
        return SCAN_IN;
    } else if(ctx->getText() == "ScanOut") {
        return SCAN_OUT;
    }
}

antlrcpp::Any STILProgramVisitor::visitSignal_groups(STILParser::Signal_groupsContext* ctx) {
    for(int i = 0; i < ctx->signal_group().size(); ++i) {
        visit(ctx->signal_group(i));
    }
    return NULL;
}

antlrcpp::Any STILProgramVisitor::visitSignal_group(STILParser::Signal_groupContext* ctx) {
    string id = visit(ctx->id());
    vector<string> signals = visit(ctx->signal_list());
    SignalGroup::WFCMaps wfcmaps;
    if(ctx->signal_attributes() != NULL && ctx->signal_attributes()->wfc_map() != NULL) {
        SignalGroup::WFCMaps aux = visit(ctx->signal_attributes()->wfc_map());
        wfcmaps = aux;
    }
    program.signalGroups[id] = SignalGroup(id, signals, wfcmaps);
    return NULL;
}

antlrcpp::Any STILProgramVisitor::visitSignal_list(STILParser::Signal_listContext* ctx) {
    vector<string> signal_list;
    for(int i = 0; i < ctx->id().size(); ++i) {
        string id = visit(ctx->id(i));
        signal_list.push_back(id);
    }
    return signal_list;
}

antlrcpp::Any STILProgramVisitor::visitWfc_map(STILParser::Wfc_mapContext* ctx) {
    SignalGroup::WFCMaps wfcmaps;
    for(int i = 0; i < ctx->map_rule().size(); ++i) {
        MapRule map_rule = visit(ctx->map_rule(i));
        wfcmaps[map_rule.first] = map_rule.second;
        if(map_rule.first.size() == 2) {
            // If the WFC are assigned in the inverted order
            char aux = map_rule.first[0];
            map_rule.first[0] = map_rule.first[1];
            map_rule.first[1] = aux;
            wfcmaps[map_rule.first] = map_rule.second;
        }
    }
    return wfcmaps;
}

antlrcpp::Any STILProgramVisitor::visitMap_rule(STILParser::Map_ruleContext* ctx) {
    string from = visit(ctx->wfc_seq());
    string to = visit(ctx->wfc());
    assert(from.size() == 2 && from[0] != from[1]);
    assert(to.size() == 1);
    return MapRule(from, to[0]);
}

antlrcpp::Any STILProgramVisitor::visitTiming(STILParser::TimingContext* ctx) {
    for(int i = 0; i < ctx->waveform_table().size(); ++i) {
        visit(ctx->waveform_table(i));
    }
    return NULL;
}

antlrcpp::Any STILProgramVisitor::visitWaveform_table(STILParser::Waveform_tableContext* ctx) {
    string id = visit(ctx->id());
    float period = visit(ctx->period());
    WaveForms waveforms = visit(ctx->waveforms());
    program.waveFormTables[id] = WaveFormTable(id, period, waveforms);
    return NULL;
}

antlrcpp::Any STILProgramVisitor::visitWaveforms(STILParser::WaveformsContext* ctx) {
    WaveForms waveforms;
    for(int i = 0; i < ctx->waveform().size(); ++i) {
        WaveForm waveform = visit(ctx->waveform(i));
        waveforms.push_back(waveform);
    }
    return waveforms;
}

antlrcpp::Any STILProgramVisitor::visitWaveform(STILParser::WaveformContext* ctx) {
    string id = visit(ctx->id());
    string wfc = visit(ctx->wfc());
    vector<WaveForm::WaveFormEvent> events;
    for(int i = 0; i < ctx->event().size(); ++i) {
        WaveForm::WaveFormEvent event = visit(ctx->event(i));
        events.push_back(event);
    }
    return WaveForm(id, wfc[0], events);
}

antlrcpp::Any STILProgramVisitor::visitEvent(STILParser::EventContext* ctx) {
    float time = visit(ctx->time_expr());
    char event_code = visit(ctx->event_code());
    return WaveForm::WaveFormEvent(time, event_code);
}

antlrcpp::Any STILProgramVisitor::visitEvent_code(STILParser::Event_codeContext* ctx) {
    string event_code = visit(ctx->CHARS());
    return event_code[0];
}

antlrcpp::Any STILProgramVisitor::visitPattern_exec(STILParser::Pattern_execContext* ctx) {
    string id = GLOBAL_DEF;
    if(ctx->id() != NULL) {
        string aux = visit(ctx->id());
        id = aux;
        // If there's only one pattern_exec, we create a global alias
        if(((STILParser::Pattern_exec_lContext*) ctx->parent)->pattern_exec().size() == 1) {
            program.patternExecs[GLOBAL_DEF] = ctx;
        }
    }
    program.patternExecs[id] = ctx;
    return NULL;
}

antlrcpp::Any STILProgramVisitor::visitPattern_burst(STILParser::Pattern_burstContext* ctx) {
    string id = visit(ctx->id());
    PatternContext context = visit(ctx->context());
    program.patternBursts[id] = PatternBurst(id, ctx->pattern_list(), context);
    return NULL;
}

antlrcpp::Any STILProgramVisitor::visitPattern(STILParser::PatternContext* ctx) {
    string id = visit(ctx->id());
    program.patterns[id] = ctx->inst_list();
    return NULL;
}

antlrcpp::Any STILProgramVisitor::visitProcedures(STILParser::ProceduresContext* ctx) {
    string id = GLOBAL_DEF;
    if(ctx->id() != NULL) {
        string aux = visit(ctx->id());
        id = aux;
    }
    for(int i = 0; i < ctx->procedure().size(); ++i) {
        string proced_id = visit(ctx->procedure(i)->id());
        program.procedures[id][proced_id] = ctx->procedure(i)->inst_list();
    }
    return NULL;
}

antlrcpp::Any STILProgramVisitor::visitMacros(STILParser::MacrosContext* ctx) {
    string id = GLOBAL_DEF;
    if(ctx->id() != NULL) {
        string aux = visit(ctx->id());
        id = aux;
    }
    for(int i = 0; i < ctx->macro().size(); ++i) {
        string macro_id = visit(ctx->macro(i)->id());
        program.macros[id][macro_id] = ctx->macro(i)->inst_list();
    }
    return NULL;
}