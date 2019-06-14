#ifndef DYNAMISMTRACER_UTILITIES_H
#define DYNAMISMTRACER_UTILITIES_H

#include "constants.h"
#include "definitions.h"
#include "stdlibs.h"

#include <openssl/evp.h>
#include <type_traits>

char* copy_string(char* destination, const char* source, size_t buffer_size);

bool sexp_to_bool(SEXP value);

SEXP strsxp_to_sym(SEXP value);

int sexp_to_int(SEXP value);

std::string sexp_to_string(SEXP value);

std::string compute_hash(const char* data);

const char* get_name(SEXP sexp);

std::string serialize_r_expression(SEXP e);

std::string clock_ticks_to_string(clock_t ticks);
std::string to_string(const char* str);

inline bool timestamp_is_undefined(const timestamp_t timestamp) {
    return timestamp == UNDEFINED_TIMESTAMP;
}

std::string pos_seq_to_string(const pos_seq_t& pos_seq);

inline bool is_dots_symbol(const SEXP symbol) {
    return symbol == R_DotsSymbol;
}

inline std::string symbol_to_string(const SEXP symbol) {
    return CHAR(PRINTNAME(symbol));
}

template <typename T>
inline void copy_and_reset(T& left, T& right) {
    left = right;
    right = 0;
}

/* is env_a parent of env_b */
inline bool is_parent_environment(SEXP env_a, SEXP env_b) {
    if (env_a == env_b)
        return false;
    for (SEXP env_cur = ENCLOS(env_b); env_cur != R_NilValue;
         env_cur = ENCLOS(env_cur)) {
        if (env_cur == env_a)
            return true;
    }
    return false;
}

template <typename E>
constexpr auto to_underlying(E e) noexcept {
    return static_cast<std::underlying_type_t<E>>(e);
}

#endif /* DYNAMISMTRACER__UTILITIES_H */
