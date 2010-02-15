/**
 *  \file QuadsConstraint.cpp
 *  \brief Use a QuadModifier applied to a QuadContainer to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 *
 */

#include "IMP/container/QuadsConstraint.h"
#include "IMP/internal/container_helpers.h"
#include <utility>

IMPCONTAINER_BEGIN_NAMESPACE

QuadsConstraint::QuadsConstraint(QuadContainer *c,
                                           QuadModifier *before,
                                           QuadModifier *after,
                                           std::string name):
  Constraint(name), c_(c) {
  if (before) f_=before;
  if (after) af_=after;
}


void QuadsConstraint::do_update_attributes()
{
  IMP_OBJECT_LOG;
  if (!f_) return;
  IMP_LOG(TERSE, "Begin QuadsConstraint::update" << std::endl);
  IMP_CHECK_OBJECT(f_);
  IMP_CHECK_OBJECT(c_);
  c_->apply(f_);
  IMP_LOG(TERSE, "End QuadsConstraint::update" << std::endl);
}

void QuadsConstraint::do_update_derivatives(DerivativeAccumulator *da)
{
  IMP_OBJECT_LOG;
  if (!af_) return;
  IMP_LOG(TERSE, "Begin QuadsConstraint::after_evaluate" << std::endl);
  IMP_CHECK_OBJECT(af_);
  IMP_CHECK_OBJECT(c_);
  c_->apply(af_, *da);
  IMP_LOG(TERSE, "End QuadsConstraint::after_evaluate" << std::endl);
}

ParticlesList QuadsConstraint::get_interacting_particles() const {
  ParticlesList ret0, ret1;
  if (f_) ret0 = IMP::internal::get_interacting_particles(c_.get(), f_.get());
  if (af_) ret1= IMP::internal::get_interacting_particles(c_.get(), af_.get());
  ret0.insert(ret0.end(), ret1.begin(), ret1.end());
  return ret0;
}


ContainersTemp QuadsConstraint::get_input_containers() const {
  return ContainersTemp(1, c_);
}

ContainersTemp QuadsConstraint::get_output_containers() const {
  return ContainersTemp();
}

ParticlesTemp QuadsConstraint::get_input_particles() const {
  ParticlesTemp ret;
  if (f_) {
    ret= IMP::internal::get_input_particles(c_.get(), f_.get());
    ParticlesTemp o= IMP::internal::get_output_particles(c_.get(), f_.get());
    ret.insert(ret.end(), o.begin(), o.end());
    IMP_IF_CHECK(USAGE) {
      if (af_) {
        ParticlesTemp oret= IMP::internal::get_output_particles(c_.get(),
                                                                af_.get());
        std::sort(ret.begin(), ret.end());
        std::sort(oret.begin(), oret.end());
        ParticlesTemp t;
        std::set_union(ret.begin(), ret.end(), oret.begin(), oret.end(),
                       std::back_inserter(t));
        IMP_USAGE_CHECK(t.size() == ret.size(), "The particles written by "
                        << " the after modifier in " << get_name()
                        << " must be a subset of those read by the before "
                        << "modifier. Before: " << Particles(ret)
                        << " and after " << Particles(oret),
                        UsageException);
      }
    }
  } else {
    ret= IMP::internal::get_output_particles(c_.get(), af_.get());
  }
  return ret;
}

ParticlesTemp QuadsConstraint::get_output_particles() const {
  ParticlesTemp ret;
  if (f_) {
    ret= IMP::internal::get_output_particles(c_.get(), f_.get());
    IMP_IF_CHECK(USAGE) {
      if (af_) {
        ParticlesTemp oret= IMP::internal::get_input_particles(c_.get(),
                                                               af_.get());
        ParticlesTemp iret=IMP::internal::get_input_particles(c_.get(),
                                                              f_.get());
        iret.insert(iret.end(), ret.begin(), ret.end());
        std::sort(iret.begin(), iret.end());
        std::sort(oret.begin(), oret.end());
        ParticlesTemp t;
        std::set_union(iret.begin(), iret.end(), oret.begin(), oret.end(),
                       std::back_inserter(t));
        IMP_USAGE_CHECK(t.size() == iret.size(), "The particles read by "
                      << " the after modifier in " << get_name() << " must "
                        << "be a subset of those written by the before "
                        << "modifier.",
                        UsageException);
      }
    }
  } else {
    ret= IMP::internal::get_input_particles(c_.get(), af_.get());
  }
  return ret;
}

void QuadsConstraint::do_show(std::ostream &out) const {
  out << "on " << *c_ << std::endl;
  if (f_) out << "before " << *f_ << std::endl;
  if (af_) out << "after " << *af_ << std::endl;
}

IMPCONTAINER_END_NAMESPACE
