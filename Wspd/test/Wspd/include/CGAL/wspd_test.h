#ifndef WSPD_TEST
#define WSPD_TEST

#include <cassert>
#include <iostream>
#include <CGAL/WSPD.h>


namespace CGAL {

template <class Traits>
bool
in(typename Split_tree<Traits>::Node_const_handle node, const typename Traits::Point_d& p) {
  typedef typename Split_tree<Traits>::Point_container Point_container;
  for(typename Point_container::const_iterator it = node->point_container().begin(); it != node->point_container().end(); it++) {
    if(**it == p) {
      return true;
    }
  }
  return false;
}

template <class Traits>
bool
is_pair_in_wspd(const CGAL::WSPD<Traits>& wspd, typename CGAL::WSPD<Traits>::Node_const_handle a, typename CGAL::WSPD<Traits>::Node_const_handle b) {
  typedef CGAL::WSPD<Traits> WSPD;
  typedef typename WSPD::Well_separated_pair Well_separated_pair;
  typedef typename WSPD::Well_separated_pair_iterator Well_separated_pair_iterator;

  for(Well_separated_pair_iterator it = wspd.wspd_begin(); it < wspd.wspd_end(); it++) {
    const Well_separated_pair &pair = *it;
    if((pair.a() == a && pair.b() == b) || (pair.a() == b && pair.b() == a)) {
      return true;
    }
  }
  return false;
}


template <class Traits>
typename Traits::Point_d
get_point_d(int d, typename Traits::FT a, typename Traits::FT b, const Traits& traits) {
  std::vector<typename Traits::FT> coord(d, 0);
  coord[d - 2] = a;
  coord[d - 1] = b;
  return traits.construct_point_d_object()(d, coord.begin(), coord.end());
}

template <class Traits>
bool
test_points(int d, const Traits& traits, const CGAL::WSPD<Traits>& points_wspd)
{
  typedef typename CGAL::WSPD<Traits>::Node_const_handle Node_const_handle;

  Node_const_handle points_root = points_wspd.split_tree().root();
  assert(points_wspd.size() == 12);
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->right()->left(), points_root->right()->right()));
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->right()->right(), points_root->left()->right()));
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->left(), points_root->right()->left()));
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->left()->left()->left(), points_root->right()->right()));
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->left()->left()->right(), points_root->right()->right()));
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->left()->right(), points_root->right()->right()));
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->left()->left()->left(), points_root->left()->right()));
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->left()->left()->right()->left(), points_root->left()->right()));
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->left()->left()->right()->right(), points_root->left()->right()));
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->left()->left()->left(), points_root->left()->left()->right()->left()));
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->left()->left()->right()->left(), points_root->left()->left()->right()->right()));
  assert(is_pair_in_wspd<Traits>(points_wspd, points_root->left()->left()->right()->right()->left(), points_root->left()->left()->right()->right()->right()));
}

