#include "probes.h"

#include "TracerState.h"

inline TracerState& tracer_state(dyntracer_t* dyntracer) {
    return *(static_cast<TracerState*>(dyntracer->state));
}

void dyntrace_entry(dyntracer_t* dyntracer, SEXP expression, SEXP environment) {
    TracerState& state = tracer_state(dyntracer);

    /* we do not do state.enter_probe() in this function because this is a
     pseudo probe that executes before the tracing actually starts. this is
     only for initialization purposes. */

    state.initialize();

    /* probe_exit here ensures we start the timer for timing argument execution.
     */
    state.exit_probe(Event::DyntraceEntry);
}

void dyntrace_exit(dyntracer_t* dyntracer,
                   SEXP expression,
                   SEXP environment,
                   SEXP result,
                   int error) {
    TracerState& state = tracer_state(dyntracer);

    state.enter_probe(Event::DyntraceExit);

    state.cleanup(error);

    /* we do not do start.exit_probe() because the tracer has finished
     executing and we don't need to resume the timer. */
}

static inline void set_dispatch(Call* call,
                                const dyntrace_dispatch_t dispatch) {
    if (dispatch == DYNTRACE_DISPATCH_S3) {
        call->set_S3_method();
    } else if (dispatch == DYNTRACE_DISPATCH_S4) {
        call->set_S4_method();
    }
}

void eval_entry(dyntracer_t* dyntracer, const SEXP expr, const SEXP rho) {
    TracerState& state = tracer_state(dyntracer);

    state.enter_probe(Event::EvalEntry);

    state.exit_probe(Event::EvalEntry);
}

void closure_entry(dyntracer_t* dyntracer,
                   const SEXP call,
                   const SEXP op,
                   const SEXP args,
                   const SEXP rho,
                   const dyntrace_dispatch_t dispatch) {
    TracerState& state = tracer_state(dyntracer);

    state.enter_probe(Event::ClosureEntry);

    Call* function_call = state.create_call(call, op, args, rho);

    // static int loopy = 1;
    // if(function_call -> get_function() -> get_id() ==
    // "jhmb9cUOgugzW1R+979kzg==") {
    //     while(loopy);
    // }

    std::string function_name = function_call->get_function_name();

    if (function_name.compare("assign") == 0) {
      Argument* arg = function_call->get_argument(0);

      DenotedValue* value = arg->get_denoted_value();

      SEXP expr = value->get_expression(); // returns a STRSXP

      SEXP expr_symbol = strsxp_to_sym(expr);

      // using the context catches the calls from global scope
      // TODO- check if this way is strictly equal to the one below
      // SEXP calling_env = state.get_stack_().peek(1).get_r_context()->sysparent; 
      Call * parent_call = state.get_parent_call(CLOSXP, 1);

      SEXP calling_env = R_GlobalEnv;
      
      if (parent_call != NULL) {
        calling_env = parent_call->get_environment();
        }

      state.push_assignment_stack(expr_symbol, rho, type_of_sexp(op), calling_env);
    }

    set_dispatch(function_call, dispatch);

    state.push_stack(function_call);

    state.exit_probe(Event::ClosureEntry);
}

void closure_exit(dyntracer_t* dyntracer,
                  const SEXP call,
                  const SEXP op,
                  const SEXP args,
                  const SEXP rho,
                  const dyntrace_dispatch_t dispatch,
                  const SEXP return_value) {
    TracerState& state = tracer_state(dyntracer);

    state.enter_probe(Event::ClosureExit);

    ExecutionContext exec_ctxt = state.pop_stack();

    if (!exec_ctxt.is_closure()) {
        dyntrace_log_error("Not found matching closure on stack");
    }

    Call* function_call = exec_ctxt.get_closure();
    
    std::string function_name = function_call->get_function_name();

    if (function_name.compare("assign") == 0) {
      state.pop_assignment_stack();
    }

    function_call->set_return_value_type(type_of_sexp(return_value));

    state.notify_caller(function_call);

    state.destroy_call(function_call);

    state.exit_probe(Event::ClosureExit);
}

void builtin_entry(dyntracer_t* dyntracer,
                   const SEXP call,
                   const SEXP op,
                   const SEXP args,
                   const SEXP rho,
                   const dyntrace_dispatch_t dispatch) {
    TracerState& state = tracer_state(dyntracer);

    state.enter_probe(Event::BuiltinEntry);

    Call* function_call = state.create_call(call, op, args, rho);

    set_dispatch(function_call, dispatch);

    state.push_stack(function_call);

    state.exit_probe(Event::BuiltinEntry);
}

