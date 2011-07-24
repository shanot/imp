/**
 *  \file ListPairContainer.cpp   \brief A list of ParticlePairsTemp.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/core/internal/CoreListPairContainer.h"
#include "IMP/PairModifier.h"
#include "IMP/PairScore.h"
#include <algorithm>


IMPCORE_BEGIN_INTERNAL_NAMESPACE


IMP_ACTIVE_CONTAINER_DEF(CoreListPairContainer,);

CoreListPairContainer
::CoreListPairContainer():
  internal::ListLikePairContainer(){}


CoreListPairContainer
::CoreListPairContainer(Model *m, std::string name):
  internal::ListLikePairContainer(m, name){
  initialize_active_container(m);
}


CoreListPairContainer
::CoreListPairContainer(Model *m, const char *name):
  internal::ListLikePairContainer(m, name){
  initialize_active_container(m);
}


void CoreListPairContainer::do_show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << get_number_of_particle_pairs()
      << " Pairs." << std::endl;
}



void CoreListPairContainer
::remove_particle_pairs(const ParticlePairsTemp &c) {
  if (c.empty()) return;
  ParticleIndexPairs cp= IMP::internal::get_index(c);
  remove_from_list(cp);
  IMP_IF_CHECK(USAGE) {
    for (unsigned int i=0; i< c.size(); ++i) {
      IMP_USAGE_CHECK(IMP::internal::is_valid(c[i]),
                    "Passed Pair cannot be NULL (or None)");
    }
  }
}


void CoreListPairContainer::do_before_evaluate() {
  internal::ListLikePairContainer::do_before_evaluate();
}

void CoreListPairContainer::do_after_evaluate() {
  internal::ListLikePairContainer::do_after_evaluate();
}

ParticlesTemp CoreListPairContainer::get_state_input_particles() const {
  return ParticlesTemp();
}

ContainersTemp CoreListPairContainer::get_state_input_containers() const {
  return ContainersTemp();
}

IMPCORE_END_INTERNAL_NAMESPACE
