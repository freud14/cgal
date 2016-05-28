#ifndef WSPD_EXACT_PROXIMITY_TEST_H
#define WSPD_EXACT_PROXIMITY_TEST_H

#include <cassert>
#include <iostream>
#include <algorithm>

#include <CGAL/WSPD_exact_proximity.h>

#include <CGAL/wspd_test_util.h>

namespace CGAL {

template <class T>
bool unordered_pair_equal(std::pair<T, T> pair1, std::pair<T, T> pair2) {
  return (pair1.first == pair2.first && pair1.second == pair2.second) ||
          (pair1.first == pair2.second && pair1.second == pair2.first);
}
template <class Traits>
struct Pair_distance_cmp {
  typedef typename Traits::Point_d Point_d;

  bool operator()(std::pair<Point_d, Point_d> pair1, std::pair<Point_d, Point_d> pair2) {
    return CGAL::squared_distance(pair1.first, pair1.second) < CGAL::squared_distance(pair2.first, pair2.second);
  }
};

template <class Traits>
bool
wspd_exact_proximity__batch_test(int d, const Traits& traits)
{
  /*
  Random homogeneous denominator have been put for the creation of the points to test the
  homogeneous kernels. The coordinates of a point are multiplied by the denominator before
  its creation so that it is clear what is the point in the Cartesian coordinate system.
  */

  typedef typename Traits::RT                                        RT;
  typedef typename Traits::Point_d                                   Point_d;
  typedef typename Traits::Iso_box_d                                 Iso_box_d;

  typedef std::vector<Point_d>                                       Point_vector;
  typedef typename Point_vector::iterator                            Point_iterator;

  Point_vector points;
  std::ifstream input("data/closest_pair.txt");
  double x,y,D;
  while (input >> x >> y >> D) {
    points.push_back(get_point_d(d, x, y, D, traits));
  }

  std::vector<std::pair<Point_d, Point_d> > expected_ordered_pair;
  for(int i = 0; i < points.size(); i++) {
    for(int j = i + 1; j < points.size(); j++) {
      expected_ordered_pair.push_back(std::make_pair(points[i], points[j]));
    }
  }
  std::sort(expected_ordered_pair.begin(), expected_ordered_pair.end(), Pair_distance_cmp<Traits>());

  std::pair<Point_d, Point_d> closest_pair = wspd_closest_pair<Traits>(d, points.begin(), points.end());
  assert(unordered_pair_equal(closest_pair, expected_ordered_pair[0]));

  for(int k = 1; k <= expected_ordered_pair.size(); k++) {
    std::vector<std::pair<Point_d, Point_d> > result;
    wspd_k_closest_pair<Traits>(d, points.begin(), points.end(), k, std::back_inserter(result));
    assert(result.size() == k);
    std::sort(result.begin(), result.end(), Pair_distance_cmp<Traits>());
    assert(std::equal(result.begin(), result.end(), expected_ordered_pair.begin(), unordered_pair_equal<Point_d>));
  }

  std::cout << "Testing WSPD exact proximity algorithms...done" << std::endl;
  return true;
}

} //namespace CGAL

#endif // WSPD_EXACT_PROXIMITY_TEST_H
