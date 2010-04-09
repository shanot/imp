/**
 *  \file ListGroupnameContainer.cpp   \brief A list of Classnames.
 *
 *  This file is generated by a script (core/tools/make-container).
 *  Do not edit directly.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/container/ListGroupnameContainer.h"
#include "IMP/GroupnameModifier.h"
#include "IMP/GroupnameScore.h"
#include <IMP/core/internal/groupname_helpers.h>
#include <algorithm>


IMPCONTAINER_BEGIN_NAMESPACE

ListGroupnameContainer
::ListGroupnameContainer():
  P(){}

ListGroupnameContainer
::ListGroupnameContainer(const Classnames &ps,
                         std::string name):
  P(P::get_model(ps.begin(), ps.end()), name)
{
  IMP_USAGE_CHECK(is_ok(ps.begin(), ps.end()),
                  "All particles must belong to the same model.");
  set_classnames(ps);
}

ListGroupnameContainer
::ListGroupnameContainer(Model *m, std::string name):
  P(m, name){
}

ListGroupnameContainer
::ListGroupnameContainer(Model *m, const char *name):
  P(m, name){
}

void ListGroupnameContainer::do_show(std::ostream &out) const {
  IMP_CHECK_OBJECT(this);
  out <<  get_number_of_classnames()
      << " classnames" << std::endl;
}

IMPCONTAINER_END_NAMESPACE
