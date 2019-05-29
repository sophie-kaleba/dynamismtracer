#ifndef DYNAMISMTRACER_CALL_SUMMARY_H
#define DYNAMISMTRACER_CALL_SUMMARY_H

#include "Call.h"

class CallSummary {
  public:
    explicit CallSummary(const Call* const call) {
        force_order_ = call->get_force_order();
        missing_argument_positions_ = call->get_missing_argument_positions();
        return_value_type_ = call->get_return_value_type();
        jumped_ = call->is_jumped();
        S3_method_ = call->is_S3_method();
        S4_method_ = call->is_S4_method();
        call_count_ = 1;
        dyn_call_count_ = call->is_dyn_call();
    }

    const pos_seq_t& get_force_order() const {
        return force_order_;
    }

    const pos_seq_t& get_missing_argument_positions() const {
        return missing_argument_positions_;
    }

    sexptype_t get_return_value_type() const {
        return return_value_type_;
    }

    bool is_jumped() const {
        return jumped_;
    }

    bool is_S3_method() const {
        return S3_method_;
    }

    bool is_S4_method() const {
        return S4_method_;
    }

    int get_call_count() const {
        return call_count_;
    }

    int get_dyn_call_count() const {
        return dyn_call_count_;
    }
    
    const std::string& get_total_arg_list() const {
      return arg_list_;
    }

    bool try_to_merge(const Call* const call) {
        if (is_mergeable_(call)) {
            call_count_++;
            if (call->is_dyn_call()) {
                dyn_call_count_++;
            }
            return true;
        }
        return false;
    }

  private:
    pos_seq_t force_order_;
    pos_seq_t missing_argument_positions_;
    sexptype_t return_value_type_;
    bool jumped_;
    bool S3_method_;
    bool S4_method_;
    int call_count_;
    int dyn_call_count_;
    std::string arg_list_;

    bool is_mergeable_(const Call* const call) const {
        return (get_force_order() == call->get_force_order() &&
                get_missing_argument_positions() ==
                    call->get_missing_argument_positions() &&
                is_jumped() == call->is_jumped() &&
                get_return_value_type() == call->get_return_value_type() &&
                is_S3_method() == call->is_S3_method() &&
                is_S4_method() == call->is_S4_method());
    }
};

#endif /* DYNAMISMTRACER_CALL_SUMMARY_H */
