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
#include <utility>
#include <algorithm>
#include <CGAL/number_utils.h>

namespace CGAL {

template <class Traits>
class WSPD {
public:
  typedef CGAL::Split_tree<Traits>                             Split_tree;

  typedef typename CGAL::Split_tree<Traits>::Iso_box_d         Iso_box_d;
  typedef typename CGAL::Split_tree<Traits>::Point_d           Point_d;
  typedef typename CGAL::Split_tree<Traits>::Node              Node;
  typedef typename std::pair<const Node*, const Node*>         Well_separated_pair;

  typedef typename Traits::Sphere_d                            Sphere_d;
  typedef typename Traits::Vector_d                            Vector_d;
  typedef typename Traits::FT                                  FT;

  template <class InputIterator>
  WSPD(int d, double separation_ratio, InputIterator begin, InputIterator end) : s(separation_ratio), split_tree_(d, begin, end) { }

  template <class OutputIterator>
  OutputIterator compute(OutputIterator result) const {
    compute(split_tree_.root(), result);
    return result;
  }

  template <class InputIterator>
  void set(int d, InputIterator begin, InputIterator end) {
    split_tree_ = Split_tree(d, begin, end);
  }

  const Split_tree& split_tree() {
    return split_tree_;
  }

  ~WSPD() { }
private:
  template <class OutputIterator>
  void compute(const Node* u, OutputIterator& result) const {
    if(u != NULL && !u->is_leaf()) {
      find_pairs(u->left(), u->right(), result);
      compute(u->left(), result);
      compute(u->right(), result);
    }
  }

  template <class OutputIterator>
  void find_pairs(const Node* v, const Node* w, OutputIterator& result) const {
    if(are_well_separated(v, w)) {
      *result = std::make_pair(v,w);
      result++;
    }
    else {
      if(has_longuest_side(v, w)) {
        find_pairs(v->left(), w, result);
        find_pairs(v->right(), w, result);
      }
      else {
        find_pairs(v, w->left(), result);
        find_pairs(v, w->right(), result);
      }
    }
  }

  bool are_well_separated(const Node* v, const Node* w) const  {
    Sphere_d cir_v = v->enclosing_circle();
    Sphere_d cir_w = w->enclosing_circle();
    FT max_rad =  std::max(cir_v.squared_radius(), cir_w.squared_radius());
    FT distance_vw = CGAL::squared_distance(cir_v.center(), cir_w.center());
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
private:
  Split_tree split_tree_;
  double s;
};


} // End namespace
#endif // CGAL_WSPD_H
