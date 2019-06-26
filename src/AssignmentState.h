#ifndef DYNAMISMTRACER_ASSIGNMENT_STATE_H
#define DYNAMISMTRACER_ASSIGNMENT_STATE_H

#include "Rinternals.h"

class AssignmentState {
  public:
    /* defined in cpp file to get around cyclic dependency issues. */
    explicit AssignmentState(const SEXP symbol,
                    const SEXP environment,
                    const sexptype_t type,
                    const SEXP caller_environment):
                symbol_(symbol),
                environment_(environment),
                type_(type),
                caller_environment_(caller_environment) {}

    const SEXP get_symbol() const {
        return symbol_;
    }

    std::string get_symbol_name() const {
        return to_string(get_name(symbol_));
    }

    const SEXP get_environment() const {
        return environment_;
    };

    const SEXP get_caller_environment() const {
      return caller_environment_;
    }

    const sexptype_t get_type() const {
      return type_;
    }

  private:
    const SEXP symbol_;
    const SEXP environment_;
    const sexptype_t type_;
    const SEXP caller_environment_;
};

#endif /* DYNAMISMTRACER_ASSIGNMENT_STATE_H */
