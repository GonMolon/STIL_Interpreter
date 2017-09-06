//
// Created by Gonzalo Solera on 07/08/2017.
//

#include "PatternContext.h"
#include "STILProgram.h"

PatternContext::PatternContext() {
    proceds_id = GLOBAL_DEF;
    macros_id = GLOBAL_DEF;
}

PatternContext::PatternContext(string proceds_id, string macros_id) {
    this->proceds_id = proceds_id;
    this->macros_id = macros_id;
}

PatternContext PatternContext::merge(PatternContext context) {
    PatternContext result = *this;
    if(context.proceds_id != GLOBAL_DEF) {
        result.proceds_id = context.proceds_id;
    }
    if(context.macros_id != GLOBAL_DEF) {
        result.macros_id = context.macros_id;
    }
    return result;
}

void ContextStack::push(const PatternContext& context) {
    if(!empty()) {
        stack::push(top().merge(context));
    } else {
        stack::push(context);
    }
}

void ContextStack::push(PatternContext&& context) {
    PatternContext aux = context;
    push(aux);
}
