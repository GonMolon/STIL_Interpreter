//
// Created by Gonzalo Solera on 04/08/2017.
//

#include "STILCustomVisitor.h"

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
    float value = stoi(num);
    return value;
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
