/**
 *  \file atom/angle_decorators.h     \brief Decorators for angles
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 *
 */

#ifndef IMPATOM_ANGLE_DECORATORS_H
#define IMPATOM_ANGLE_DECORATORS_H

#include "config.h"
#include <IMP/core/XYZ.h>
#include <IMP/Decorator.h>

IMPATOM_BEGIN_NAMESPACE

class IMPATOMEXPORT Angle : public Decorator
{
public:
  IMP_DECORATOR(Angle, Decorator);

  //! Create an angle with the given particles.
  static Angle setup_particle(Particle *p, core::XYZ a, core::XYZ b,
                              core::XYZ c) {
    p->add_attribute(get_particle_key(0), a);
    p->add_attribute(get_particle_key(1), b);
    p->add_attribute(get_particle_key(2), c);
    return Angle(p);
  }

  //! Return true if the particle is an angle.
  static bool particle_is_instance(Particle *p) {
    for (unsigned int i = 0; i < 3; ++i) {
      if (!p->has_attribute(get_particle_key(i))) return false;
    }
    return true;
  }

  Particle* get_particle() const {
    return Decorator::get_particle();
  }

  //! Get the ith particle in the angle.
  Particle* get_particle(unsigned int i) const {
    return get_particle()->get_value(get_particle_key(i));
  }

  IMP_DECORATOR_GET_SET_OPT(ideal, get_ideal_key(), Float, Float, -1);
  IMP_DECORATOR_GET_SET_OPT(stiffness, get_stiffness_key(), Float, Float, -1);

  static ParticleKey get_particle_key(unsigned int i);
  static FloatKey get_ideal_key();
  static FloatKey get_stiffness_key();
};

IMP_OUTPUT_OPERATOR(Angle);

class IMPATOMEXPORT Dihedral : public Decorator
{
public:
  IMP_DECORATOR(Dihedral, Decorator);

  //! Create a dihedral with the given particles.
  static Dihedral setup_particle(Particle *p, core::XYZ a, core::XYZ b,
                                 core::XYZ c, core::XYZ d) {
    p->add_attribute(get_particle_key(0), a);
    p->add_attribute(get_particle_key(1), b);
    p->add_attribute(get_particle_key(2), c);
    p->add_attribute(get_particle_key(3), d);
    return Dihedral(p);
  }

  //! Return true if the particle is a dihedral.
  static bool particle_is_instance(Particle *p) {
    for (unsigned int i = 0; i < 4; ++i) {
      if (!p->has_attribute(get_particle_key(i))) return false;
    }
    return true;
  }

  Particle* get_particle() const {
    return Decorator::get_particle();
  }

  //! Get the ith particle in the dihedral.
  Particle* get_particle(unsigned int i) const {
    return get_particle()->get_value(get_particle_key(i));
  }

  IMP_DECORATOR_GET_SET_OPT(ideal, get_ideal_key(), Float, Float, -1);
  IMP_DECORATOR_GET_SET_OPT(multiplicity, get_multiplicity_key(),
                            Int, Int, 0);
  IMP_DECORATOR_GET_SET_OPT(stiffness, get_stiffness_key(), Float, Float, -1);

  static ParticleKey get_particle_key(unsigned int i);
  static FloatKey get_ideal_key();
  static IntKey get_multiplicity_key();
  static FloatKey get_stiffness_key();
};

IMP_OUTPUT_OPERATOR(Dihedral);

IMPATOM_END_NAMESPACE

#endif  /* IMPATOM_ANGLE_DECORATORS_H */
