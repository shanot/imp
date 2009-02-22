/**
 *  \file bond_geometry.cpp
 *  \brief XXXX.
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 *
 */

#include "IMP/display/bond_geometry.h"
#include <IMP/core/XYZDecorator.h>


IMPDISPLAY_BEGIN_NAMESPACE

BondGeometry::BondGeometry(core::BondDecorator p): d_(p){
}

BondGeometry::~BondGeometry(){}

void BondGeometry::show(std::ostream &out) const {
  out << "BondGeometry" << std::endl;
}

unsigned int BondGeometry::get_dimension() const{
  return 1;
}
algebra::Vector3D BondGeometry::get_vertex(unsigned int i) const {
  core::BondedDecorator ep=  d_.get_bonded(i);
  core::XYZDecorator epi(ep.get_particle());
  return epi.get_coordinates();
}

Float BondGeometry::get_size() const {
  return 0;
}


BondGeometryExtractor::BondGeometryExtractor(){
}

BondGeometryExtractor::~BondGeometryExtractor(){}

void BondGeometryExtractor::show(std::ostream &out) const {
  out << "BondGeometryExtractor" << std::endl;
}

Geometries BondGeometryExtractor::get_geometry(Particle *p) const {
  return Geometries(1, new BondGeometry(core::BondDecorator(p)));
}

IMPDISPLAY_END_NAMESPACE
