/**
 *  \file algebra_macros.h    \brief Various important macros
 *                           for implementing geometry.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#ifndef IMPALGEBRA_MACROS_H
#define IMPALGEBRA_MACROS_H

#ifdef IMP_DOXYGEN
//! implement the needed namespace methods for a geometry type
/** These are
    - IMP::algebra::get_surface_area()
    - IMP::algebra::get_volume()
    - IMP::algebra::get_bounding_box()
    - output to a stream

    The name of the argument is g and the code snipets should return
    the expected value.
*/
#define IMP_VOLUME_GEOMETRY_METHODS(Name, area, volume, bounding_box)

//! implement the needed namespace methods for a geometry type
/** These are
    - IMP::algebra::get_surface_area()
    - IMP::algebra::get_volume()
    - IMP::algebra::get_bounding_box()
    - output to a stream

    The name of the argument is g and the code snipets should return
    the expected value.
*/
#define IMP_VOLUME_GEOMETRY_METHODS_D(Name, area, volume, bounding_box)

//! implement the needed namespace methods for a geometry type
/** These are
    - IMP::algebra::get_bounding_box()
    - output to a stream

    The name should not include the D.

    The name of the argument is g and the code snipets should return
    the expected value.
*/
#define IMP_LINEAR_GEOMETRY_METHODS(Name, bounding_box)

//! implement the needed namespace methods for a geometry type
/** These are
    - IMP::algebra::get_surface_area()
    - IMP::algebra::get_bounding_box()
    - output to a stream

    The name of the argument is g and the code snipets should return
    the expected value.
*/
#define IMP_AREA_GEOMETRY_METHODS(Name, area, bounding_box)

#else
#define IMP_VOLUME_GEOMETRY_METHODS(Name, area, volume, bounding_box)  \
  IMP_OUTPUT_OPERATOR(Name);                                           \
  inline double get_surface_area(const Name &g) {                      \
    area;                                                              \
  }                                                                    \
  inline double get_volume(const Name &g) {                            \
    volume;                                                            \
  }                                                                    \
  inline BoundingBoxD<3> get_bounding_box(const Name &g) {             \
    bounding_box;                                                      \
  }                                                                    \
  inline const Name &get_geometry(const Name &g) {return g;}           \
  inline void set_geometry(Name &g, const Name &v) {g=v;}              \
  IMP_VALUES(Name, Name##s)

#define IMP_VOLUME_GEOMETRY_METHODS_D(Name, area, volume, bounding_box) \
  IMP_OUTPUT_OPERATOR_D(Name##D);                                       \
  inline double get_surface_area(const Name##D<3> &g) {                 \
    area;                                                               \
  }                                                                     \
  inline double get_volume(const Name##D<3> &g) {                       \
    volume;                                                             \
  }                                                                     \
  template <unsigned int D>                                             \
  inline BoundingBoxD<D> get_bounding_box(const Name##D<D> &g) {        \
    bounding_box;                                                       \
  }                                                                     \
  template <unsigned int D>                                             \
  const Name##D<D> &get_geometry(const Name##D<D> &g) {return g;}       \
  template <unsigned int D>                                             \
  inline void set_geometry(Name##D<D> &g, const Name##D<D> &v) {g=v;}   \
  IMPALGEBRA_EXPORT_TEMPLATE(Name##D<3>);                               \
  typedef Name##D<2> Name##2D;                                          \
  IMP_VALUES(Name##2D, Name##2Ds);                                      \
  typedef Name##D<3> Name##3D;                                          \
  IMP_VALUES(Name##3D, Name##3Ds);                                      \
  typedef Name##D<4> Name##4D;                                          \
  IMP_VALUES(Name##4D, Name##4Ds)


#define IMP_LINEAR_GEOMETRY_METHODS(Name, bounding_box)                \
  IMP_OUTPUT_OPERATOR(Name);                                           \
  inline BoundingBoxD<3> get_bounding_box(const Name &g) {             \
    bounding_box;                                                      \
  }                                                                    \
  inline const Name &get_geometry(const Name &g) {return g;}           \
  IMP_VALUES(Name, Name##s)

#define IMP_AREA_GEOMETRY_METHODS(Name, area, bounding_box)            \
  IMP_OUTPUT_OPERATOR(Name);                                           \
  inline double get_area(const Name &g) {                              \
    area;                                                              \
  }                                                                    \
  inline BoundingBoxD<3> get_bounding_box(const Name &g) {             \
    bounding_box;                                                      \
  }                                                                    \
  inline const Name &get_geometry(const Name &g) {return g;}           \
  inline void set_geometry(Name &g, const Name &v) {g=v;}              \
  IMP_VALUES(Name, Name##s)

#endif

#endif  /* IMPALGEBRA_MACROS_H */
