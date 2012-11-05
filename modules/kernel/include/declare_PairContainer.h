/**
 *  \file IMP/declare_PairContainer.h
 *  \brief A container for Pairs.
 *
 *  WARNING This file was generated from declare_NAMEContainer.hpp
 *  in tools/maintenance/container_templates/kernel
 *  by tools/maintenance/make-container.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPKERNEL_DECLARE_PAIR_CONTAINER_H
#define IMPKERNEL_DECLARE_PAIR_CONTAINER_H

#include "kernel_config.h"
#include "internal/IndexingIterator.h"
#include "declare_Particle.h"
#include "container_base.h"
#include "internal/container_helpers.h"
#include "DerivativeAccumulator.h"
#include "internal/OwnerPointer.h"
#include "ParticleTuple.h"
#include <IMP/base/ref_counted_macros.h>
#include <IMP/base/check_macros.h>
#include <IMP/base/Pointer.h>
#include <IMP/base/InputAdaptor.h>
#include <IMP/base/utility_macros.h>
#include <IMP/base/deprecation_macros.h>
#include <algorithm>


IMP_BEGIN_NAMESPACE
class PairModifier;
class PairDerivativeModifier;
class PairScore;

//! A shared container for Pairs
/** Stores a searchable shared collection of Pairs.
    \headerfile PairContainer.h "IMP/PairContainer.h"
    \implementationwithoutexample{PairContainer, IMP_PAIR_CONTAINER}
 */
class IMPEXPORT PairContainer : public Container
{
  IMP_PROTECTED_CONSTRUCTOR(PairContainer, (Model *m,
                           std::string name="PairContainer %1%"), );
public:
  typedef ParticlePair ContainedType;
  typedef ParticlePairsTemp ContainedTypes;
  typedef ParticleIndexPairs ContainedIndexTypes;
  typedef ParticleIndexPair ContainedIndexType;

  //! Apply a SingletonModifier to the contents
  virtual void apply(const PairModifier *sm) const=0;
  //! Apply a SingletonModifier to the contents
  virtual void apply(const PairDerivativeModifier *sm,
                     DerivativeAccumulator &da) const=0;

  //! Evaluate a score on the contents
  virtual double evaluate(const PairScore *s,
                          DerivativeAccumulator *da) const=0;

  //! Evaluate a score on the contents
  virtual double evaluate_if_good(const PairScore *s,
                                  DerivativeAccumulator *da,
                                  double max) const=0;

  /** Get all the indexes contained in the container.*/
  virtual ParticleIndexPairs get_indexes() const=0;
  /** Get all the indexes that might possibly be contained in the
      container, useful with dynamic containers.*/
  virtual ParticleIndexPairs get_all_possible_indexes() const=0;

#ifndef IMP_DOXYGEN
  ParticlePairsTemp get() const {
    return IMP::internal::get_particle(get_model(),
                                       get_indexes());
  }

  ParticlePair get(unsigned int i) const {
    return IMP::internal::get_particle(get_model(),
                                       get_indexes()[i]);
  }
  unsigned int get_number() const {return get_indexes().size();}
#ifndef SWIG
  virtual bool get_provides_access() const {return false;}
  virtual const ParticleIndexPairs& get_access() const {
    IMP_THROW("Object not implemented properly.", base::IndexException);
  }


  template <class Functor>
    Functor for_each(Functor f) {
    ParticleIndexPairs vs=get_indexes();
    // use boost range instead
    return std::for_each(vs.begin(), vs.end(), f);
  }

#endif
#endif

  /** Use this for debugging only.
   */
  ParticlePairsTemp get_particle_pairs() const;

#if defined(IMP_USE_DEPRECATED)
  /** \brief This function is very slow and you should think hard about using
      it.

      \deprecated This is slow and dependent on the order of elements in the
      tuple.

      Return whether the container has the given element.*/
  IMP_DEPRECATED_WARN
    bool get_contains_particle_pair(ParticlePair v) const;

  /** \deprecated This can be very slow and is probably not useful
   */
  IMP_DEPRECATED_WARN unsigned int get_number_of_particle_pairs() const;

  /** \deprecated Use indexes instead and thing about using the
      IMP_CONTAINER_FOREACH() macro.*/
  IMP_DEPRECATED_WARN ParticlePair
    get_particle_pair(unsigned int i) const;

#endif

  IMP_REF_COUNTED_NONTRIVIAL_DESTRUCTOR(PairContainer);
};


/** This class allows either a list or a container to be
    accepted as input.
*/
class IMPEXPORT PairContainerAdaptor:
#if !defined(SWIG) && !defined(IMP_DOXYGEN)
public base::Pointer<PairContainer>
#else
public base::InputAdaptor
#endif
{
  typedef base::Pointer<PairContainer> P;
 public:
  PairContainerAdaptor(){}
  PairContainerAdaptor(PairContainer *c);
  template <class C>
  PairContainerAdaptor(base::internal::PointerBase<C> c): P(c){}
  PairContainerAdaptor(const ParticlePairsTemp &t,
                          std::string name="PairContainerAdaptor%1%");
};


IMP_END_NAMESPACE

#endif  /* IMPKERNEL_DECLARE_PAIR_CONTAINER_H */
