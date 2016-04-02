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

#ifndef CGAL_SPLIT_TREE_TRAITS_3_H
#define CGAL_SPLIT_TREE_TRAITS_3_H
#include <CGAL/Search_traits_3.h>

namespace CGAL {

  template <class Kernel >
  class Split_tree_traits_3 : public Search_traits_3<Kernel> {
  public:
    typedef Kernel K;
    typedef typename Kernel::Vector_3 Vector_d;

  private:
    template <class K>
    class Construct_sphere {
    public:
      typedef typename K::Sphere_3 Sphere_3;
      typedef typename K::Point_3 Point_3;
      typedef typename K::FT FT;
      Sphere_3 operator()(int d, const Point_3& a, const Point_3& b) const {
        CGAL_assertion(d == 3);
        Point_3 center = CGAL::midpoint(a, b);
        FT squared_radius = CGAL::squared_distance(center, b);
        return Sphere_3(center, squared_radius);
      }
    };

    template <class K>
    class Construct_point {
    public:
      typedef typename K::Point_3 Point_3;
      typedef typename K::FT FT;
      template<class InputIterator>
      Point_3 operator()(int d, InputIterator first, InputIterator last) const {
        CGAL_assertion(d == 3);
        FT x = *first++;
        FT y = *first++;
        FT z = *first++;
        return Point_3(x, y, z);
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
#endif // CGAL_SPLIT_TREE_TRAITS_3_H
