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
        dynamic_call_count_ = call->is_dynamic_call();
        redefining_ = call->is_redefining_symbol();
        symbol_name_ = call->get_symbol_name();
        symbol_type_ = call->get_symbol_type();
        environment_address_ = call->get_assignment_environment();
        to_package_environment_ = call->to_package_environment();
        to_fresh_environment_ = call->to_fresh_environment();
        parent_id_ = call->get_parent_id();
        location_ = call->get_location();
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

    int get_dynamic_call_count() const {
        return dynamic_call_count_;
    }

    int get_redefining() const {
        return redefining_;
    }

    std::string get_location() const {
        return location_;
    }

    bool try_to_merge(const Call* const call) {
        if (is_mergeable_(call)) {
            call_count_++;
            if (call->is_dynamic_call()) {
                dynamic_call_count_++;
            }
            if (call->is_redefining_symbol()) {
                redefining_++;
            }
            return true;
        }
        return false;
    }

    std::string get_symbol_name() const {
        return symbol_name_;
    }

    sexptype_t get_symbol_type() const {
        return symbol_type_;
    }

    int get_environment_address() const {
        return environment_address_;
    }

    bool to_fresh_environment() const {
        return to_fresh_environment_;
    }

    bool to_package_environment() const {
        return to_package_environment_;
    }

    function_id_t get_parent_id() const {
        return parent_id_;
    }

  private:
    pos_seq_t force_order_;
    pos_seq_t missing_argument_positions_;
    sexptype_t return_value_type_;
    bool jumped_;
    bool S3_method_;
    bool S4_method_;
    int call_count_;
    int dynamic_call_count_;
    int redefining_;
    std::string symbol_name_;
    sexptype_t symbol_type_;
    int environment_address_;
    bool to_fresh_environment_;
    bool to_package_environment_;
    function_id_t parent_id_;
    std::string location_;

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
