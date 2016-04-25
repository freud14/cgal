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

#ifndef CGAL_WELL_SEPARATED_PAIR_H
#define CGAL_WELL_SEPARATED_PAIR_H
#include <CGAL/Split_tree.h>

namespace CGAL {

template <class Traits>
class Well_separated_pair {
public:
  typedef CGAL::Split_tree<Traits>                                   Split_tree;
  typedef typename Split_tree::Node_const_handle                     Node_const_handle;

  Well_separated_pair(Node_const_handle a, Node_const_handle b) : first(a), second(b) { }

  inline Node_const_handle a() const {
    return first;
  }

  inline Node_const_handle b() const {
    return second;
  }
private:
  Node_const_handle first;
  Node_const_handle second;
};

} // End namespace
#endif // CGAL_WELL_SEPARATED_PAIR_H
