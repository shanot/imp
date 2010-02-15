/**
 *  \file PairsRestraint.cpp
 *  \brief Implementation
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 *
 */

#include "IMP/container/PairsRestraint.h"


IMPCONTAINER_BEGIN_NAMESPACE

PairsRestraint
::PairsRestraint(PairScore *ss,
                      PairContainer *pc,
                      std::string name): P(ss, pc, name) {

}

void PairsRestraint::do_show(std::ostream& out) const
{
  out << "score " << *ss_ << std::endl;
  out << "container " << *pc_ << std::endl;
}

IMPCONTAINER_END_NAMESPACE
