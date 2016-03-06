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

  template <class K >
  class Split_tree_traits_2 : public Search_traits_2<K> {
  public:
    typedef typename K::Vector_2 Vector_d;
    typedef typename K::Aff_transformation_2 Aff_transformation_d;
    typedef typename K::RT RT;

    template <class K_>
    class Construct_sphere {
    public:
      typedef typename K_::Circle_2 Circle_2;
      typedef typename K_::Point_2 Point_2;
      template<class InputIterator>
      Circle_2 operator()(int d, InputIterator first, InputIterator last) const {
        CGAL_assertion(d == 2);
        Point_2 p1 = *first++;
        Point_2 p2 = *first++;
        Point_2 p3 = *first++;
        if(p1 != p2 && p2 != p3) {
          return Circle_2(p1, p2, p3);
        }
        else {
          return Circle_2(p1);
        }

      }
    };

    template <class K_>
    class Construct_point {
    public:
      typedef typename K_::Point_2 Point_2;
      typedef typename K_::FT FT;
      template<class InputIterator>
      Point_2 operator()(int d, InputIterator first, InputIterator last) const {
        CGAL_assertion(d == 2);
        FT x = *first++;
        FT y = *first++;
        return Point_2(x, y);
      }
    };

    template <class K_>
    class Construct_aff_transformation {
    public:
      typedef typename K_::Aff_transformation_2 Aff_transformation_2;
      typedef typename K_::RT RT;
      Aff_transformation_2 operator()(int d, Rotation ro, RT sin_num, RT cos_num, RT den, int e1=0, int e2=1) const {
        CGAL_assertion(d == 2);
        return Aff_transformation_2(ro, sin_num, cos_num);
      }
    };

    typedef Construct_sphere<K> Construct_sphere_d;
    Construct_sphere_d construct_sphere_d_object() const {
      return Construct_sphere_d();
    }

    typedef Construct_point<K> Construct_point_d;
    Construct_point_d construct_point_d_object() const {
      return Construct_point_d();
    }

    typedef Construct_aff_transformation<K> Construct_aff_transformation_d;
    Construct_aff_transformation_d construct_aff_transformation_d_object() const {
      return Construct_aff_transformation_d();
    }
  };

} // namespace CGAL
#endif // CGAL_SPLIT_TREE_TRAITS_2_H
