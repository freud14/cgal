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

namespace CGAL {

template <class Traits>
class Split_tree {

public:
  typedef Split_tree_node<Traits>                                           Node;
  typedef typename Traits::Point_d                                          Point_d;
  typedef typename Traits::Iso_box_d                                        Iso_box_d;
  typedef typename CGAL::Point_container<Traits>                            Point_container;
private:
  typedef std::vector<Point_d>                                              Point_vector;
  typedef std::vector<const Point_d*>                                       Point_ptr_vector;

public:
  template <class InputIterator>
  Split_tree(int d_, InputIterator begin, InputIterator end) : d(d_), points(begin, end) {
    construtor();
  }

  Split_tree(const Split_tree<Traits>& split_tree) : d(split_tree.d), points(split_tree.points) {
    construtor();
  }

  ~Split_tree() {
    destructor();
  }

  Split_tree<Traits>& operator=(const Split_tree<Traits>& split_tree) {
    if (this == &split_tree) {
      return *this;
    }

    destructor();
    d = split_tree.d;
    points = Point_vector(split_tree.points);
    construtor();
    return *this;
  }

  template <class OutputIterator>
  OutputIterator bounding_boxes(OutputIterator result) const {
    bounding_boxes(tree_root, result);;
    return result;
  }

  const Node* root() const {
    return tree_root;
  }
private:
  template <class OutputIterator>
  void bounding_boxes(const Node* node, OutputIterator& result) const {
    if(!node->is_leaf()) {
      *result = node->bounding_box();
      result++;
      bounding_boxes(node->left(), result);
      bounding_boxes(node->right(), result);
    }
  }

  void construtor() {
    for(int i = 0; i < points.size(); i++) {
      p_vec.push_back(&points[i]);
    }
    Point_container root_container(d, p_vec.begin(), p_vec.end(), traits);
    tree_root = new Node(d, root_container, traits);
  }

  void destructor() {
    delete tree_root;
  }

private:
  int d;
  Point_vector points;
  Point_ptr_vector p_vec;
  Traits traits;
  Node* tree_root;
};


} // End namespace
#endif // CGAL_SPLIT_TREE_H
