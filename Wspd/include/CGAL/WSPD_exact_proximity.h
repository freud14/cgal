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

#ifndef CGAL_WSPD_EXACT_PROXIMITY_H
#define CGAL_WSPD_EXACT_PROXIMITY_H
#include <CGAL/WSPD.h>
#include <CGAL/Random.h>
#include <algorithm>
#include <vector>

namespace CGAL {

template <class Traits>
std::pair<typename Traits::Point_d, typename Traits::Point_d> wspd_closest_pair(const CGAL::WSPD<Traits>& wspd) {
  typedef typename Traits::Point_d                                   Point_d;
  typedef typename Traits::FT                                        FT;
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Well_separated_pair                         Well_separated_pair;
  typedef typename WSPD::Well_separated_pair_iterator                Well_separated_pair_iterator;

  CGAL_assertion(wspd.separation_ratio() > 2);
  CGAL_assertion(wspd.points_size() >= 2);

  std::pair<Point_d, Point_d> ret;
  FT min_squared_distance = -1;
  for(Well_separated_pair_iterator it = wspd.wspd_begin(); it != wspd.wspd_end(); it++) {
    Well_separated_pair pair = *it;
    if(pair.a()->points_size() == 1 && pair.b()->points_size() == 1) {
      Point_d a = *pair.a()->points_begin();
      Point_d b = *pair.b()->points_begin();
      FT new_distance = CGAL::squared_distance(a, b);
      if(min_squared_distance == -1 || new_distance < min_squared_distance) {
        ret = std::make_pair(a, b);
        min_squared_distance = new_distance;
      }
    }
  }
  return ret;
}

template <class Traits, class InputIterator>
std::pair<typename Traits::Point_d, typename Traits::Point_d> wspd_closest_pair(int d, InputIterator begin, InputIterator end) {
  return wspd_closest_pair<Traits>(CGAL::WSPD<Traits>(d, 3, begin, end));
}

template <class RandomAccessIterator, class Compare, class Weight, class WeightOf>
RandomAccessIterator weighted_quickselect(RandomAccessIterator first, RandomAccessIterator last, Compare comp, Weight init, Weight weight, WeightOf weight_of) {
  CGAL::Random rand;
  RandomAccessIterator current_first = first;
  RandomAccessIterator current_last = last;
  Weight current_final_weight = init;
  while(current_first != last && current_first != current_last) {
    RandomAccessIterator nth = current_first + rand(current_last-current_first);
    std::nth_element(current_first, nth, current_last, comp);
    Weight final_weight = init;
    for(RandomAccessIterator it = current_first; it < nth; it++) {
      final_weight = final_weight + weight_of(*it);
    }
    Weight new_weight = current_final_weight + final_weight;
    if(new_weight < weight) {
      if(new_weight + weight_of(*nth) < weight) {
        current_final_weight = new_weight + weight_of(*nth);
        current_first = nth + 1;
      }
      else {
        return nth;
      }
    }
    else {
      current_last = nth;
    }
  }
  return current_first;
}

template <class FT, class Iso_box_d>
FT squared_distance_box(Iso_box_d b1, Iso_box_d b2) {
  int d = b1.min().dimension();
  FT distance = 0;
  for(int i = 0; i < d; i++) {
    Iso_box_d left = b1.min()[i] < b2.min()[i] ? b1 : b2;
    Iso_box_d right = b1.min()[i] < b2.min()[i] ? b2 : b1;
    FT new_distance = right.min()[i] - left.max()[i];
    distance += new_distance < 0 ? 0 : new_distance*new_distance;
  }
  return distance;
}

template <class R>
typename R::FT squared_distance(Iso_rectangle_2<R> p, Iso_rectangle_2<R> q) {
  return squared_distance_box<typename R::FT>(p, q);
}

template <class R>
typename R::FT squared_distance(Iso_cuboid_3<R> p, Iso_cuboid_3<R> q) {
  return squared_distance_box<typename R::FT>(p, q);
}

template <class R>
typename R::FT squared_distance(Iso_box_d<R> p, Iso_box_d<R> q) {
  return squared_distance_box<typename R::FT>(p, q);
}

template <class Traits>
struct WSP_pred {
  typedef typename Traits::FT                                        FT;
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Well_separated_pair                         Well_separated_pair;

  WSP_pred(FT s_, FT squared_r_) : s(s_), squared_r(squared_r_) {}

  bool operator()(Well_separated_pair pair) {
    return squared_distance(pair.a()->bounding_box(), pair.b()->bounding_box()) <= (1+4/s)*(1+4/s)*squared_r;
  }
private:
  FT s;
  FT squared_r;
};

template <class Traits>
struct WSP_comp {
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Well_separated_pair                         Well_separated_pair;

