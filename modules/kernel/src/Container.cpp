/**
 *  \file Restraint.cpp   \brief Abstract base class for all restraints.
 *
 *  Copyright 2007-2013 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/kernel/container_base.h"
#include "IMP/kernel/internal/utility.h"
#include "IMP/kernel/Particle.h"
#include "IMP/kernel/Model.h"
#include "IMP/kernel/internal/graph_utility.h"
#include "IMP/kernel/dependency_graph.h"

IMPKERNEL_BEGIN_NAMESPACE

Container::Container(kernel::Model *m, std::string name) : ScoreState(m, name) {
  IMP_USAGE_CHECK(m, "Must pass model to container constructor.");
  // incremented to 0 at start
  version_ = -1;
#if IMP_HAS_CHECKS >= IMP_INTERNAL
  writeable_ = true;
  readable_ = true;
#endif
}

void Container::set_is_changed(bool tf) {
  validate_writable();
  if (tf) {
    ++version_;
    if (version_ < 0) version_ = 0;
  }
}

bool Container::get_is_changed() const {
  IMPKERNEL_DEPRECATED_FUNCTION_DEF(2.1, "Use get_contents_version() instead.");
  return true;
}

void Container::do_after_evaluate(DerivativeAccumulator *) {}

void Container::validate_readable() const {
#if IMP_HAS_CHECKS >= IMP_INTERNAL
  if (!readable_) {
    base::handle_error("bad container read");
    throw internal::InputOutputException(get_name(),
                                         internal::InputOutputException::GET);
  }
#endif
}
void Container::validate_writable() const {
#if IMP_HAS_CHECKS >= IMP_INTERNAL
  if (!writeable_) {
    base::handle_error("bad container written");
    throw internal::InputOutputException(get_name(),
                                         internal::InputOutputException::GET);
  }
#endif
}
void Container::set_is_readable(bool tf) {
#if IMP_HAS_CHECKS >= IMP_INTERNAL
  readable_ = tf;
#else
  IMP_UNUSED(tf);
#endif
}
void Container::set_is_writable(bool tf) {
#if IMP_HAS_CHECKS >= IMP_INTERNAL
  writeable_ = tf;
#else
  IMP_UNUSED(tf);
#endif
}

IMPKERNEL_END_NAMESPACE