/*!
  Assumptions on the way the algorithm works are done in these tests (e.g.
  left node is visited before right node in computeWspd).
*/
template <class Traits>
bool
wspd__batch_test(int d, const Traits& traits)
{
  typedef typename Traits::Point_d Point_d;
  typedef CGAL::WSPD<Traits> WSPD;
  typedef typename WSPD::Well_separated_pair Well_separated_pair;

  WSPD empty_wspd(d, 2.0);
  assert(empty_wspd.size() == 0);
  assert(empty_wspd.wspd_begin() == empty_wspd.wspd_end());
  assert(empty_wspd.points_begin() == empty_wspd.points_end());

  Point_d single_point[] = {get_point_d(d, 1, 2, traits)};
  WSPD single_wspd(d, 2.0, single_point, single_point + 1);
  assert(single_wspd.size() == 0);
  assert(single_wspd.wspd_begin() == single_wspd.wspd_end());
  assert(*single_wspd.points_begin() == single_point[0]);
  assert(single_wspd.points_begin() + 1 == single_wspd.points_end());

  Point_d two_points[] = {get_point_d(d, 1, 2, traits), get_point_d(d, 1, 4, traits)};
  WSPD two_points_wspd(d, 2.0, two_points, two_points + 2);
  std::vector<Well_separated_pair> two_points_pairs(two_points_wspd.wspd_begin(), two_points_wspd.wspd_end());
  assert(two_points_wspd.size() == 1);
  assert(two_points_pairs.size() == 1);
  assert(in<Traits>(two_points_pairs[0].a(), two_points[0]));
  assert(in<Traits>(two_points_pairs[0].b(), two_points[1]));

  Point_d three_points[] = {get_point_d(d, 0, 2, traits), get_point_d(d, 0, 0, traits), get_point_d(d, 0, 5, traits)};
  WSPD three_points_wspd(d, 2.0, three_points, three_points + 3);
  std::vector<Well_separated_pair> three_points_pairs(three_points_wspd.wspd_begin(), three_points_wspd.wspd_end());
  assert(three_points_wspd.size() == 2);
  assert(three_points_pairs.size() == 2);
  assert(in<Traits>(three_points_pairs[0].b(), three_points[2]));
  assert(in<Traits>(three_points_pairs[0].a(), three_points[0]));
  assert(in<Traits>(three_points_pairs[0].a(), three_points[1]));
  assert(in<Traits>(three_points_pairs[1].a(), three_points[1]) && in<Traits>(three_points_pairs[1].b(), three_points[0]));

  Point_d four_points[] = {get_point_d(d, 5, 2, traits), get_point_d(d, 4, 3, traits), get_point_d(d, 0, 1, traits), get_point_d(d, -1, -1, traits)};
  WSPD four_points_wspd(d, 2.0, four_points, four_points + 4);
  std::vector<Well_separated_pair> four_points_pairs(four_points_wspd.wspd_begin(), four_points_wspd.wspd_end());
  assert(four_points_wspd.size() == 3);
  assert(four_points_pairs.size() == 3);
  assert(in<Traits>(four_points_pairs[0].a(), four_points[2]));
  assert(in<Traits>(four_points_pairs[0].a(), four_points[3]));
  assert(in<Traits>(four_points_pairs[0].b(), four_points[0]));
  assert(in<Traits>(four_points_pairs[0].b(), four_points[1]));
  assert((in<Traits>(four_points_pairs[1].a(), four_points[2]) && in<Traits>(four_points_pairs[1].b(), four_points[3])) ||
          (in<Traits>(four_points_pairs[1].a(), four_points[3]) && in<Traits>(four_points_pairs[1].b(), four_points[2])));
  assert((in<Traits>(four_points_pairs[2].a(), four_points[0]) && in<Traits>(four_points_pairs[2].b(), four_points[1])) ||
          (in<Traits>(four_points_pairs[2].a(), four_points[1]) && in<Traits>(four_points_pairs[2].b(), four_points[0])));

  std::vector<Point_d> points;
  std::ifstream input("data/points.txt");
  double x,y;
  while (input >> x >> y) {
    points.push_back(get_point_d(d, x, y, traits));
  }

  WSPD points_wspd(d, 1.4, points.begin(), points.end());
  test_points(d, traits, points_wspd);

  WSPD ratio_points_wspd(d, 2.0, points.begin(), points.end());
  ratio_points_wspd.compute();
  ratio_points_wspd.separation_ratio(1.4);
  test_points(d, traits, points_wspd);
  ratio_points_wspd.separation_ratio(1.3);
  test_points(d, traits, points_wspd);

  WSPD add_points_wspd(d, 1.4);
  add_points_wspd.add(points.begin(), points.end());
  test_points(d, traits, add_points_wspd);

  WSPD set_points_wspd(d, 1.4);
  set_points_wspd.set(d, points.begin(), points.end());
  test_points(d, traits, set_points_wspd);

  WSPD add_set_points_wspd(d, 1.4);
  add_set_points_wspd.add(points.begin(), points.end());
  add_set_points_wspd.compute();
  add_set_points_wspd.set(d, points.begin(), points.end());
  test_points(d, traits, add_set_points_wspd);

  WSPD set_add_points_wspd(d, 1.4);
  set_add_points_wspd.set(d, points.begin(), points.begin() + 2);
  set_add_points_wspd.add(points.begin() + 2, points.end());
  test_points(d, traits, set_add_points_wspd);
  set_add_points_wspd.clear();
  set_add_points_wspd.compute();
  set_add_points_wspd.add(points.begin(), points.end());
  test_points(d, traits, set_add_points_wspd);
  set_add_points_wspd.clear();
  set_add_points_wspd.compute();
  set_add_points_wspd.set(d, points.begin(), points.end());
  test_points(d, traits, set_add_points_wspd);
  set_add_points_wspd.clear();
  set_add_points_wspd.compute();
  set_add_points_wspd.set(d, points.begin(), points.begin() + 2);
  set_add_points_wspd.compute();
  set_add_points_wspd.add(points.begin() + 2, points.end());
  test_points(d, traits, set_add_points_wspd);

  WSPD no_compute_set_add_points_wspd(d, 1.4);
  no_compute_set_add_points_wspd.set(d, points.begin(), points.begin() + 2);
  no_compute_set_add_points_wspd.add(points.begin() + 2, points.end());
  test_points(d, traits, set_add_points_wspd);
  no_compute_set_add_points_wspd.clear();
  no_compute_set_add_points_wspd.add(points.begin(), points.end());
  test_points(d, traits, set_add_points_wspd);
  no_compute_set_add_points_wspd.clear();
  no_compute_set_add_points_wspd.set(d, points.begin(), points.end());
  test_points(d, traits, set_add_points_wspd);
  no_compute_set_add_points_wspd.clear();
  no_compute_set_add_points_wspd.set(d, points.begin(), points.begin() + 2);
  no_compute_set_add_points_wspd.add(points.begin() + 2, points.end());
  test_points(d, traits, no_compute_set_add_points_wspd);

  std::cout << "Testing WSPD...done" << std::endl;
  return true;
}

} //namespace CGAL

#endif // WSPD_TEST
