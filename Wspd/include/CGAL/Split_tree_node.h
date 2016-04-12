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
#include <CGAL/constructions_d.h>
#include <vector>
#include <CGAL/Point_container.h>

namespace CGAL {

template <class Traits>
class Split_tree_node {
public:
  typedef Split_tree_node<Traits>                                    Node;
  typedef typename Traits::Point_d                                   Point_d;
  typedef typename Traits::Iso_box_d                                 Iso_box_d;
  typedef typename Traits::Sphere_d                                  Sphere_d;
  typedef CGAL::Point_container<Traits>                              Point_container;

  typedef typename Traits::FT                                        FT;
  typedef typename internal::Get_dimension_tag<Traits>::Dimension    D;

  typedef CGAL::Midpoint_of_rectangle<Traits>                        Splitter;
  typedef typename Splitter::Separator                               Separator;

public:
  Split_tree_node(int d_, Point_container container_, Traits traits_) :
        traits(traits_), container(d_, container_.begin(), container_.end(), traits_), d(d_),
        bbox_computed(false), circle_computed(false), center_computed(false), squared_radius_computed(false) {
    if(container_.size() >= 2) {
      Point_container left_container(d, traits);
      Point_container right_container(d, container_.begin(), container_.end(), traits);
      Splitter split;
      Separator sep;
      // When Point_container splits itself, it keep the upper half of the points
      // hence the order of right_container and left_container on this functior call.
      split(sep, right_container, left_container);
      left_child = new Node(d, left_container, traits);
      right_child = new Node(d, right_container, traits);
    }
    else {
      left_child = NULL;
      right_child = NULL;
    }
  }

  Split_tree_node(const Split_tree_node& node) :
        traits(node.traits), container(node.d, node.container.begin(), node.container.end(), node.traits), d(node.d),
        bbox_computed(node.bbox_computed), bbox(node.bbox),
        circle_computed(node.circle_computed), circle(node.circle),
        center_computed(node.center_computed), center_(node.center_),
        sqradius(node.sqradius), squared_radius_computed(node.squared_radius_computed) {
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
    if(!bbox_computed) {
      std::vector<FT> min;
      std::vector<FT> max;
      const Kd_tree_rectangle<FT, D>& kdbox = container.bounding_box();
      for(int i = 0; i < d; i++) {
        min.push_back(kdbox.min_coord(i));
        max.push_back(kdbox.max_coord(i));
      }
      bbox = construct_iso_box_d(construct_point_d(d, min.begin(), min.end()),
                        construct_point_d(d, max.begin(), max.end()));
      bbox_computed = true;
    }
    return bbox;
  }

  Sphere_d enclosing_circle() const {
    if(!circle_computed) {
      Iso_box_d bbox = bounding_box();
      circle = construct_sphere_d(d, bbox.min(), bbox.max());
      circle_computed = true;
    }
    return circle;
  }

  Point_d center() const {
    if(!center_computed) {
      Iso_box_d bbox = bounding_box();
      center_ = CGAL::midpoint(bbox.min(), bbox.max());
      center_computed = true;
    }
    return center_;
  }

  FT squared_radius() const {
    if(!squared_radius_computed) {
      Iso_box_d bbox = bounding_box();
      sqradius = CGAL::squared_distance(center(), bbox.max());
      squared_radius_computed = true;
    }
    return sqradius;
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

  //Those are used to lazy compute the values.
  mutable bool bbox_computed;
  mutable Iso_box_d bbox;
  mutable bool circle_computed;
  mutable Sphere_d circle;
  mutable bool center_computed;
  mutable Point_d center_;
  mutable bool squared_radius_computed;
  mutable FT sqradius;

  typename Traits::Construct_iso_box_d construct_iso_box_d;
  typename Traits::Construct_sphere_d construct_sphere_d;
  typename Traits::Construct_point_d construct_point_d;
};

} // End namespace
#endif // CGAL_SPLIT_TREE_NODE_H