  bool operator()(Well_separated_pair pair1, Well_separated_pair pair2) {
    return squared_distance(pair1.a()->bounding_box(), pair1.b()->bounding_box()) < squared_distance(pair2.a()->bounding_box(), pair2.b()->bounding_box());
  }
};

template <class Traits>
struct WSP_weight_of {
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Well_separated_pair                         Well_separated_pair;

  int operator()(Well_separated_pair pair) {
    return pair.a()->points_size() * pair.b()->points_size();
  }
};

template <class Traits>
struct Point_pair_comp {
  typedef typename Traits::Point_d Point_d;

  bool operator()(std::pair<Point_d, Point_d> pair1, std::pair<Point_d, Point_d> pair2) {
    return CGAL::squared_distance(pair1.first, pair1.second) < CGAL::squared_distance(pair2.first, pair2.second);
  }
};

template <class Traits, class OutputIterator>
void wspd_k_closest_pair(const CGAL::WSPD<Traits>& wspd, int k, OutputIterator result) {
  typedef typename Traits::Point_d                                   Point_d;
  typedef typename Traits::FT                                        FT;
  typedef typename Traits::Iso_box_d                                 Iso_box_d;
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Well_separated_pair                         Well_separated_pair;
  typedef typename WSPD::Well_separated_pair_decomposition           Well_separated_pair_decomposition;
  typedef typename WSPD::Well_separated_pair_iterator                Well_separated_pair_iterator;
  typedef typename WSPD::Node::Point_iterator                        Point_iterator;

  // Step 1 and 2
  Well_separated_pair_decomposition decomposition = wspd.wspd();
  Well_separated_pair pair = *weighted_quickselect(decomposition.begin(), decomposition.end(), WSP_comp<Traits>(), 0, k, WSP_weight_of<Traits>());
  FT min_squared_distance = squared_distance(pair.a()->bounding_box(), pair.b()->bounding_box());

  // Step 3
  Well_separated_pair_iterator last = std::partition(decomposition.begin(), decomposition.end(), WSP_pred<Traits>(wspd.separation_ratio(), min_squared_distance));

  // Step 4
  std::vector<std::pair<Point_d, Point_d> > candidate_pairs;
  for(Well_separated_pair_iterator it = decomposition.begin(); it != last; it++) {
    Well_separated_pair pair = *it;
    for(Point_iterator itA = pair.a()->points_begin(); itA != pair.a()->points_end(); itA++) {
      for(Point_iterator itB = pair.b()->points_begin(); itB != pair.b()->points_end(); itB++) {
        candidate_pairs.push_back(std::make_pair(*itA, *itB));
      }
    }
  }

  // Step 5
  std::nth_element(candidate_pairs.begin(), candidate_pairs.begin() + k, candidate_pairs.end(), Point_pair_comp<Traits>());
  for(int i = 0; i < k; i++) {
    *result++ = candidate_pairs[i];
  }
}

template <class Traits, class InputIterator, class OutputIterator>
void wspd_k_closest_pair(int d, InputIterator begin, InputIterator end, int k, OutputIterator result) {
  wspd_k_closest_pair<Traits>(CGAL::WSPD<Traits>(d, 1, begin, end), k, result);
}


template <class Traits>
typename Traits::FT wspd_squared_diameter_approximation(const CGAL::WSPD<Traits>& wspd) {
  typedef typename Traits::Point_d                                   Point_d;
  typedef typename Traits::FT                                        FT;
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Well_separated_pair                         Well_separated_pair;
  typedef typename WSPD::Well_separated_pair_iterator                Well_separated_pair_iterator;

  CGAL_assertion(wspd.points_size() >= 1);

  FT squared_diameter = 0;
  for(Well_separated_pair_iterator it = wspd.wspd_begin(); it != wspd.wspd_end(); it++) {
    Well_separated_pair pair = *it;
    FT new_squared_diameter = CGAL::squared_distance(*pair.a()->points_begin(), *pair.b()->points_begin());
    if(new_squared_diameter > squared_diameter) {
      squared_diameter = new_squared_diameter;
    }
  }
  FT s = wspd.separation_ratio();
  return (1+4/s)*(1+4/s)*squared_diameter;
}

template <class Traits, class InputIterator>
typename Traits::FT wspd_squared_diameter_approximation(int d, InputIterator begin, InputIterator end, typename Traits::FT epsilon) {
  CGAL_assertion(epsilon > 0);
  CGAL_assertion(epsilon < 1);
  return wspd_squared_diameter_approximation<Traits>(CGAL::WSPD<Traits>(d, 4.*(1. - epsilon)/epsilon, begin, end));
}

} // End namespace
#endif // CGAL_WSPD_EXACT_PROXIMITY_H
