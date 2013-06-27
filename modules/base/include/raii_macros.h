/**
 *  \file IMP/base/raii_macros.h
 *  \brief Various general useful macros for IMP.
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPBASE_RAII_MACROS_H
#define IMPBASE_RAII_MACROS_H
#include <IMP/base/base_config.h>
#include "showable_macros.h"
#include "RAII.h"

//! Declare a RAII-style class
/** Since such classes are typically quite small and simple, all
    the implementation is inline. The macro declares
    - default constructor
    - RAII::set()
    - RAII::reset()
    - destructor
    The Set and Reset arguments are the code to run in the respective
    functions. "args" are the argument string (in parens) for the
    constructor and set.
*/
#define IMP_RAII(Name, args, Initialize, Set, Reset, Show)                     \
  IMP_HELPER_MACRO_PUSH_WARNINGS Name() { Initialize; }                        \
  explicit Name args {                                                         \
    Initialize;                                                                \
    Set;                                                                       \
  }                                                                            \
  void set args {                                                              \
    reset();                                                                   \
    Set;                                                                       \
  }                                                                            \
  void reset() { Reset; }                                                      \
  ~Name() { reset(); }                                                         \
  IMP_HELPER_MACRO_POP_WARNINGS IMP_SHOWABLE_INLINE(Name, out << #Name << '('; \
                                                    Show; out << ')')

#ifdef SWIG
#define IMP_DEPRECATED_RAII(version, help_message, MODULE, Name, args, \
                            Initialize, Set, Reset, Show)              \
  Name();                                                              \
  explicit Name args;                                                  \
  void set args;                                                       \
  void reset();                                                        \
  ~Name();                                                             \
  IMP_SHOWABLE_INLINE(Name, out << #Name << '('; Show; out << ')')
#else
/** Like IMP_RAII, but for deprecated versions (it has hard to apply
    the deprecated macros oneself.*/
#define IMP_DEPRECATED_RAII(version, help_message, MODULE, Name, args,         \
                            Initialize, Set, Reset, Show)                      \
  IMP_HELPER_MACRO_PUSH_WARNINGS IMP##MODULE##_DEPRECATED_VALUE_DECL(version)  \
      Name() {                                                                 \
    IMP##MODULE##_DEPRECATED_VALUE_DEF(version, help_message);                 \
    Initialize;                                                                \
  }                                                                            \
  IMP##MODULE##_DEPRECATED_VALUE_DECL(version) explicit Name args {            \
    IMP##MODULE##_DEPRECATED_VALUE_DEF(version, help_message);                 \
    Initialize;                                                                \
    Set;                                                                       \
  }                                                                            \
  void set args {                                                              \
    reset();                                                                   \
    Set;                                                                       \
  }                                                                            \
  void reset() { Reset; }                                                      \
  ~Name() { reset(); }                                                         \
  IMP_HELPER_MACRO_POP_WARNINGS IMP_SHOWABLE_INLINE(Name, out << #Name << '('; \
                                                    Show; out << ')')
#endif

#endif /* IMPBASE_RAII_MACROS_H */
