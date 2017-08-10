//
// Created by Gonzalo Solera on 04/08/2017.
//

#include "STILCustomVisitor.h"
#include "program/definitions/PatternContext.h"
#include "program/STILProgram.h"
#include "STILState.h"

antlrcpp::Any STILCustomVisitor::visitTerminal(tree::TerminalNode* node) {
    return node->getText();
}

antlrcpp::Any STILCustomVisitor::visitFloat_t(STILParser::Float_tContext* ctx) {
    string num = visit(ctx->FLOAT());
    float value = stof(num);
    return value;
}

antlrcpp::Any STILCustomVisitor::visitInt_t(STILParser::Int_tContext* ctx) {
    string num = visit(ctx->INT());
    int value = stoi(num);
    return value;
}

antlrcpp::Any STILCustomVisitor::visitNum(STILParser::NumContext* ctx) {
    if(ctx->int_t() != NULL) {
        int num = visit(ctx->int_t());
        return (float) num;
    } else {
        float num = visit(ctx->float_t());
        return num;
    }
}

antlrcpp::Any STILCustomVisitor::visitPeriod(STILParser::PeriodContext* ctx) {
    return visit(ctx->time_expr());
}

antlrcpp::Any STILCustomVisitor::visitTime_expr(STILParser::Time_exprContext* ctx) {
    float time = visit(ctx->num());
    string units = visit(ctx->TIME_UNIT());
    if(units == "ns") {
        return time;
    } else if(units == "ms") {
        return time * 1000;
    } else {
        return time * 1000000;
    }
}

antlrcpp::Any STILCustomVisitor::visitWfc(STILParser::WfcContext* ctx) {
    return ctx->getText();
}

antlrcpp::Any STILCustomVisitor::visitWfc_extended(STILParser::Wfc_extendedContext* ctx) {
    return ctx->getText();
}

antlrcpp::Any STILCustomVisitor::visitContext(STILParser::ContextContext* ctx) {
    string proceds_id = GLOBAL_DEF;
    string macros_id = GLOBAL_DEF;
    if(ctx->proced_context() != NULL) {
        string aux = visit(ctx->proced_context()->id());
        proceds_id = aux;
    }
    if(ctx->macro_context() != NULL) {
        string aux = visit(ctx->macro_context()->id());
        macros_id = aux;
    }
    return PatternContext(proceds_id, macros_id);
}

antlrcpp::Any STILCustomVisitor::visitAssigs(STILParser::AssigsContext* ctx) {
    list<STILState::Assig> assigs;
    for(int i = 0; i < ctx->assig().size(); ++i) {
        string signal_group_id = visit(ctx->assig(i)->id());
        string value = visit(ctx->assig(i)->assig_expr());
        assigs.push_back(STILState::Assig(signal_group_id, value));
    }
    return assigs;
}

antlrcpp::Any STILCustomVisitor::visitAssig_expr(STILParser::Assig_exprContext* ctx) {
    string value;
    for(int i = 0; i < ctx->children.size(); ++i) {
        if(i != 0 || ctx->JOIN() == NULL) { // Ignoring the \j token
            string seq = visit(ctx->children[i]);
            value += seq;
        }
    }
    return value;
}

antlrcpp::Any STILCustomVisitor::visitRepeat(STILParser::RepeatContext* ctx) {
    int times = visit(ctx->int_t());
    string wfc = visit(ctx->wfc_extended());
    string result;
    while(times-- > 0) {
        result += wfc;
    }
    return result;
}
