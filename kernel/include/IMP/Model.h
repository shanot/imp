/**
 *  \file Model.h   \brief Storage of a model, its restraints,
 *                         constraints and particles.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#ifndef __IMP_MODEL_H
#define __IMP_MODEL_H

#include "IMP_config.h"
#include "internal/Object.h"
#include "internal/kernel_version_info.h"
#include "base_types.h"
#include "VersionInfo.h"

namespace IMP
{

class Particle;
class Restraint;
class ScoreState;
typedef std::vector<Restraint*> Restraints;
typedef std::vector<ScoreState*> ScoreStates;

//! Class for storing model, its restraints, and particles.
/** All attribute data for particles is stored through indexing in the
    model_data_ structure.
    Currently no suport for constraints (e.g. rigid bodies).
    \ingroup kernel
 */
class IMPDLLEXPORT Model: public internal::Object
{
  friend class Restraint;
public:
  Model();
  ~Model();

  IMP_CONTAINER(Particle, particle, ParticleIndex);
  IMP_CONTAINER(ScoreState, score_state, ScoreStateIndex);
  IMP_CONTAINER(Restraint, restraint, RestraintIndex);
 public:


  //! Evaluate all of the restraints in the model and return the score.
  /** \param[in] calc_derivs If true, also evaluate the first derivatives.
      \return The score.
   */
  Float evaluate(bool calc_derivs);

  //! Show the model contents.
  /** \param[in] out Stream to write model description to.
   */
  void show (std::ostream& out = std::cout) const;

  //! \return version and authorship information.
  VersionInfo get_version_info() const {
    return internal::kernel_version_info;
  }
};


} // namespace IMP

#endif  /* __IMP_MODEL_H */
