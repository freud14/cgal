// Copyright (c) 2016 University of Ottawa (Canada)
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Frédérik Paradis (<fpara058@uottawa.ca>)

#ifndef CGAL_WSPD_H
#define CGAL_WSPD_H
#include <CGAL/Split_tree.h>
#include <CGAL/Well_separated_pair.h>
#include <CGAL/Point_container.h>
#include <CGAL/constructions_d.h>
#include <utility>
#include <algorithm>

namespace CGAL {

template <class Traits>
class WSPD {
public:
  typedef CGAL::Split_tree<Traits>                                   Split_tree;
  typedef typename Split_tree::Node                                  Node;
  typedef typename Split_tree::Node_const_handle                     Node_const_handle;
  typedef CGAL::Well_separated_pair<Traits>                          Well_separated_pair;
  typedef typename std::vector<Well_separated_pair>                  Well_separated_pair_decomposition;
  typedef typename Well_separated_pair_decomposition::const_iterator Well_separated_pair_iterator;

  typedef typename Traits::Point_d                                   Point_d;
  typedef typename Traits::FT                                        FT;

  typedef typename std::vector<Point_d>                              Point_vector;
  typedef typename Point_vector::const_iterator                      Point_iterator;

  WSPD(int dimension, FT separation_ratio) :
            d(dimension), s(separation_ratio), stree(d), computed(true) { }

  template <class InputIterator>
  WSPD(int dimension, FT separation_ratio, InputIterator begin, InputIterator end) :
            d(dimension), s(separation_ratio), stree(d, begin, end), points(begin, end), computed(false) { }

  WSPD(const WSPD& rhs) :
            d(rhs.d), s(rhs.s), stree(rhs.stree), points(rhs.points), computed(false) { }

  WSPD& operator=(const WSPD& rhs) {
    if (this == &rhs) {
      return *this;
    }

    d = rhs.d;
    s = rhs.s;
    stree = rhs.stree;
    points = rhs.points;
    computed_wspd.clear();
    computed = false;
    return *this;
  }

  virtual void compute() const {
    if(!computed) {
      computed_wspd.clear();
      compute(stree.root());
      computed = true;
    }
  }

  int dimension() const {
    return d;
  }

  FT separation_ratio() const {
    return s;
  }

  void separation_ratio(FT separation_ratio) {
    if(s != separation_ratio) {
      s = separation_ratio;
      computed = false;
    }
  }

  template <class InputIterator>
  void set(int d, InputIterator begin, InputIterator end) {
    points.assign(begin, end);
    stree.set(d, begin, end);
    computed = false;
  }

  template <class InputIterator>
  void add(InputIterator begin, InputIterator end) {
    points.insert(points.end(), begin, end);
    stree.add(begin, end);
    computed = false;
  }

  void clear() {
    points.clear();
    stree.clear();
    computed_wspd.clear();
    computed = true;
  }

  const Split_tree& split_tree() const {
    return stree;
  }

  const Well_separated_pair_decomposition& wspd() const {
    compute();
    return computed_wspd;
  }

  Well_separated_pair_iterator wspd_begin() const {
    compute();
    return computed_wspd.begin();
  }

  Well_separated_pair_iterator wspd_end() const {
    compute();
    return computed_wspd.end();
  }

  int wspd_size() const {
    compute();
    return computed_wspd.size();
  }

  Point_iterator points_begin() const {
    return points.begin();
  }

  Point_iterator points_end() const {
    return points.end();
  }

  int points_size() const {
    return points.size();
  }
private:
  void compute(Node_const_handle u) const {
    if(u != NULL && !u->is_leaf()) {
      find_pairs(u->left(), u->right());
      compute(u->left());
      compute(u->right());
    }
  }

  void find_pairs(Node_const_handle v, Node_const_handle w) const {
    if(v->is_well_separated_with(w, s)) {
      computed_wspd.push_back(Well_separated_pair(v,w));
    }
    else {
      if(v->has_longuer_side_than(w)) {
        find_pairs(v->left(), w);
        find_pairs(v->right(), w);
      }
      else {
        find_pairs(v, w->left());
        find_pairs(v, w->right());
      }
    }
  }
protected:
  mutable bool computed;
  mutable Well_separated_pair_decomposition computed_wspd;
  int d;
  Split_tree stree;
  Point_vector points;
  FT s;
};


} // End namespace
#endif // CGAL_WSPD_H
