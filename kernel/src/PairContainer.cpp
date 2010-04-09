/**
 *  \file PairContainer.cpp   \brief Container for particle_pair.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/PairContainer.h"
#include "IMP/internal/utility.h"
#include "IMP/PairModifier.h"

IMP_BEGIN_NAMESPACE

namespace {
  unsigned int next_index=0;
}


PairContainer::PairContainer(Model *m, std::string name):
  Container(m, internal::make_object_name(name, next_index++)){
}

// here for gcc
PairContainer::~PairContainer(){
}

IMP_END_NAMESPACE
