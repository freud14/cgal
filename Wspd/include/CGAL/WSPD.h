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
#include <CGAL/Point_container.h>
#include <CGAL/constructions_d.h>
#include <utility>
#include <algorithm>

namespace CGAL {

template <class Traits>
class WSPD {
public:
  typedef CGAL::Split_tree<Traits>                             Split_tree;

  typedef typename CGAL::Split_tree<Traits>::Iso_box_d         Iso_box_d;
  typedef typename CGAL::Split_tree<Traits>::Point_d           Point_d;
  typedef typename CGAL::Split_tree<Traits>::Node              Node;
  typedef typename std::pair<const Node*, const Node*>         Well_separated_pair;
  typedef typename std::vector<Well_separated_pair>            Well_separated_pair_decomposition;
  typedef typename Well_separated_pair_decomposition::iterator Well_separated_pair_iterator;

  typedef typename std::vector<Point_d>                        Point_vector;
  typedef typename Point_vector::iterator                      Point_vector_iterator;

  typedef typename Traits::Vector_d                            Vector_d;
  typedef typename Traits::FT                                  FT;

  WSPD(int d, FT separation_ratio) : s(separation_ratio), computed_split_tree(d), computed(true) { }

  template <class InputIterator>
  WSPD(int d, FT separation_ratio, InputIterator begin, InputIterator end) : s(separation_ratio), computed_split_tree(d, begin, end), computed(false) { }

  virtual void compute() const {
    if(!computed) {
      wspd.clear();
      compute(computed_split_tree.root());
      computed = true;
    }
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
    computed_split_tree.set(d, begin, end);
    computed = false;
  }

  template <class InputIterator>
  void add(InputIterator begin, InputIterator end) {
    points.insert(points.end(), begin, end);
    computed_split_tree.add(begin, end);
    computed = false;
  }

  template <class InputIterator>
  void clear() {
    points.clear();
    computed_split_tree.clear();
    wspd.clear();
    computed = true;
  }

  const Split_tree& split_tree() {
    return computed_split_tree;
  }

  Well_separated_pair_iterator wspd_begin() const {
    compute();
    return wspd.begin();
  }

  Well_separated_pair_iterator wspd_end() const {
    compute();
    return wspd.end();
  }

  Point_vector_iterator points_begin() const {
    return points.begin();
  }

  Point_vector_iterator points_end() const {
    return points.end();
  }

  int size() const {
    compute();
    return wspd.size();
  }

  ~WSPD() { }
private:
  void compute(const Node* u) const {
    if(u != NULL && !u->is_leaf()) {
      find_pairs(u->left(), u->right());
      compute(u->left());
      compute(u->right());
    }
  }

  void find_pairs(const Node* v, const Node* w) const {
    if(are_well_separated(v, w)) {
      wspd.push_back(std::make_pair(v,w));
    }
    else {
      if(has_longuest_side(v, w)) {
        find_pairs(v->left(), w);
        find_pairs(v->right(), w);
      }
      else {
        find_pairs(v, w->left());
        find_pairs(v, w->right());
      }
    }
  }

  bool are_well_separated(const Node* v, const Node* w) const  {
    FT max_rad =  std::max(v->squared_radius(), w->squared_radius());
    FT distance_vw = CGAL::squared_distance(v->center(), w->center());
    return distance_vw >= (s+2)*(s+2)*max_rad;
  }

  bool has_longuest_side(const Node* v, const Node* w) const {
    Iso_box_d rect_v = v->bounding_box();
    Iso_box_d rect_w = w->bounding_box();
    Vector_d vector_v = rect_v.max() - rect_v.min();
    Vector_d vector_w = rect_w.max() - rect_w.min();

    FT max_v = *std::max_element(vector_v.cartesian_begin(), vector_v.cartesian_end());
    FT max_w = *std::max_element(vector_w.cartesian_begin(), vector_w.cartesian_end());
    return max_v > max_w;
  }
protected:
  mutable bool computed;
  mutable Well_separated_pair_decomposition wspd;
  Split_tree computed_split_tree;
  Point_vector points;
  FT s;
};


} // End namespace
#endif // CGAL_WSPD_H
