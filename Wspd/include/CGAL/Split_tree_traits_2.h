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

#ifndef CGAL_SPLIT_TREE_TRAITS_2_H
#define CGAL_SPLIT_TREE_TRAITS_2_H
#include <CGAL/Search_traits_2.h>

namespace CGAL {

  template <class Kernel >
  class Split_tree_traits_2 : public Search_traits_2<Kernel> {
  public:
    typedef Kernel K;
    typedef typename Kernel::Vector_2 Vector_d;

  private:
    template <class K>
    class Construct_sphere {
    public:
      typedef typename K::Circle_2 Circle_2;
      typedef typename K::Point_2 Point_2;
      typedef typename K::FT FT;
      Circle_2 operator()(int d, const Point_2& a, const Point_2& b) const {
        CGAL_assertion(d == 2);
        Point_2 center = CGAL::midpoint(a, b);
        FT squared_radius = CGAL::squared_distance(center, b);
        return Circle_2(center, squared_radius);
      }
    };

    template <class K>
    class Construct_point {
    public:
      typedef typename K::Point_2 Point_2;
      typedef typename K::FT FT;
      template<class InputIterator>
      Point_2 operator()(int d, InputIterator first, InputIterator last) const {
        CGAL_assertion(d == 2);
        FT x = *first++;
        FT y = *first++;
        return Point_2(x, y);
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
#endif // CGAL_SPLIT_TREE_TRAITS_2_H
