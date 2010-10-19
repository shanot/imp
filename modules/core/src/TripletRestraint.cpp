/**
 *  \file TripletRestraint.cpp
 *  \brief Implementation
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/core/TripletRestraint.h"
#include <IMP/internal/container_helpers.h>

#include <IMP/log.h>


IMPCORE_BEGIN_NAMESPACE

TripletRestraint
::TripletRestraint(TripletScore *ss,
                     const ParticleTriplet& vt,
                     std::string name):
  TripletScoreRestraint(name),
  ss_(ss),
  v_(vt),
  score_(std::numeric_limits<double>::quiet_NaN())
{
  IMP_IF_CHECK(USAGE) {
    // check the arguments are OK
    ss_->evaluate(v_, NULL);
  }
}

double TripletRestraint
::unprotected_evaluate(DerivativeAccumulator *accum) const
{
  IMP_OBJECT_LOG;
  IMP_CHECK_OBJECT(ss_);
  score_ = ss_->evaluate(v_, accum);

  return score_;
}

double TripletRestraint
::unprotected_incremental_evaluate(DerivativeAccumulator *accum) const
{
  if (ss_->get_is_changed(v_)) {
    score_+=ss_->evaluate_change(v_, accum);
  }
  return score_;
}

ParticlesTemp TripletRestraint::get_input_particles() const
{
  return IMP::internal::get_input_particles(ss_.get(), v_);
}

ContainersTemp TripletRestraint::get_input_containers() const
{
  return IMP::internal::get_input_containers(ss_.get(), v_);
}

void TripletRestraint::do_show(std::ostream& out) const
{
  out << "score " << ss_->get_name() << std::endl;
  out << "data " << IMP::internal::streamable(v_) << std::endl;
}

IMPCORE_END_NAMESPACE
