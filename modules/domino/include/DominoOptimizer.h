/**
 *  \file DominoOptimizer.h   \brief An exact inference optimizer
 *
 *  Copyright 2007-8 Sali Lab. All rights reserved.
 */

#ifndef IMPDOMINO_DOMINO_OPTIMIZER_H
#define IMPDOMINO_DOMINO_OPTIMIZER_H

#include "config.h"
#include "internal/domino_version_info.h"
#include "RestraintGraph.h"

#include <IMP/core/RestraintSet.h>

#include <IMP/Optimizer.h>
#include <IMP/Restraint.h>
#include <IMP/Model.h>

IMPDOMINO_BEGIN_NAMESPACE

//! An exact inference optimizer.
/** \ingroup optimizer
 */
class IMPDOMINOEXPORT DominoOptimizer : public Optimizer
{
  typedef boost::tuple<Restraint *,Particles,Float> OptTuple;
public:
  DominoOptimizer(std::string jt_filename, Model *m);
  IMP_OPTIMIZER(internal::domino_version_info)

  void show(std::ostream &out = std::cout) const {
    out << "DOMINO optimizer" << std::endl;
  }
  void set_sampling_space(DiscreteSampler *ds);

  void show_restraint_graph(std::ostream& out = std::cout) const {
    g_->show(out);
  }
  DiscreteSampler *get_sampling_space() const {return ds_;}
  RestraintGraph  *get_graph() const {return g_;}
  inline unsigned int get_number_of_solutions() const {
      return num_of_solutions_;}
  inline void set_number_of_solutions(unsigned int n){num_of_solutions_=n;}
  void move_to_opt_comb(unsigned int i) const;
  //! Add a restraint that should be used in the optimization procedure.
  /**
  /param[in] r   the restraint
  /param[in] ps  the particles participating in the restraint, at the
                 same hierarhcy level as encoded in the Restraint Graph.
  /param[in] w   the weight of the restraint
   */
  void add_restraint(Restraint *r, Particles ps, Float w);
protected:
  void clear(); //TODO implement!
  //! Creates a new node and add it to the graphs that take part in the
  //! optimization
  /**
    \param[in] node_index the index of the node
    \param[in] particles  the particles that are part of the node
  */
  void add_jt_node(int node_index, std::vector<Int>  &particles_ind,
                   Model &m);
  //! Adds an undirected edge between a pair of nodes
  /** \param[in] node1_ind  the index of the first node
      \param[in] node2_ind  the index of the second node
   */
  void add_jt_edge(int node1_ind, int node2_ind);

  //  void realize_rec(IMP::core::RestraintSet *rs, Float weight);
  void initialize_jt_graph(int number_of_nodes);
  DiscreteSampler *ds_;
  RestraintGraph *g_;
  unsigned int num_of_solutions_;
  std::vector<OptTuple> rs_;
};
IMPDOMINO_END_NAMESPACE

#endif  /* IMPDOMINO_DOMINO_OPTIMIZER_H */
