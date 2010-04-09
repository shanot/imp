/**
 *  \file internal/swig.cpp
 *  \brief Functions for use in swig wrappers
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 */
#include <IMP/internal/swig.h>

IMP_BEGIN_INTERNAL_NAMESPACE


double _ConstRestraint
::unprotected_evaluate(IMP::DerivativeAccumulator *) const {
  return v_;
}
void _ConstRestraint::do_show(std::ostream &out) const {
  out << "value: " << v_ << std::endl;
}
ParticlesList _ConstRestraint::get_interacting_particles() const {
  return ParticlesList();
}
ContainersTemp _ConstRestraint::get_input_containers() const {
  return ContainersTemp();
}
ParticlesTemp _ConstRestraint::get_input_particles() const {
  return ParticlesTemp();
}


double _ConstSingletonScore::evaluate(Particle *p,
                                      IMP::DerivativeAccumulator *da) const {
  return v_;
}
void _ConstSingletonScore::do_show(std::ostream &out) const {
  out << "value: " << v_ << std::endl;
}
ParticlesList
_ConstSingletonScore::get_interacting_particles(Particle *) const {
  return ParticlesList();
}
ContainersTemp
_ConstSingletonScore::get_input_containers(Particle *) const {
  return ContainersTemp();
}
ParticlesTemp _ConstSingletonScore::get_input_particles(Particle *) const {
  return ParticlesTemp();
}
bool _ConstSingletonScore::get_is_changed(Particle *) const {return false;}




double _ConstPairScore::evaluate(const ParticlePair &p,
                                 IMP::DerivativeAccumulator *da) const {
  return v_;
}
void _ConstPairScore::do_show(std::ostream &out) const {
  out << "value: " << v_ << std::endl;
}
ParticlesList
_ConstPairScore::get_interacting_particles(const ParticlePair &) const {
  return ParticlesList();
}
ContainersTemp
_ConstPairScore::get_input_containers(const ParticlePair &) const {
  return ContainersTemp();
}
ParticlesTemp _ConstPairScore::get_input_particles(const ParticlePair &) const {
  return ParticlesTemp();
}
bool _ConstPairScore::get_is_changed(const ParticlePair &) const {return false;}



void _TrivialDecorator::show(std::ostream &out) const {
  out << "trivial decorator " << get_particle()->get_name();
}
void _TrivialDerivedDecorator::show(std::ostream &out) const {
  out << "trivial derived decorator " << get_particle()->get_name();
}

void _TrivialTraitsDecorator::show(std::ostream &out) const {
  out << "trivial traits decorator "<< get_particle()->get_name()
      << " with " << get_decorator_traits();
}







int _overloaded_decorator(_TrivialDecorator a) {
  return 0;
}
int _overloaded_decorator(_TrivialDerivedDecorator a) {
  return 1;
}



std::string _test_ifile(TextInput a) {
  std::string read;
  while (true) {
    std::string cur;
    a.get_stream() >> cur;
    if (!a) break;
    read= read+cur;
  }
  std::cout << read;
  return read;
}
std::string _test_ofile(TextOutput a) {
  static_cast<std::ostream &>(a) << "hi\n"
                                 << " there, how are things"<< std::endl;
  return "hi\n";
}

void _decorator_test(Particle*p) {
  std::cout << "hi " << p->get_name() << std::endl;
}

unsigned int _take_particles(const Particles &ps) {
  for (unsigned int i=0; i< ps.size(); ++i) {
    IMP_CHECK_OBJECT(ps[i]);
  }
  return ps.size();
}

unsigned int _take_particles(Model *m, const Particles &ps) {
  for (unsigned int i=0; i< ps.size(); ++i) {
    IMP_CHECK_OBJECT(ps[i]);
  }
  return ps.size();
}

unsigned int _take_particles(Model *m, const Particles &ps, TextOutput out) {
  for (unsigned int i=0; i< ps.size(); ++i) {
    IMP_CHECK_OBJECT(ps[i]);
  }
  return ps.size();
}
const Particles& _give_particles(Model *m) {
  static Particles ret;
  while(ret.size() <10) {
    ret.push_back(new Particle(m));
  }
  return ret;
}
const Particles& _pass_particles(const Particles &ps) {
  return ps;
}
Particle* _pass_particle(Particle* ps) {
  return ps;
}
const ParticlePair& _pass_particle_pair(const ParticlePair &pp) {
  for (unsigned int i=0; i< 2; ++i) {
    std::cout << pp[i]->get_name() << " ";
  }
  std::cout << std::endl;
  return pp;
}
Particles _give_particles_copy(Model *m) {
  Particles ret;
  while(ret.size() <10) {
    ret.push_back(new Particle(m));
  }
  return ret;
}
FloatKeys _pass_float_keys(const FloatKeys& in) {
  for (unsigned int i=0; i< in.size(); ++i) {
    std::cout << in[i] << " ";
  }
  return in;
}
Floats _pass_floats(const Floats& in) {
  for (unsigned int i=0; i< in.size(); ++i) {
    std::cout << in[i] << " ";
  }
  return in;
}
Ints _pass_ints( Ints in) {
  for (unsigned int i=0; i< in.size(); ++i) {
    std::cout << in[i] << " ";
  }
  return in;
}
const Strings& _pass_strings(const Strings& in) {
  for (unsigned int i=0; i< in.size(); ++i) {
    std::cout << in[i] << " ";
  }
  return in;
}

const Particles &_pass(const Particles &p) {
  for (unsigned int i=0; i< p.size(); ++i) {
    std::cout << p[i]->get_name() << " ";
  }
  return p;
}
const Restraints &_pass(const Restraints &p) {
  for (unsigned int i=0; i< p.size(); ++i) {
    std::cout << p[i]->get_name() << " ";
  }
  return p;
}

const _TrivialDecorators &
_pass_decorators(const internal::_TrivialDecorators &p) {
  for (unsigned int i=0; i< p.size(); ++i) {
    std::cout << p[i] << " ";
  }
  std::cout << std::endl;
  return p;
}

const _TrivialTraitsDecorators &
_pass_decorator_traits(const _TrivialTraitsDecorators &p) {
  for (unsigned int i=0; i< p.size(); ++i) {
    std::cout << p[i] << " ";
  }
  std::cout << std::endl;
  return p;
}

const ParticlePairsTemp &
_pass_particle_pairs(const ParticlePairs &p) {
  for (unsigned int i=0; i< p.size(); ++i) {
    std::cout << p[i] << " ";
  }
  std::cout << std::endl;
  return p;
}


DerivativePair
_pass_pair(const DerivativePair &p) {
  std::cout << p.first << " " << p.second << std::endl;
  return p;
}


int _test_overload(const Particles &ps) {
  return 0;
}

int _test_overload(const Restraints &ps) {
  return 1;
}


IMP_END_INTERNAL_NAMESPACE
