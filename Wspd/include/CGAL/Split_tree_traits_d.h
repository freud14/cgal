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

#ifndef CGAL_SPLIT_TREE_TRAITS_D_H
#define CGAL_SPLIT_TREE_TRAITS_D_H
#include <CGAL/Search_traits_d.h>
#include <CGAL/constructions_d.h>

namespace CGAL {

  template <class Kernel >
  class Split_tree_traits_d : public Search_traits_d<Kernel> {
  public:
    typedef Kernel K;
    typedef typename Kernel::Vector_d Vector_d;
    typedef typename Kernel::Sphere_d Sphere_d;

  private:
    template <class K>
    class Construct_sphere {
    public:
      typedef typename K::Sphere_d Sphere_d;
      typedef typename K::Point_d Point_d;
      typedef typename K::Vector_d Vector_d;
      typedef typename K::Aff_transformation_d Aff_transformation_d;
      typedef typename K::FT FT;
      typedef typename K::RT RT;
      Sphere_d operator()(int d, const Point_d& a, const Point_d& b) const {
        std::vector<Point_d> points_on_sphere;
        points_on_sphere.reserve(d + 1);
        if(a == b) {
          points_on_sphere.assign(d + 1, a);
        }
        else {
          Point_d center = CGAL::midpoint(a, b);
          Vector_d radius_vector = b - center;
          int non_zero_coord = -1;
          for(int i = 0; i < d; i++) {
            if(radius_vector[i] != FT(0)) {
              non_zero_coord = i;
              break;
            }
          }
          points_on_sphere.push_back(a);
          points_on_sphere.push_back(b);
          for(int i = 0; i < d; i++) {
            if(i == non_zero_coord) continue;
            int e1 = i < non_zero_coord ? i : non_zero_coord;
            int e2 = i < non_zero_coord ? non_zero_coord : i;
            Point_d point_on_sphere = center + radius_vector.transform(Aff_transformation_d(d, Rotation(), RT(1), RT(0), RT(1), e1, e2));
            points_on_sphere.push_back(point_on_sphere);
          }
          Sphere_d circle(d, points_on_sphere.begin(), points_on_sphere.end());
        }
        return Sphere_d(d, points_on_sphere.begin(), points_on_sphere.end());
      }
    };

    template <class K>
    class Construct_point {
    public:
      typedef typename K::Point_d Point_d;
      typedef typename K::FT FT;
      template<class InputIterator>
      Point_d operator()(int d, InputIterator first, InputIterator last) const {
        return Point_d(d, first, last);
      }
    };
  public:
    typedef Construct_sphere<Kernel> Construct_sphere_d;
    typedef Construct_point<Kernel> Construct_point_d;

    Construct_sphere_d construct_sphere_d_object() const {
      return Construct_sphere_d();
    }

    Construct_point_d construct_point_d_object() const {
      return Construct_point_d();
    }
  };

} // namespace CGAL
#endif // CGAL_SPLIT_TREE_TRAITS_D_H
