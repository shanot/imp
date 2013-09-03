/**
 *  \file IMP/container/InContainerClassnameFilter.h
 *  \brief A filter for Classnames.
 *
 *  This file is generated by a script (core/tools/make-containers).
 *  Do not edit directly.
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 */

#ifndef IMPCONTAINER_IN_CONTAINER_CLASSNAME_FILTER_H
#define IMPCONTAINER_IN_CONTAINER_CLASSNAME_FILTER_H

#include <IMP/container/container_config.h>
#include <IMP/ClassnamePredicate.h>
#include <IMP/ClassnameContainer.h>
#include <IMP/kernel/internal/container_helpers.h>
#include "internal/ClassnameContainerIndex.h"
#include <IMP/classname_macros.h>
#include <IMP/base/object_macros.h>
#include <IMP/base/warning_macros.h>

IMPCONTAINER_BEGIN_NAMESPACE

//! A filter which returns true if a container containers the Classname
/** This predicate returns 1 if the passed tuple is in the container.
    \note Use the handle_permutations parameter to the constructor to
    determine whether only exact matchers, or matches under permutation
    are considered matching. By default they are are.
 */
class IMPCONTAINEREXPORT InContainerClassnameFilter
    : public ClassnamePredicate {
  IMP::base::PointerMember<internal::ClassnameContainerIndex> c_;

 public:
  InContainerClassnameFilter(ClassnameContainer *c,
                             std::string name = "ClassnameFilter %1%");
  InContainerClassnameFilter(ClassnameContainer *c, bool handle_permutations,
                             std::string name = "ClassnameFilter %1%");

  virtual int get_value_index(kernel::Model *, PASSINDEXTYPE vt) const IMP_OVERRIDE {
    return c_->get_contains(vt);
  }
  virtual kernel::ModelObjectsTemp do_get_inputs(kernel::Model *m, const kernel::ParticleIndexes &pi)
    const IMP_OVERRIDE {
    kernel::ModelObjectsTemp ret = IMP::get_particles(m, pi);
    ret.push_back(c_);
    return ret;
  }
  IMP_CLASSNAME_PREDICATE_METHODS(InContainerClassnameFilter);
  IMP_OBJECT_METHODS(InContainerClassnameFilter);
};

IMP_OBJECTS(InContainerClassnameFilter, InContainerClassnameFilters);

IMPCONTAINER_END_NAMESPACE

#endif /* IMPCONTAINER_IN_CONTAINER_CLASSNAME_FILTER_H */
