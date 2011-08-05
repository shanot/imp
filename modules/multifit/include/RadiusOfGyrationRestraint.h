/**
 *  \file RadiusOfGyrationRestraint.h    \brief radius of gyration restraint.
 *
 *  Restrict max distance between every pair of particle
 *
 *  Copyright 2007-2011 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPMULTIFIT_RADIUS_OF_GYRATION_RESTRAINT_H
#define IMPMULTIFIT_RADIUS_OF_GYRATION_RESTRAINT_H

#include "multifit_config.h"
#include <IMP/Restraint.h>
#include <IMP/core/HarmonicUpperBound.h>
#include <IMP/Particle.h>
IMPMULTIFIT_BEGIN_NAMESPACE

//! Ensure the radius of gyration of particles fits the predicted one
/**
/note a harmonic upper bound is applied between
      the predicted radius and the actual radius
 */
class IMPMULTIFITEXPORT RadiusOfGyrationRestraint : public Restraint
{
public:
  //! Constructor
  /**
    \param[in] ps the particles to work on
    \param[in] the number of residues the particles represent
    \param[in] scale allow the radius of gyration of the particles
                      to be at most scale times the predicted one
   */
  RadiusOfGyrationRestraint(Particles ps,int num_residues,Float scale=1.);
  IMP_RESTRAINT(RadiusOfGyrationRestraint);
  IMP_LIST(private, Particle, particle, Particle*, Particles);
  //IMP_OBJECT_INLINE(RadiusOfGyrationRestraint, show(),release(););
 protected:
  Model *mdl_;
  Float predicted_rog_;
  Float scale_;
  Pointer<core::HarmonicUpperBound> hub_;
};

IMPMULTIFITEXPORT
float get_approximated_radius_of_gyration(int len);
IMPMULTIFITEXPORT
float get_actual_radius_of_gyration(Particles ps);
IMPMULTIFITEXPORT
float get_approximated_radius(int len);
IMPMULTIFIT_END_NAMESPACE

#endif /* IMPMULTIFIT_RADIUS_OF_GYRATION_RESTRAINT_H */
