/**
 *  \file SingletonsRestraint.cpp
 *  \brief Implementation
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 Sali Lab. All rights reserved.
 *
 */

#include "IMP/container/SingletonsRestraint.h"


IMPCONTAINER_BEGIN_NAMESPACE

SingletonsRestraint
::SingletonsRestraint(SingletonScore *ss,
                      SingletonContainer *pc,
                      std::string name): P(ss, pc, name) {

}

void SingletonsRestraint::do_show(std::ostream& out) const
{
  out << "score " << *ss_ << std::endl;
  out << "container " << *pc_ << std::endl;
}

IMPCONTAINER_END_NAMESPACE
