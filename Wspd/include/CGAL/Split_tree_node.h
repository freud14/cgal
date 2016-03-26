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

#ifndef CGAL_SPLIT_TREE_NODE_H
#define CGAL_SPLIT_TREE_NODE_H
#include <CGAL/Splitters.h>
#include <vector>
#include <CGAL/number_utils.h>
#include <CGAL/constructions_d.h>

namespace CGAL {

template <class Traits>
class Split_tree;

template <class Traits>
class Split_tree_node {
  friend class Split_tree<Traits>;

  typedef typename Split_tree<Traits>::Node                          Node;
  typedef typename Split_tree<Traits>::Point_d                       Point_d;
  typedef typename Split_tree<Traits>::Iso_box_d                     Iso_box_d;
  typedef typename Traits::Sphere_d                                  Sphere_d;
  typedef typename Traits::Vector_d                                  Vector_d;
  typedef typename Traits::Aff_transformation_d                      Aff_transformation_d;
  typedef typename Split_tree<Traits>::Point_container               Point_container;

  typedef typename Traits::FT                                        FT;
  typedef typename Traits::RT                                        RT;
  typedef typename internal::Get_dimension_tag<Traits>::Dimension    D;

  typedef CGAL::Midpoint_of_rectangle<Traits>                        Splitter;
  typedef typename Splitter::Separator                               Separator;

public:
  Split_tree_node(int d_, Traits traits_) : traits(traits_), container(d_, traits_), d(d_), left_child(NULL), right_child(NULL) { }

  Split_tree_node(int d_, Point_container container_, Traits traits_) : traits(traits_), container(d_, container_.begin(), container_.end(), traits_), d(d_) {
    if(container_.size() >= 2) {
      Point_container left_container(d, container_.begin(), container_.end(), traits);
      Point_container right_container(d, traits);
      Splitter split;
      Separator sep;
      split(sep, left_container, right_container);
      left_child = new Node(d, left_container, traits);
      right_child = new Node(d, right_container, traits);
    }
    else {
      left_child = NULL;
      right_child = NULL;
    }
  }

  Split_tree_node(const Split_tree_node& node) : traits(node.traits), container(node.d, node.container.begin(), node.container.end(), node.traits), d(node.d) {
    if(node.left_child != NULL && node.right_child != NULL) {
      left_child = new Node(*node.left_child);
      right_child = new Node(*node.right_child);
    }
    else {
      left_child = NULL;
      right_child = NULL;
    }
  }

  ~Split_tree_node() {
    delete left_child;
    delete right_child;
  }

  inline const Node* left() const { return left_child;}
  inline const Node* right() const { return right_child; }
  inline bool is_leaf() const { return left_child == NULL || right_child == NULL; }

  Iso_box_d bounding_box() const {
    const Kd_tree_rectangle<FT, D>& bbox = container.bounding_box();
    std::vector<FT> min;
    std::vector<FT> max;
    for(int i = 0; i < d; i++) {
      min.push_back(bbox.min_coord(i));
      max.push_back(bbox.max_coord(i));
    }
    return Iso_box_d(traits.construct_point_d_object()(d, min.begin(), min.end()),
                      traits.construct_point_d_object()(d, max.begin(), max.end()));
  }

  Sphere_d enclosing_circle() const {
    Iso_box_d bbox = bounding_box();
    Point_d center = CGAL::midpoint(bbox.min(), bbox.max());
    Vector_d radius_vector = bbox.max() - center;
    std::vector<Point_d> points_on_sphere;
    points_on_sphere.push_back(bbox.min());
    points_on_sphere.push_back(bbox.max());
    for(int i = 1; i < d; i++) {
      Point_d point_on_sphere = center + radius_vector.transform(traits.construct_aff_transformation_d_object()(d, Rotation(), RT(1), RT(0), RT(1), 0, i));
      points_on_sphere.push_back(point_on_sphere);
    }
    return traits.construct_sphere_d_object()(d, points_on_sphere.begin(), points_on_sphere.end());
  }

  const Point_container& point_container() const {
    return container;
  }
private:
  Traits traits;
  Node* left_child;
  Node* right_child;
  Point_container container; //Node
  int d;
};

} // End namespace
#endif // CGAL_SPLIT_TREE_NODE_H
