/**
 *  \file SingletonsOptimizerState.h
 *  \brief Use a SingletonModifier applied to a Particles to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_SINGLETONS_OPTIMIZER_STATE_H
#define IMPCORE_SINGLETONS_OPTIMIZER_STATE_H

#include "config.h"
#include <IMP/SingletonContainer.h>
#include <IMP/SingletonModifier.h>
#include <IMP/OptimizerState.h>

IMP_BEGIN_NAMESPACE
// for swig
class SingletonContainer;
class SingletonModifier;
IMP_END_NAMESPACE

IMPCORE_BEGIN_NAMESPACE
//! Apply a SingletonFunction to a SingletonContainer to maintain an invariant
/** \ingroup restraint
    \see SingletonOptimizerState
 */
class IMPCOREEXPORT SingletonsOptimizerState : public OptimizerState
{
  IMP::internal::OwnerPointer<SingletonModifier> f_;
  IMP::internal::OwnerPointer<SingletonContainer> c_;
public:
  /** \param[in] c The Container to hold the elements to process
      \param[in] gf The SingletonModifier to apply to all elements.
      \param[in] name The name to use for this Object
   */
  SingletonsOptimizerState(SingletonContainer *c, SingletonModifier *gf,
                           std::string name="SingletonsOptimizerState %1%");

  IMP_OPTIMIZER_STATE(SingletonsOptimizerState, get_module_version_info())
};


IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_SINGLETONS_OPTIMIZER_STATE_H */
