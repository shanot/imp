/**
 *  \file PairContainerSet.cpp
 *  \brief A set of PairContainers.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/container/PairContainerSet.h"
#include <algorithm>


IMPCONTAINER_BEGIN_NAMESPACE

namespace {
  PairContainerSet* get_set(PairContainer* c) {
    return dynamic_cast<PairContainerSet*>(c);
  }
}

PairContainerSet
::PairContainerSet() {
}

PairContainerSet
::PairContainerSet(Model *m, std::string name):
  PairContainer(m, name),
  deps_(new DependenciesScoreState(this), m){
}

namespace {
  Model *my_get_model(const PairContainersTemp &in) {
    if (in.empty()) {
      IMP_THROW("Cannot initialize from empty list of containers.",
                IndexException);
    }
    return in[0]->get_model();
  }
}

PairContainerSet
::PairContainerSet(const PairContainersTemp& in,
                        std::string name):
  PairContainer(my_get_model(in), name),
  deps_(new DependenciesScoreState(this), my_get_model(in)){
  set_pair_containers(in);
}


bool
PairContainerSet
::get_contains_particle_pair(const ParticlePair& vt) const {
  for (PairContainerConstIterator it= pair_containers_begin();
       it != pair_containers_end(); ++it) {
    if ((*it)->get_contains_particle_pair(vt)) return true;
  }
  return false;
}

void PairContainerSet::do_show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out << get_number_of_particle_pairs()
      << " containers" << std::endl;
}

unsigned int
PairContainerSet::get_number_of_particle_pairs() const {
  unsigned int sum=0;
  for (PairContainerConstIterator it= pair_containers_begin();
       it != pair_containers_end(); ++it) {
    sum+= (*it)->get_number_of_particle_pairs();
  }
  return sum;
}

ParticlePair
PairContainerSet::get_particle_pair(unsigned int i) const {
  for (PairContainerConstIterator it= pair_containers_begin();
       it != pair_containers_end(); ++it) {
    if ( i >= (*it)->get_number_of_particle_pairs()) {
      i-= (*it)->get_number_of_particle_pairs();
    } else {
      return (*it)->get_particle_pair(i);
    }
  }
  throw IndexException("out of range");
}



IMP_LIST_IMPL(PairContainerSet,
              PairContainer,
              pair_container,
              PairContainer*,
              PairContainers,
              {
                if (get_has_added_and_removed_containers()) {
                  get_set(get_added_container())
                    ->add_pair_container(obj
                           ->get_added_container());
                }
                obj->set_was_used(true);
              },{},
              if (container
                  && container->get_has_added_and_removed_containers()) {
                get_set(container->get_removed_container())
                  ->add_pair_container(obj
                       ->get_removed_container());
              });


void PairContainerSet::apply(const PairModifier *sm) {
  for (unsigned int i=0; i< get_number_of_pair_containers(); ++i) {
    get_pair_container(i)->apply(sm);
  }
}

void PairContainerSet::apply(const PairModifier *sm,
                               DerivativeAccumulator &da) {
  for (unsigned int i=0; i< get_number_of_pair_containers(); ++i) {
    get_pair_container(i)->apply(sm, da);
  }
}

double PairContainerSet::evaluate(const PairScore *s,
                                       DerivativeAccumulator *da) const {
  return template_evaluate(s, da);
}

double PairContainerSet::evaluate_if_good(const PairScore *s,
                                               DerivativeAccumulator *da,
                                               double max) const {
  return template_evaluate_if_good(s, da, max);
}


ParticlesTemp PairContainerSet::get_contained_particles() const {
  ParticlesTemp ret;
  for (unsigned int i=0; i< get_number_of_pair_containers(); ++i) {
    ParticlesTemp cur= get_pair_container(i)->get_contained_particles();
    ret.insert(ret.end(), cur.begin(), cur.end());
  }
  return ret;
}



IMPCONTAINER_END_NAMESPACE
