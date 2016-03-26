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
    typedef typename K::Vector_3 Vector_d;
    typedef typename K::Aff_transformation_3 Aff_transformation_d;
    typedef typename K::RT RT;

    template <class K_>
    class Construct_sphere {
    public:
      typedef typename K_::Sphere_3 Sphere_3;
      typedef typename K_::Point_3 Point_3;
      template<class InputIterator>
      Sphere_3 operator()(int d, InputIterator first, InputIterator last) const {
        CGAL_assertion(d == 3);
        Point_3 p = *first++;
        Point_3 q = *first++;
        Point_3 r = *first++;
        Point_3 s = *first++;
        if(p != q && q != r && r != s) {
          CGAL_assertion(!CGAL::coplanar(p, q, r, s));
          return Sphere_3(p, q, r, s);
        }
        else {
          return Sphere_3(p);
        }

      }
    };

    template <class K_>
    class Construct_point {
    public:
      typedef typename K_::Point_3 Point_3;
      typedef typename K_::FT FT;
      template<class InputIterator>
      Point_3 operator()(int d, InputIterator first, InputIterator last) const {
        CGAL_assertion(d == 3);
        FT x = *first++;
        FT y = *first++;
        FT z = *first++;
        return Point_3(x, y, z);
      }
    };

    template <class K_>
    class Construct_aff_transformation {
    public:
      typedef typename K_::Aff_transformation_3 Aff_transformation_3;
      typedef typename K_::RT RT;
      Aff_transformation_3 operator()(int d, Rotation ro, RT sin_num, RT cos_num, RT den, int e1=0, int e2=1) const {
        CGAL_assertion(d == 3);
        CGAL_assertion(e1 < d);
        CGAL_assertion(e2 < d);
        RT M[3][3];
        for (int i = 0; i < 3; i++) {
          for (int j = 0; j < 3; j++) {
            M[i][j] = RT(0) ? i != j : RT(1);
          }
        }
        M[e1][e1] = cos_num/den; M[e1][e2] = -sin_num/den;
        M[e2][e1] = sin_num/den; M[e2][e2] = cos_num/den;
        return Aff_transformation_3(M[0][0], M[0][1], M[0][2], M[1][0], M[1][1], M[1][2], M[2][0], M[2][1], M[2][2]);
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
#endif // CGAL_SPLIT_TREE_TRAITS_3_H
