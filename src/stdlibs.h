#ifndef __STDLIBS_H__
#define __STDLIBS_H__

#ifdef __cplusplus

#    include <R.h>
#    define USE_RINTERNALS
#    include <Rinternals.h>
#    undef TRUE
#    undef FALSE
#    undef length
#    undef eval
#    undef error
#endif

#include <config.h>
#include <stdint.h>
#include <libunwind.h>
#include <cstdio>
#include <cstdlib>
#define R_USE_SIGNALS 1
#define HAVE_DECL_SIZE_MAX 1
#define UNW_LOCAL_ONLY
extern "C" {
#include <Defn.h>
#include <Rdyntrace.h>
}

#ifdef __cplusplus
#    include <cassert>
#    include <chrono>
#    include <ctime>
#    include <fstream>
#    include <functional>
#    include <iostream>
#    include <map>
#    include <set>
#    include <sstream>
#    include <stack>
#    include <string>
#    include <tuple>
#    include <unordered_map>
#    include <unordered_set>
#    include <vector>
#endif

#endif /* __STDLIBS_H__ */
