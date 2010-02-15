/**
 *  \file QuadsOptimizerState.cpp
 *  \brief Use a QuadModifier applied to a QuadContainer to
 *  maintain an invariant
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 *
 */

#include "IMP/container/QuadsOptimizerState.h"
#include <utility>

IMPCONTAINER_BEGIN_NAMESPACE

QuadsOptimizerState
::QuadsOptimizerState(QuadContainer *c,
                           QuadModifier *gm,
                           std::string name):
  OptimizerState(name),
  c_(c)
{
  f_=gm;
}


void QuadsOptimizerState::update()
{
  IMP_OBJECT_LOG;
  if (!f_) return;
  IMP_LOG(TERSE, "Begin QuadsOptimizerState::update" << std::endl);
  IMP_CHECK_OBJECT(f_);
  IMP_CHECK_OBJECT(c_);
  ParticleQuadsTemp set =c_->get_particle_quads();
  f_->apply(set);

  IMP_LOG(TERSE, "End QuadsOptimizerState::update" << std::endl);
}



void QuadsOptimizerState::do_show(std::ostream &out) const {
}

IMPCONTAINER_END_NAMESPACE
