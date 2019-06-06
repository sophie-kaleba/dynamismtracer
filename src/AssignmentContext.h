#ifndef DYNAMISMTRACER_ASSIGNMENT_CONTEXT_H
#define DYNAMISMTRACER_ASSIGNMENT_CONTEXT_H

#include "Rinternals.h"

class AssignmentContext {
  public:
    /* defined in cpp file to get around cyclic dependency issues. */
    explicit AssignmentContext(const SEXP symbol,
                    const SEXP environment):
                symbol_(symbol),
                environment_(environment) {}

    const SEXP get_symbol() const {
        return symbol_;
    }

    const SEXP get_environment() const {
        return environment_;
    };

  private:
    const SEXP symbol_;
    const SEXP environment_;
};

#endif /* DYNAMISMTRACER_ASSIGNMENT_CONTEXT_H */
