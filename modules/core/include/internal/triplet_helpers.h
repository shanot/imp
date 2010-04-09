/**
 *  \file internal/triplet_helpers.h
 *  \brief A container for particle_triplets.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 */

#ifndef IMPCORE_INTERNAL_TRIPLET_HELPERS_H
#define IMPCORE_INTERNAL_TRIPLET_HELPERS_H

#include "../core_config.h"
#include <IMP/TripletContainer.h>
#include <IMP/TripletModifier.h>
#include <IMP/internal/container_helpers.h>
#include <algorithm>

IMPCORE_BEGIN_INTERNAL_NAMESPACE

class IMPCOREEXPORT ListLikeTripletContainer: public TripletContainer {
private:
  void set_added_and_removed_containers(TripletContainer *,
                                        TripletContainer *){}
  ListLikeTripletContainer *get_added() const {
    return dynamic_cast<ListLikeTripletContainer*>
      (get_added_triplets_container());
  }
  ListLikeTripletContainer *get_removed() const {
    return dynamic_cast<ListLikeTripletContainer*>
      (get_removed_triplets_container());
  }
  ParticleTriplets data_;
protected:
  ListLikeTripletContainer(){}
  const ParticleTriplets &access() const {return data_;}
  void update_list(ParticleTripletsTemp &cur) {
    IMP_IF_CHECK(USAGE) {
      for (unsigned int i=0; i< cur.size(); ++i) {
        IMP_USAGE_CHECK(
         IMP::internal::is_valid(cur[i]),
         "Passed ParticleTriplet cannot be NULL (or None)");
      }
    }
    std::sort(cur.begin(), cur.end());
    if (!get_is_added_or_removed_container()) {
      ParticleTripletsTemp added, removed;
      std::set_difference(cur.begin(), cur.end(),
                          data_.begin(), data_.end(),
                          std::back_inserter(added));
      std::set_difference(data_.begin(), data_.end(),
                          cur.begin(), cur.end(),
                          std::back_inserter(removed));
      get_added()->data_=added;
      get_removed()->data_=removed;
    }
    swap(data_, cur);
  }
  void add_to_list(ParticleTripletsTemp &cur) {
    std::sort(cur.begin(), cur.end());
    ParticleTripletsTemp added;
    std::set_difference(cur.begin(), cur.end(),
                        data_.begin(), data_.end(),
                        std::back_inserter(added));
    unsigned int osz= data_.size();
    data_.insert(data_.end(), added.begin(), added.end());
    std::inplace_merge(data_.begin(), data_.begin()+osz, data_.end());
    if (!get_is_added_or_removed_container()) {
      ListLikeTripletContainer* ac=get_added();
      ac->data_.insert(ac->data_.end(), added.begin(), added.end());
    }
  }
  void add_to_list(const ParticleTriplet& cur) {
    data_.insert(std::lower_bound(data_.begin(),
                                  data_.end(), cur), cur);
    if (!get_is_added_or_removed_container()) {
      ListLikeTripletContainer* ac=get_added();
      ac->data_.push_back(cur);
    }
  }
  ListLikeTripletContainer(Model *m, std::string name):
    TripletContainer(m,name){
    TripletContainer::
      set_added_and_removed_containers( new ListLikeTripletContainer(),
                                        new ListLikeTripletContainer());
  }
public:
  ParticleTriplet get_particle_triplet(unsigned int i) const;
  void apply(const TripletModifier *sm);
  void apply(const TripletModifier *sm,
             DerivativeAccumulator &da);
  double evaluate(const TripletScore *s,
                  DerivativeAccumulator *da) const;
  double evaluate_change(const TripletScore *s,
                         DerivativeAccumulator *da) const;
  double evaluate_prechange(const TripletScore *s,
                            DerivativeAccumulator *da) const;
  unsigned int get_number_of_particle_triplets() const;
  bool get_contains_particle_triplet(const ParticleTriplet& vt) const;
  typedef ParticleTriplets::const_iterator ParticleTripletIterator;
  ParticleTripletIterator particle_triplets_begin() const {
    return data_.begin();
  }
  ParticleTripletIterator particle_triplets_end() const {
    return data_.end();
  }
  ObjectsTemp get_input_objects() const;
  void do_after_evaluate() {
    get_added()->data_.clear();
    get_removed()->data_.clear();
  }
  void do_before_evaluate() {
    std::remove_if(data_.begin(), data_.end(),
         IMP::internal::IsInactive());
  }
  IMP_OBJECT(ListLikeTripletContainer);
  bool get_contained_particles_changed() const;
  ParticlesTemp get_contained_particles() const;
  ContainersTemp get_input_containers() const {
    return ContainersTemp();
  }
};


IMPCORE_END_INTERNAL_NAMESPACE


#define IMP_LISTLIKE_TRIPLET_CONTAINER(Name)               \
  ContainersTemp get_input_containers() const;               \
  IMP_OBJECT(Name);


#endif  /* IMPCORE_INTERNAL_TRIPLET_HELPERS_H */
