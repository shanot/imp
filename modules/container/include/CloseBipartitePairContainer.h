/**
 *  \file IMP/container/CloseBipartitePairContainer.h
 *  \brief Return all pairs from a SingletonContainer
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2013 IMP Inventors. Close rights reserved.
 */

#ifndef IMPCONTAINER_CLOSE_BIPARTITE_PAIR_CONTAINER_H
#define IMPCONTAINER_CLOSE_BIPARTITE_PAIR_CONTAINER_H

#include <IMP/container/container_config.h>
#include <IMP/core/ClosePairsFinder.h>
#include <IMP/core/internal/CoreCloseBipartitePairContainer.h>
#include <IMP/core/internal/MovedSingletonContainer.h>
#include <IMP/PairContainer.h>
#include <IMP/PairPredicate.h>
#include <IMP/SingletonContainer.h>
#include <IMP/container/ListPairContainer.h>

IMPCONTAINER_BEGIN_NAMESPACE

/** \brief Return all close ordered pairs of particles taken from the
    two SingletonContainers.

    See ClosePairContainer for a more detailed description. This
    container lists all close pairs of particles where one particle is
    taken from each of the input sets.

    See also AllPairContainer, ClosePairContainer,
    AllBipartitePairContainer for variants on the functionality provided.
 */
class IMPCONTAINEREXPORT CloseBipartitePairContainer :
#if defined(IMP_DOXYGEN) || defined(SWIG)
    public PairContainer
#else
    public IMP::core::internal::CoreCloseBipartitePairContainer
#endif
    {
  typedef IMP::core::internal::CoreCloseBipartitePairContainer P;

 public:
  //! Get the individual particles from the passed SingletonContainer
  CloseBipartitePairContainer(SingletonContainerAdaptor a,
                              SingletonContainerAdaptor b, double distance,
                              double slack = 1,
                              std::string name
                              = "CloseBipartitePairContainer%1%");
#ifndef IMP_DOXYGEN
  //! Get the individual particles from the passed SingletonContainer
  CloseBipartitePairContainer(SingletonContainerAdaptor a,
                              SingletonContainerAdaptor b, double distance,
                              core::ClosePairsFinder *cpf, double slack = 1,
                              std::string name
                              = "CloseBipartitePairContainer%1%");
#endif

#if defined(IMP_DOXYGEN) || defined(SWIG)

  /** @name Methods to control the set of filters

      PairPredicate objects can be used as filters to prevent
     the addition of pairs to the containeroutput list. Pairs
     for which the predicates evaluate to a non-zero value are
     excluded from the list.
  */
  /**@{*/
  IMP_LIST_ACTION(public, PairPredicate, PairPredicates, pair_filter,
                  pair_filters, PairPredicate *, PairPredicates,
                  obj->set_was_used(true);
                  , , );
  /**@}*/
  kernel::ParticleIndexPairs get_indexes() const;
  kernel::ParticleIndexPairs get_range_indexes() const;
  void do_before_evaluate();
  kernel::ModelObjectsTemp do_get_inputs() const;
  void do_apply(const PairModifier *sm) const;
  kernel::ParticleIndexes get_all_possible_indexes() const;
#endif
  IMP_OBJECT_METHODS(CloseBipartitePairContainer);
};

IMPCONTAINER_END_NAMESPACE

#endif /* IMPCONTAINER_CLOSE_BIPARTITE_PAIR_CONTAINER_H */
