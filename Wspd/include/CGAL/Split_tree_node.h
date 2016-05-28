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

template <class Traits, class Rep_tag>
class Construct_bounding_box_;

template <class Traits>
class Construct_bounding_box_<Traits, Cartesian_tag> {
public:
  typedef typename Traits::Iso_box_d                                 Iso_box_d;
  typedef typename Traits::FT                                        FT;
  typedef typename internal::Get_dimension_tag<Traits>::Dimension    D;
  typedef CGAL::Point_container<Traits>                              Point_container;

  Iso_box_d operator()(int d, const Point_container& container) const {
    std::vector<FT> min;
    std::vector<FT> max;
    const Kd_tree_rectangle<FT, D>& kdbox = container.bounding_box();
    for(int i = 0; i < d; i++) {
      min.push_back(kdbox.min_coord(i));
      max.push_back(kdbox.max_coord(i));
    }
    return construct_iso_box_d(construct_point_d(d, min.begin(), min.end(), 1),
                      construct_point_d(d, max.begin(), max.end(), 1));
  }
private:
  typename Traits::Construct_iso_box_d construct_iso_box_d;
  typename Traits::Construct_point_d construct_point_d;
};

template <class Traits>
class Construct_bounding_box_<Traits, Homogeneous_tag> {
public:
  typedef typename Traits::Iso_box_d                                 Iso_box_d;
  typedef typename Traits::FT                                        FT;
  typedef typename internal::Get_dimension_tag<Traits>::Dimension    D;
  typedef CGAL::Point_container<Traits>                              Point_container;
  typedef CGAL::Fraction_traits<FT>                                  FractionTraits;
  typedef typename FractionTraits::Numerator_type                    Numerator_type;
  typedef typename FractionTraits::Denominator_type                  Denominator_type;
  typedef typename FractionTraits::Decompose                         Decompose;

  Iso_box_d operator()(int d, const Point_container& container) const {
    std::vector<FT> minFT;
    std::vector<FT> maxFT;
    const Kd_tree_rectangle<FT, D>& kdbox = container.bounding_box();
    for(int i = 0; i < d; i++) {
      minFT.push_back(kdbox.min_coord(i));
      maxFT.push_back(kdbox.max_coord(i));
    }

    std::vector<Numerator_type> minNum(d, 0);
    std::vector<Numerator_type> maxNum(d, 0);
    std::vector<Denominator_type> minDen(d, 0);
    std::vector<Denominator_type> maxDen(d, 0);
    Denominator_type minDvalue = 1;
    Denominator_type maxDvalue = 1;
    Decompose decompose;
    for(int i = 0; i < d; i++) {
      decompose(minFT[i], minNum[i], minDen[i]);
      decompose(maxFT[i], maxNum[i], maxDen[i]);
      minDvalue *= minDen[i];
      maxDvalue *= maxDen[i];
    }
    std::vector<Denominator_type> maxMultipleD(d, 1);
    std::vector<Denominator_type> minMultipleD(d, 1);
    for(int i = 0; i < d; i++) {
      for(int j = 0; j < d; j++) {
        if(i != j) {
          minMultipleD[i] *= minDen[j];
          maxMultipleD[i] *= maxDen[j];
        }
      }
    }
    for(int i = 0; i < d; i++) {
      minNum[i] *= minMultipleD[i];
      maxNum[i] *= maxMultipleD[i];
    }

    return construct_iso_box_d(construct_point_d(d, minNum.begin(), minNum.end(), minDvalue),
                      construct_point_d(d, maxNum.begin(), maxNum.end(), maxDvalue));
  }
private:
  typename Traits::Construct_iso_box_d construct_iso_box_d;
  typename Traits::Construct_point_d construct_point_d;
};


template <class Traits>
class Construct_bounding_box : public Construct_bounding_box_<Traits, typename Traits::K::Rep_tag> { };


template <class Traits>
class Split_tree_node {
public:
  typedef Split_tree_node<Traits>                                    Node;
  typedef const Node*                                                Node_const_handle;
  typedef typename Traits::Point_d                                   Point_d;
  typedef typename Traits::Iso_box_d                                 Iso_box_d;
  typedef typename Traits::Sphere_d                                  Sphere_d;
  typedef typename Traits::Vector_d                                  Vector_d;
  typedef CGAL::Point_container<Traits>                              Point_container;

  struct Point_iterator_function {
    typedef const Point_d* argument_type;
    typedef const Point_d& result_type;
    result_type operator() (argument_type point_ptr) const {return *point_ptr; }
  };
  typedef boost::transform_iterator<Point_iterator_function, typename Point_container::const_iterator> Point_iterator;

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

  inline Node_const_handle left() const { return left_child;}
  inline Node_const_handle right() const { return right_child; }
  inline bool is_leaf() const { return left_child == NULL || right_child == NULL; }

  Iso_box_d bounding_box() const {
    if(!bbox_computed) {
      bbox = construct_bounding_box(d, container);
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

  Point_iterator points_begin() const {
    return Point_iterator(container.begin(), Point_iterator_function());
  }

  Point_iterator points_end() const {
    return Point_iterator(container.end(), Point_iterator_function());
  }

  int points_size() const {
    return container.size();
  }

  bool is_well_separated_with(Node_const_handle w, FT s) const  {
    FT max_rad =  std::max(this->squared_radius(), w->squared_radius());
    FT distance_vw = CGAL::squared_distance(this->center(), w->center());
    return distance_vw >= (s+2)*(s+2)*max_rad;
  }

  bool has_longuer_side_than(Node_const_handle w) const {
    Iso_box_d rect_v = this->bounding_box();
    Iso_box_d rect_w = w->bounding_box();
    Vector_d vector_v = rect_v.max() - rect_v.min();
    Vector_d vector_w = rect_w.max() - rect_w.min();

    FT max_v = *std::max_element(vector_v.cartesian_begin(), vector_v.cartesian_end());
    FT max_w = *std::max_element(vector_w.cartesian_begin(), vector_w.cartesian_end());
    return max_v > max_w;
  }
private:
  Traits traits;
  Node_const_handle left_child;
  Node_const_handle right_child;
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

  typename Traits::Construct_sphere_d construct_sphere_d;
  Construct_bounding_box<Traits> construct_bounding_box;
};

} // End namespace
#endif // CGAL_SPLIT_TREE_NODE_H
