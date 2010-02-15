/**
 *  \file MinimumPairRestraint.cpp
 *  \brief The minimum score over a container.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 *
 */

#include "IMP/container/MinimumPairRestraint.h"
#include "IMP/core/internal/MinimalSet.h"
#include <IMP/internal/container_helpers.h>


IMPCONTAINER_BEGIN_NAMESPACE

MinimumPairRestraint
::MinimumPairRestraint(PairScore *f,
                           PairContainer *c,
                           unsigned int n,
                           std::string name):
  Restraint(name),
  f_(f), c_(c), n_(n){
}

namespace {
  typedef core::internal::MinimalSet<double,
          ParticlePair, std::less<double> > MS;
  template <class It, class F>
  MS find_minimal_set(It b, It e, F *f, unsigned int n) {
    IMP_LOG(TERSE, "Finding Minimum " << n << " of "
            << std::distance(b,e) << std::endl);
    MS bestn(n);
    for (It it= b; it != e; ++it) {
      double score= f->evaluate(*it, NULL);

      if (bestn.can_insert(score)) {
        bestn.insert(score, *it);
      }
    }
    return bestn;
  }
}

double MinimumPairRestraint
::unprotected_evaluate(DerivativeAccumulator *da) const {
  MS bestn= find_minimal_set(c_->particle_pairs_begin(),
                             c_->particle_pairs_end(), f_.get(), n_);

  double score=0;
  for (unsigned int i=0; i< bestn.size(); ++i) {
    if (da) {
      f_->evaluate(bestn[i].second, da);
    }
    score+= bestn[i].first;
  }

  return score;
}

void MinimumPairRestraint::do_show(std::ostream &out) const {
  out << "container " << *c_ << std::endl;
  out << "score " << *f_ << std::endl;
}


ParticlesList MinimumPairRestraint::get_interacting_particles() const
{
  MS bestn= find_minimal_set(c_->particle_pairs_begin(),
                             c_->particle_pairs_end(), f_.get(), n_);
  ParticlesList ret;
  for (unsigned int i=0; i< bestn.size(); ++i) {
    ParticlesList pt=f_->get_interacting_particles(bestn[i].second);
    if (!pt.empty()) {
      ret.push_back(IMP::internal::get_union(pt));
    }
  }

  return ret;
}


ParticlesTemp MinimumPairRestraint::get_input_particles() const
{
  return IMP::internal::get_input_particles(c_.get(), f_.get());
}

ContainersTemp MinimumPairRestraint::get_input_containers() const
{
  return ContainersTemp(1, c_);
}


IMPCONTAINER_END_NAMESPACE
