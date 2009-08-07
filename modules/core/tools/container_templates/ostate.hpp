/**
 *  \file GroupnamesOptimizerState.h
 *  \brief Use a GroupnameModifier applied to a Classnames to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-9 Sali Lab. All rights reserved.
 */

#ifndef IMPCORE_GROUPNAMES_OPTIMIZER_STATE_H
#define IMPCORE_GROUPNAMES_OPTIMIZER_STATE_H

#include "config.h"
#include "internal/version_info.h"
#include <IMP/GroupnameContainer.h>
#include <IMP/GroupnameModifier.h>
#include <IMP/OptimizerState.h>

IMP_BEGIN_NAMESPACE
// for swig
class GroupnameContainer;
class GroupnameModifier;
IMP_END_NAMESPACE

IMPCORE_BEGIN_NAMESPACE
//! Apply a GroupnameFunction to a GroupnameContainer to maintain an invariant
/** \ingroup restraint
    \see GroupnameOptimizerState
 */
class IMPCOREEXPORT GroupnamesOptimizerState : public OptimizerState
{
  Pointer<GroupnameModifier> f_;
  Pointer<GroupnameContainer> c_;
public:
  /** \param[in] c The Container to hold the elements to process
      \param[in] gf The GroupnameModifier to apply to all elements.
      \param[in] name The name to use for this Object
   */
  GroupnamesOptimizerState(GroupnameContainer *c, GroupnameModifier *gf,
                           std::string name="GroupnamesOptimizerState %1%");

  IMP_OPTIMIZER_STATE(GroupnamesOptimizerState, internal::version_info)
};


IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_GROUPNAMES_OPTIMIZER_STATE_H */
