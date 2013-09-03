/**
 *  \file IMP/container/ClassnameContainerStatistics.h
 *  \brief A container for Classnames.
 *
 *  BLURB
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 */

#ifndef IMPCONTAINER_CLASSNAME_CONTAINER_INDEX_H
#define IMPCONTAINER_CLASSNAME_CONTAINER_INDEX_H

#include <IMP/container/container_config.h>
#include <IMP/ClassnameContainer.h>
#include <IMP/kernel/internal/container_helpers.h>
#include <IMP/ScoreState.h>
#include <IMP/score_state_macros.h>
#include <IMP/base/set.h>

IMPCONTAINER_BEGIN_INTERNAL_NAMESPACE

/** Store an index that allows one to quickly determine of something
    is in a container.*/
class IMPCONTAINEREXPORT ClassnameContainerIndex : public ScoreState {
  base::Pointer<ClassnameContainer> container_;
  int container_version_;
  IMP::base::set<INDEXTYPE> contents_;
  bool handle_permutations_;

  void build();

 public:
  ClassnameContainerIndex(ClassnameContainerAdaptor c,
                          bool handle_permutations);
  bool get_contains(INDEXTYPE i) const {
    if (handle_permutations_) i = IMP::kernel::internal::get_canonical(i);
    return contents_.find(i) != contents_.end();
  }
  virtual void do_before_evaluate() IMP_OVERRIDE;
  virtual void do_after_evaluate(DerivativeAccumulator *da) IMP_OVERRIDE;
  virtual kernel::ModelObjectsTemp do_get_inputs() const IMP_OVERRIDE;
  virtual kernel::ModelObjectsTemp do_get_outputs() const IMP_OVERRIDE;
  IMP_OBJECT_METHODS(ClassnameContainerIndex);
};

IMPCONTAINER_END_INTERNAL_NAMESPACE

#endif /* IMPCONTAINER_CLASSNAME_CONTAINER_INDEX_H */