void builtin_exit(dyntracer_t* dyntracer,
                  const SEXP call,
                  const SEXP op,
                  const SEXP args,
                  const SEXP rho,
                  const dyntrace_dispatch_t dispatch,
                  const SEXP return_value) {
    TracerState& state = tracer_state(dyntracer);

    state.enter_probe(Event::BuiltinExit);

    ExecutionContext exec_ctxt = state.pop_stack();

    if (!exec_ctxt.is_builtin()) {
        dyntrace_log_error("Not found matching builtin on stack");
    }

    Call* function_call = exec_ctxt.get_builtin();

    function_call->set_return_value_type(type_of_sexp(return_value));

    state.notify_caller(function_call);

    state.destroy_call(function_call);

    state.exit_probe(Event::BuiltinExit);
}

void special_entry(dyntracer_t* dyntracer,
                   const SEXP call,
                   const SEXP op,
                   const SEXP args,
                   const SEXP rho,
                   const dyntrace_dispatch_t dispatch) {
    TracerState& state = tracer_state(dyntracer);

    state.enter_probe(Event::SpecialEntry);

    Call* function_call = state.create_call(call, op, args, rho);

    Function * function = function_call->get_function();
    
    if (function->is_super_assign()) {
      state.push_assignment_stack(CAR(args),rho, type_of_sexp(op), rho);
    }
    else if (function->is_left_assign()) {
      state.push_assignment_stack(CAR(args),rho, type_of_sexp(op), rho);
    }
    else if (function->is_equal_assign()) {
      state.push_assignment_stack(CAR(args),rho, type_of_sexp(op), rho);
    }
    else if (function->is_fast_subassign()) {
      state.push_assignment_stack(CAR(args),rho, type_of_sexp(op), rho);
    }

    set_dispatch(function_call, dispatch);

    state.push_stack(function_call);

    state.exit_probe(Event::SpecialEntry);
}


void special_exit(dyntracer_t* dyntracer,
                  const SEXP call,
                  const SEXP op,
                  const SEXP args,
                  const SEXP rho,
                  const dyntrace_dispatch_t dispatch,
                  const SEXP return_value) {
    TracerState& state = tracer_state(dyntracer);

    state.enter_probe(Event::SpecialExit);

    ExecutionContext exec_ctxt = state.pop_stack();

    if (!exec_ctxt.is_special()) {
        dyntrace_log_error("Not found matching special object on stack");
    }

    Call* function_call = exec_ctxt.get_special();
    
    Function * function = function_call->get_function();
    
    if (function->is_super_assign()) {
      state.pop_assignment_stack();
    }
    else if (function->is_left_assign()) {
      state.pop_assignment_stack();
    }
    else if (function->is_equal_assign()) {
      state.pop_assignment_stack();
    }
    else if (function->is_fast_subassign()) {
      state.pop_assignment_stack();
    }

    function_call->set_return_value_type(type_of_sexp(return_value));

    state.notify_caller(function_call);

    state.destroy_call(function_call);

    state.exit_probe(Event::SpecialExit);
}

void jump_single_context(TracerState& state,
                         ExecutionContext& exec_ctxt,
                         bool returned,
                         const sexptype_t return_value_type,
                         const SEXP rho) {
    if (exec_ctxt.is_call()) {
        Call* call = exec_ctxt.get_call();

        call->set_jumped();
        call->set_return_value_type(return_value_type);

        state.notify_caller(call);

        state.destroy_call(call);
    }

    else if (exec_ctxt.is_promise()) {
        DenotedValue* promise = exec_ctxt.get_promise();

        promise->set_value_type(JUMPSXP);

        if (returned && promise->is_argument() &&
            (promise->get_environment() == rho)) {
            promise->set_non_local_return();
        }
    }
}

void context_jump(dyntracer_t* dyntracer,
                  const RCNTXT* context,
                  const SEXP return_value,
                  int restart) {
    TracerState& state = tracer_state(dyntracer);

    state.enter_probe(Event::ContextJump);

    /* Identify promises that do non local return. First, check if
     this special is a 'return', then check if the return happens
     right after a promise is forced, then walk back in the stack
     to the promise with the same environment as the return. This
     promise is the one that does non local return. Note that the
     loop breaks after the first such promise is found. This is
     because only one promise can be held responsible for non local
     return, the one that invokes the return function. */

    execution_contexts_t exec_ctxts(state.unwind_stack(context));

    const SEXP rho = context->cloenv;

    std::size_t context_count = exec_ctxts.size();

    if (context_count == 0) {
    } else if (context_count == 1) {
        jump_single_context(state, exec_ctxts.front(), false, JUMPSXP, rho);
    } else {
        auto begin_iter = exec_ctxts.begin();
        auto end_iter = --exec_ctxts.end();

        bool returned =
            (begin_iter->is_special() &&
             begin_iter->get_special()->get_function()->is_return());

        for (auto iter = begin_iter; iter != end_iter; ++iter) {
            jump_single_context(state, *iter, returned, JUMPSXP, rho);
        }

        jump_single_context(
            state, *end_iter, returned, type_of_sexp(return_value), rho);
    }

    state.exit_probe(Event::ContextJump);
}

