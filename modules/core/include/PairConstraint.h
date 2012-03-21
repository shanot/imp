/**
 *  \file PairConstraint.h
 *  \brief Use a PairModifier applied to a ParticlePairsTemp to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2012 IMP Inventors. All rights reserved.
 */

#ifndef IMPCORE_PAIR_CONSTRAINT_H
#define IMPCORE_PAIR_CONSTRAINT_H

#include "core_config.h"
#include <IMP/PairModifier.h>
#include <IMP/PairDerivativeModifier.h>
#include <IMP/Constraint.h>
#include <IMP/Particle.h>
#include <IMP/score_state_macros.h>

namespace IMP {
// for swig
class PairModifier;
}

IMPCORE_BEGIN_NAMESPACE
//! Apply a PairFunction to a Pair
/** The score state is passed up to two PairModifiers, one to
    apply before evaluation and the other after. The one after
    should take a DerivativeAccumulator as its last argument for
    PairModifier::apply() and will only be called if
    the score was computed with derivatives.

    \see container::PairsConstraint
 */
class IMPCOREEXPORT PairConstraint : public Constraint
{
  IMP::OwnerPointer<PairModifier> f_;
  IMP::OwnerPointer<PairDerivativeModifier> af_;
  ParticleIndexPair v_;
public:
  /** before and after are the modifiers to apply before and after
      evaluate.
   */
  PairConstraint(PairModifier *before,
                      PairDerivativeModifier *after,
                      const ParticlePair& vt,
                      std::string name="PairConstraint %1%");

  //! Apply this modifier to all the elements after an evaluate
  void set_after_evaluate_modifier(PairDerivativeModifier* f) {
    af_=f;
  }

  //! Apply this modifier to all the elements before an evaluate
  void set_before_evaluate_modifier(PairModifier* f) {
    f_=f;
  }

  IMP_CONSTRAINT(PairConstraint);
};


IMPCORE_END_NAMESPACE

#endif  /* IMPCORE_PAIR_CONSTRAINT_H */
