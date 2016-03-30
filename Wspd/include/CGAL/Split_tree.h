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

#ifndef CGAL_SPLIT_TREE_H
#define CGAL_SPLIT_TREE_H
#include <CGAL/Split_tree_node.h>
#include <CGAL/Point_container.h>
#include <vector>

namespace CGAL {

template <class Traits>
class Split_tree {
public:
  typedef Split_tree_node<Traits>                                           Node;
  typedef typename Traits::Point_d                                          Point_d;
  typedef typename Traits::Iso_box_d                                        Iso_box_d;
  typedef typename CGAL::Point_container<Traits>                            Point_container;

  typedef std::vector<Iso_box_d>                                            Bounding_box_vector;
  typedef typename Bounding_box_vector::iterator                            Bounding_box_iterator;
private:
  typedef std::vector<Point_d>                                              Point_vector;
  typedef std::vector<const Point_d*>                                       Point_ptr_vector;

public:
  Split_tree(int d_) : d(d_), tree_root(NULL), computed(true), bbox_computed(true) { }

  template <class InputIterator>
  Split_tree(int d_, InputIterator begin, InputIterator end) :
                d(d_), points(begin, end), tree_root(NULL) {
    invalidate_cache();
  }

  Split_tree(const Split_tree<Traits>& split_tree) :
                d(split_tree.d), points(split_tree.points), tree_root(NULL) {
    invalidate_cache();
  }

  ~Split_tree() {
    delete_root();
  }

  Split_tree<Traits>& operator=(const Split_tree<Traits>& split_tree) {
    if (this == &split_tree) {
      return *this;
    }

    delete_root();
    d = split_tree.d;
    points = split_tree.points;
    tree_root = NULL;
    invalidate_cache();
    return *this;
  }

  void compute_bounding_boxes() const {
    compute();
    if(!bbox_computed) {
      bounding_boxes.clear();
      compute_bounding_boxes(tree_root);
      bbox_computed = true;
    }
  }

  Bounding_box_iterator bounding_box_begin() const {
    compute_bounding_boxes();
    return bounding_boxes.begin();
  }

  Bounding_box_iterator bounding_box_end() const {
    compute_bounding_boxes();
    return bounding_boxes.end();
  }

  const Node* root() const {
    compute();
    return tree_root;
  }

  template <class InputIterator>
  void set(int d_, InputIterator begin, InputIterator end) {
    d = d_;
    points.assign(begin, end);
    invalidate_cache();
  }

  template <class InputIterator>
  void add(int d_, InputIterator begin, InputIterator end) {
    CGAL_assertion(points.size() == 0 || d == d_);
    d = d_;
    points.insert(points.end(), begin, end);
    invalidate_cache();
  }


  void clear() {
    points.clear();
    p_vec.clear();
    bounding_boxes.clear();
    delete_root();
    computed = true;
    bbox_computed = true;
  }

  void compute() const {
    if(!computed) {
      delete_root();
      p_vec.clear();
      for(int i = 0; i < points.size(); i++) {
        p_vec.push_back(&points[i]);
      }
      Point_container root_container(d, p_vec.begin(), p_vec.end(), traits);
      tree_root = new Node(d, root_container, traits);
      computed = true;
    }
  }
private:
  void compute_bounding_boxes(const Node* node) const {
    if(!node->is_leaf()) {
      bounding_boxes.push_back(node->bounding_box());
      compute_bounding_boxes(node->left());
      compute_bounding_boxes(node->right());
    }
  }

  void delete_root() const {
    if(tree_root) delete tree_root;
    tree_root = NULL;
  }

  void invalidate_cache() {
    computed = false;
    bbox_computed = false;
  }
private:
  int d;
  Point_vector points;
  Traits traits;

  mutable bool computed;
  mutable Point_ptr_vector p_vec;
  mutable Node* tree_root;

  mutable bool bbox_computed;
  mutable Bounding_box_vector bounding_boxes;
};


} // End namespace
#endif // CGAL_SPLIT_TREE_H
