/**
 *  \file Model.cpp \brief Storage of a model, its restraints,
 *                         constraints and particles.
 *
 *  Copyright 2007-2010 IMP Inventors. All rights reserved.
 *
 */

#include "IMP/Model.h"
#include "IMP/Particle.h"
#include "IMP/log.h"
#include "IMP/Restraint.h"
#include "IMP/DerivativeAccumulator.h"
#include "IMP/ScoreState.h"
#include "IMP/RestraintSet.h"
#include <boost/timer.hpp>
#include <set>



IMP_BEGIN_NAMESPACE


namespace {
  struct Statistics {
    double total_time_;
    double total_time_after_;
    unsigned int calls_;
    double total_value_;
    double min_value_;
    double max_value_;
    double last_value_;
    Statistics(): total_time_(0), total_time_after_(0),
                  calls_(0), total_value_(0),
                  min_value_(std::numeric_limits<double>::max()),
                  max_value_(-std::numeric_limits<double>::max()),
                  last_value_(-1)
    {}
  };

  std::map<Object*, Statistics> stats_data_;
}





void Model::set_gather_statistics(bool tf) {
  gather_statistics_=tf;
}


void Model::show_restraint_time_statistics(std::ostream &out) const {
  out << "Restraints: running_time [min_value max_value] "
      << "average_value last_value\n";
  RestraintsTemp r= get_restraints(restraints_begin(), restraints_end()).first;
  for (RestraintsTemp::const_iterator it= r.begin();
       it != r.end(); ++it) {
    if (stats_data_.find(*it) != stats_data_.end()) {
      out << "  " << (*it)->get_name() << ": ";
      out << stats_data_[*it].total_time_/ stats_data_[*it].calls_
          << "s\n";
    } else {
      out << "  " << (*it)->get_name() << ":\n";
    }
  }
}
void Model::show_score_state_time_statistics(std::ostream &out) const {
  out << "ScoreStates: running_time_before running_time_after\n";
  for (ScoreStateConstIterator it= score_states_begin();
       it != score_states_end(); ++it) {
    if (stats_data_.find(*it) != stats_data_.end()) {
      out << "  " << (*it)->get_name() << ": ";
      out << stats_data_[*it].total_time_/ stats_data_[*it].calls_
          << "s "
          << stats_data_[*it].total_time_after_/ stats_data_[*it].calls_
          << "s\n";
    }
  }
}
void Model::show_all_statistics(std::ostream &out) const {
  show_restraint_score_statistics(out);
  show_restraint_time_statistics(out);
  show_score_state_time_statistics(out);
}

void Model::show_restraint_score_statistics(std::ostream &out) const {
  out << "Restraints: [min_value max_value] average_value last_value\n";
  RestraintsTemp r= get_restraints(restraints_begin(), restraints_end()).first;
  for (RestraintsTemp::const_iterator it= r.begin();
       it != r.end(); ++it) {
    if (stats_data_.find(*it) != stats_data_.end()) {
      out << "  " << (*it)->get_name() << ": ";
      out << "["
          << stats_data_[*it].min_value_ << ", "
          << stats_data_[*it].max_value_ << "] "
          << stats_data_[*it].total_value_/ stats_data_[*it].calls_ << " "
          << stats_data_[*it].last_value_ << "\n";
    } else {
      out << "  " << (*it)->get_name() << ":\n";
    }
  }
}


void Model::clear_all_statistics() {
  for (ScoreStateConstIterator it= score_states_begin();
       it != score_states_end(); ++it) {
    stats_data_.erase(*it);
  }
  RestraintsTemp r= get_restraints(restraints_begin(), restraints_end()).first;
  for (RestraintsTemp::const_iterator it= r.begin();
       it != r.end(); ++it) {
    stats_data_.erase(*it);
  }
}

void Model::add_to_update_before_time(ScoreState *s, double t) const {
  ++stats_data_[s].calls_;
  stats_data_[s].total_time_+=t;
}
void Model::add_to_update_after_time(ScoreState *s, double t) const {
  stats_data_[s].total_time_after_+=t;
}
void Model::add_to_restraint_evaluate(Restraint *r, double t,
                                      double score) const {
  stats_data_[r].total_time_+=t;
  ++stats_data_[r].calls_;
  stats_data_[r].min_value_= std::min(score, stats_data_[r].min_value_);
  stats_data_[r].max_value_= std::max(score, stats_data_[r].max_value_);
  stats_data_[r].last_value_=score;
  stats_data_[r].total_value_+=score;
}



IMP_END_NAMESPACE
