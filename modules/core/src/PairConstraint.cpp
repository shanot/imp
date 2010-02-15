/**
 *  \file PairConstraint.cpp
 *  \brief Use a PairModifier applied to a PairContainer to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 *
 */

#include "IMP/core/PairConstraint.h"
#include "IMP/internal/container_helpers.h"

IMPCORE_BEGIN_NAMESPACE

PairConstraint::PairConstraint(PairModifier *before,
                                         PairModifier *after,
                                         const ParticlePair& vt,
                                         std::string name):
  Constraint(name), v_(vt){
  if (before) f_=before;
  if (after) af_=after;
}


void PairConstraint::do_update_attributes()
{
  IMP_OBJECT_LOG;
  if (!f_) return;
  IMP_LOG(TERSE, "Begin PairsConstraint::update" << std::endl);
  IMP_CHECK_OBJECT(f_);
  f_->apply(v_);
  IMP_LOG(TERSE, "End PairsConstraint::update" << std::endl);
}

void PairConstraint::do_update_derivatives(DerivativeAccumulator *da)
{
  IMP_OBJECT_LOG;
  if (!af_) return;
  IMP_LOG(TERSE, "Begin PairsConstraint::after_evaluate" << std::endl);
  IMP_CHECK_OBJECT(af_);
  af_->apply(v_, *da);
  IMP_LOG(TERSE, "End PairsConstraint::after_evaluate" << std::endl);
}

ParticlesList PairConstraint::get_interacting_particles() const {
  ParticlesList ret0, ret1;
  if (f_) ret0= f_->get_interacting_particles(v_);
  if (af_) ret1= af_->get_interacting_particles(v_);
  ret0.insert(ret0.end(), ret1.begin(), ret1.end());
  return ret0;
}

ContainersTemp PairConstraint::get_input_containers() const {
  return ContainersTemp();
}

ContainersTemp PairConstraint::get_output_containers() const {
  return ContainersTemp();
}

ParticlesTemp PairConstraint::get_input_particles() const {
  ParticlesTemp ret;
  if (f_) {
    ret= f_->get_input_particles(v_);
    ParticlesTemp o= f_->get_output_particles(v_);
    ret.insert(ret.end(), o.begin(), o.end());
    IMP_IF_CHECK(USAGE) {
      if (af_) {
        ParticlesTemp oret= af_->get_input_particles(v_);
        std::sort(ret.begin(), ret.end());
        std::sort(oret.begin(), oret.end());
        ParticlesTemp t;
        std::set_union(ret.begin(), ret.end(), oret.begin(), oret.end(),
                       std::back_inserter(t));
        IMP_USAGE_CHECK(t.size() == ret.size(),
                        "The particles written by "
                        << " the after modifier in " << get_name() << " must "
                        << "be a subset of those read by the before "
                        << "modifier. Before: " << Particles(ret)
                        << " and after " << Particles(oret),
                        UsageException);
      }
    }
  } else {
    ret= af_->get_output_particles(v_);
  }
  return ret;
}

ParticlesTemp PairConstraint::get_output_particles() const {
  ParticlesTemp ret;
  if (f_) {
    ret= f_->get_output_particles(v_);
    IMP_IF_CHECK(USAGE) {
      if (af_) {
        ParticlesTemp oret= af_->get_input_particles(v_);
        ParticlesTemp iret=f_->get_input_particles(v_);
        iret.insert(iret.end(), ret.begin(), ret.end());
        std::sort(iret.begin(), iret.end());
        std::sort(oret.begin(), oret.end());
        ParticlesTemp t;
        std::set_union(iret.begin(), iret.end(), oret.begin(), oret.end(),
                       std::back_inserter(t));
        IMP_USAGE_CHECK(t.size() == iret.size(), "The particles read by "
                      << " the after modifier in " << get_name() << " must "
                        << "be a subset of those written by the before"
                        << " modifier.",
                        ParticlePairException);
      }
    }
  } else {
    ret= af_->get_input_particles(v_);
  }
  return ret;
}

void PairConstraint::do_show(std::ostream &out) const {
  out << "on " << IMP::internal::streamable(v_).get_name() << std::endl;
  if (f_) out << "before " << *f_ << std::endl;
  if (af_) out << "after " << *af_ << std::endl;
}

IMPCORE_END_NAMESPACE