void context_exit(dyntracer_t* dyntracer, const RCNTXT* cptr) {
    TracerState& state = tracer_state(dyntracer);

    state.enter_probe(Event::ContextExit);

    ExecutionContext exec_ctxt = state.pop_stack();

    if (!exec_ctxt.is_r_context()) {
        dyntrace_log_error("Nonmatching r context on stack");
    }

    state.exit_probe(Event::ContextExit);
}

void context_entry(dyntracer_t* dyntracer, const RCNTXT* cptr) {
    TracerState& state = tracer_state(dyntracer);

    state.enter_probe(Event::ContextEntry);

    state.push_stack(cptr);

    state.exit_probe(Event::ContextEntry);
}

void environment_variable_define(dyntracer_t* dyntracer,
                                 const SEXP symbol,
                                 const SEXP value,
                                 const SEXP rho) {
  TracerState& state = tracer_state(dyntracer);
  
  state.enter_probe(Event::EnvironmentVariableDefine);
  
  if ((!state.assignment_stack_is_empty())
  and (state.peek_assignment_stack().get_symbol() == symbol)
  and (state.peek_assignment_stack().get_calling_environment() != rho)
  and (!state.is_fresh_environment(rho))
  and (type_of_sexp(value) == CLOSXP))    
  {
      sexptype_t type = state.peek_assignment_stack().get_type();

      Call * assignment_call = state.get_parent_call(type, 1); 

      assignment_call->set_dynamic_call();

      if (state.peek_assignment_stack().get_calling_environment() != R_GlobalEnv) {
        Function * parent_function = state.get_parent_call(type, 2)->get_function();

        std::string parent_function_names = parent_function->get_name_string();

        state.serialize_dynamic_function_definition_(parent_function, parent_function_names);
      }
      else {
        // the function has been called from the global environment
        state.serialize_dynamic_function_definition_(assignment_call->get_function(), "called from global scope");
      }
    }
  
  state.exit_probe(Event::EnvironmentVariableDefine);
}

void environment_variable_assign(dyntracer_t* dyntracer,
                                 const SEXP symbol,
                                 const SEXP value,
                                 const SEXP rho) {
  TracerState& state = tracer_state(dyntracer);
  
  state.enter_probe(Event::EnvironmentVariableAssign);

    if ((!state.assignment_stack_is_empty())
    and (state.peek_assignment_stack().get_symbol() == symbol)
    and (state.peek_assignment_stack().get_calling_environment() != rho)
    and (!state.is_fresh_environment(rho))
    and (type_of_sexp(value) == CLOSXP)) 
    {
      sexptype_t type = state.peek_assignment_stack().get_type();

      Call * assignment_call = state.get_parent_call(type, 1); 

      assignment_call->set_dynamic_call();

      if (state.peek_assignment_stack().get_calling_environment() != R_GlobalEnv) {
        Function * parent_function = state.get_parent_call(type, 2)->get_function();

        std::string parent_function_names = parent_function->get_name_string();

        state.serialize_dynamic_function_definition_(parent_function, parent_function_names);
      }
      else {
        // the function has been called from the global environment
        state.serialize_dynamic_function_definition_(assignment_call->get_function(), "called from global scope");
      }
    }
  
  state.exit_probe(Event::EnvironmentVariableAssign);
}

void environment_variable_remove(dyntracer_t* dyntracer,
                                 const SEXP symbol,
                                 const SEXP rho) {
  TracerState& state = tracer_state(dyntracer);
  
  state.enter_probe(Event::EnvironmentVariableRemove);
  
  state.exit_probe(Event::EnvironmentVariableRemove);
}

void environment_variable_lookup(dyntracer_t* dyntracer,
                                 const SEXP symbol,
                                 const SEXP value,
                                 const SEXP rho) {
  TracerState& state = tracer_state(dyntracer);
  
  state.enter_probe(Event::EnvironmentVariableLookup);
  
  state.exit_probe(Event::EnvironmentVariableLookup);
}
