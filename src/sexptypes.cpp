#include "sexptypes.h"

const sexptype_t UNBOUNDSXP = 100002;
const sexptype_t UNASSIGNEDSXP = 100003;
const sexptype_t MISSINGSXP = 100004;
const sexptype_t JUMPSXP = 100005;
const sexptype_t CONTEXTSXP = 100006;
const sexptype_t NULLSXP = 100007;

std::string sexptype_to_string(sexptype_t sexptype) {
    switch (sexptype) {
    case CONTEXTSXP:
        return "Context";
    case JUMPSXP:
        return "Unknown (Jumped)";
    case MISSINGSXP:
        return "Missing";
    case UNASSIGNEDSXP:
        return "Unassigned";
    case NILSXP:
        return "Null";
    case UNBOUNDSXP:
        return "Unbound";
    case LANGSXP:
        return "Function Call";
    case NEWSXP:
        return "New";
    case FREESXP:
        return "Free";
    case FUNSXP:
        return "Closure or Builtin";
    case NULLSXP:
        return "Null Pointer";
    default:
        std::string str(type2char(sexptype));
        str[0] = std::toupper(str[0]);
        return str;
    }
}

sexptype_t type_of_sexp(SEXP value) {
    if (value == NULL) {
        return NULLSXP;
    }
    if (value == R_UnboundValue) {
        return UNBOUNDSXP;
    }
    if (value == R_MissingArg) {
        return MISSINGSXP;
    }
    return static_cast<sexptype_t>(TYPEOF(value));
}

std::string value_type_to_string(SEXP value) {
    return sexptype_to_string(type_of_sexp(value));
}

void inspect_sexp(SEXP op, SEXP args) {
    std::cout << "OP is " << value_type_to_string(op) << "\n"  ;
    std::cout << "ARGS are " << value_type_to_string(args) << "\n"  ;
    if (value_type_to_string(args).compare("Pairlist") == 0) {
        std::cout << "Parlist[1] is " << value_type_to_string(CAR(args)) << " whose value is " << CHAR(PRINTNAME(CAR(args))) << "\n"  ;
        std::cout << "Pairlist[2] is " << value_type_to_string(CDR(args)) << " whose CAR is " << value_type_to_string(CADR(args)) << "\n"  ;
        
        if (value_type_to_string(CADR(args)).compare("Function Call") == 0) {
            std::cout << "LANGSXP[1] is " << value_type_to_string(CAR(CADR(args))) << " whose value is " << CHAR(PRINTNAME(CAR(CADR(args)))) << "\n\n"; 
        }
    }
}

void inspect_sexp_args(SEXP args) {
    std::cout << "ARGS are " << value_type_to_string(args) << "\n"  ;
    if (value_type_to_string(args).compare("Pairlist") == 0) {
        std::cout << "Parlist[1] is " << value_type_to_string(CAR(args)) << " whose value is " << CHAR(PRINTNAME(CAR(args))) << "\n"  ;
        std::cout << "Pairlist[2] is " << value_type_to_string(CDR(args)) << " whose CAR is " << value_type_to_string(CADR(args)) << "\n"  ;
        
        if (value_type_to_string(CADR(args)).compare("Function Call") == 0) {
            std::cout << "LANGSXP[1] is " << value_type_to_string(CAR(CADR(args))) << " whose value is " << CHAR(PRINTNAME(CAR(CADR(args)))) << "\n\n"; 
        }
    }
    
}
