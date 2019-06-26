#ifndef DYNAMISMTRACER_ASSIGNMENT_STATE_STACK_H
#define DYNAMISMTRACER_ASSIGNMENT_STATE_STACK_H

#include "AssignmentState.h"

class AssignmentStateStack {
  public:
    explicit AssignmentStateStack(): assignement_state_stack_() {
    }

    void push(SEXP symbol, SEXP env, sexptype_t type, SEXP caller_env) {
        assignement_state_stack_.push_back(AssignmentState(symbol,env, type, caller_env));
    }
    
    AssignmentState& peek() {
        return assignement_state_stack_[assignement_state_stack_.size() - 1];
    }
    
    bool is_empty() {
        return assignement_state_stack_.size() == 0;
    }

    void pop() {
        assignement_state_stack_.pop_back();
    }

  private:
    std::vector<AssignmentState> assignement_state_stack_;
};

#endif /* DYNAMISMTRACER_ASSIGNMENT_STATE_STACK_H */
