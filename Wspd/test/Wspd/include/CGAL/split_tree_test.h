#ifndef SPLIT_TREE_TEST_H
#define SPLIT_TREE_TEST_H

#include <cassert>
#include <iostream>
#include <CGAL/Split_tree.h>

#include <CGAL/wspd_test_util.h>

namespace CGAL {

template <class Traits>
bool
test_points(int d, const Traits& traits, const CGAL::Split_tree<Traits>& points_split_tree, const std::vector<typename Traits::Point_d>& points)
{
  typedef typename Traits::Iso_box_d Iso_box_d;
  typedef CGAL::Split_tree<Traits> Split_tree;
  typedef typename Split_tree::Node_const_handle Node_const_handle;

  std::vector<Iso_box_d> points_bboxes(points_split_tree.bounding_box_begin(), points_split_tree.bounding_box_end());
  Node_const_handle points_root = points_split_tree.root();
  assert(points_bboxes.size() == 13);
  assert(points_root->bounding_box() == Iso_box_d(get_point_d(d, 10, 30, 2, traits), get_point_d(d, 464, 411, 3, traits)));
  assert(points_root->right()->bounding_box() == Iso_box_d(get_point_d(d, 328, 30, 4,traits), get_point_d(d, 464, 367, 5, traits)));
  assert(points_root->left()->bounding_box() == Iso_box_d(points[6], points[4]));
  assert(points_root->left()->left()->bounding_box() == Iso_box_d(points[6], points[2]));
  assert(points_root->left()->left()->right()->bounding_box() == Iso_box_d(points[0], points[2]));
  assert(points_root->left()->left()->right()->right()->bounding_box() == Iso_box_d(points[1], points[2]));
}

template <class Traits>
bool
split_tree__batch_test(int d, const Traits& traits)
{
  /*
  Random homogeneous denominator have been put for the creation of the points to test the
  homogeneous kernels. The coordinates of a point are multiplied by the denominator before
  its creation so that it is clear what is the point in the Cartesian coordinate system.
  */
  typedef typename Traits::Point_d Point_d;
  typedef typename Traits::Iso_box_d Iso_box_d;
  typedef typename Traits::RT RT;
  typedef CGAL::Split_tree<Traits> Split_tree;
  typedef typename Split_tree::Node_const_handle Node_const_handle;

  Split_tree empty_split_tree(d);
  assert(empty_split_tree.root() == NULL);
  assert(empty_split_tree.bounding_box_begin() == empty_split_tree.bounding_box_end());

  Point_d single_point[] = {get_point_d(d, 1, 2, 2, traits)};
  Iso_box_d expected_single_rect(single_point[0], single_point[0]);
  Split_tree single_split_tree(d, single_point, single_point + 1);
  assert(single_split_tree.root() != NULL);
  assert(single_split_tree.root()->is_leaf());
  assert(single_split_tree.root()->left() == NULL);
  assert(single_split_tree.root()->right() == NULL);
  assert(*single_split_tree.bounding_box_begin() == expected_single_rect);
  assert(single_split_tree.bounding_box_begin() + 1 == single_split_tree.bounding_box_end());


  Point_d two_points[] = {get_point_d(d, 1, 2, 5, traits), get_point_d(d, 1, 4, 7, traits)};
  Split_tree two_points_split_tree(d, two_points, two_points + 2);
  std::vector<Iso_box_d> two_points_bboxes(two_points_split_tree.bounding_box_begin(), two_points_split_tree.bounding_box_end());
  Iso_box_d two_points_expected_root_bbox(two_points[0], two_points[1]);
  Iso_box_d two_points_expected_left_bbox(two_points[0], two_points[0]);
  Iso_box_d two_points_expected_right_bbox(two_points[1], two_points[1]);
  assert(two_points_split_tree.root() != NULL);
  assert(two_points_split_tree.root()->bounding_box() == two_points_expected_root_bbox);
  assert(two_points_split_tree.root()->left() != NULL);
  assert(two_points_split_tree.root()->left()->is_leaf());
  assert(two_points_split_tree.root()->left()->bounding_box() == two_points_expected_left_bbox);
  assert(two_points_split_tree.root()->right() != NULL);
  assert(two_points_split_tree.root()->right()->is_leaf());
  assert(two_points_split_tree.root()->right()->bounding_box() == two_points_expected_right_bbox);
  assert(two_points_bboxes.size() == 3);
  assert(in(two_points_bboxes, two_points_expected_root_bbox));
  assert(in(two_points_bboxes, two_points_expected_left_bbox));
  assert(in(two_points_bboxes, two_points_expected_right_bbox));

  Point_d three_points[] = {get_point_d(d, 5, 2, 1, traits), get_point_d(d, 4, 3, 3, traits), get_point_d(d, 1, 1, 5, traits)};
  Split_tree three_points_split_tree(d, three_points, three_points + 3);
  std::vector<Iso_box_d> three_points_bboxes(three_points_split_tree.bounding_box_begin(), three_points_split_tree.bounding_box_end());
  Iso_box_d three_points_expected_root_bbox(three_points[2], get_point_d(d, 5, 3, 4, traits));
  Iso_box_d three_points_expected_left_bbox(three_points[2], three_points[2]);
  Iso_box_d three_points_expected_right_bbox(get_point_d(d, 4, 2, 4, traits), get_point_d(d, 5, 3, 4, traits));
  assert(three_points_split_tree.root() != NULL);
  assert(three_points_split_tree.root()->bounding_box() == three_points_expected_root_bbox);
  assert(three_points_split_tree.root()->left() != NULL);
  assert(three_points_split_tree.root()->left()->is_leaf());
  assert(three_points_split_tree.root()->left()->bounding_box() == three_points_expected_left_bbox);
  assert(three_points_split_tree.root()->right() != NULL);
  assert(three_points_split_tree.root()->right()->is_leaf() == false);
  assert(three_points_split_tree.root()->right()->bounding_box() == three_points_expected_right_bbox);
  assert(three_points_bboxes.size() == 5);
  assert(in(three_points_bboxes, three_points_expected_root_bbox));
  assert(in(three_points_bboxes, three_points_expected_left_bbox));
  assert(in(three_points_bboxes, three_points_expected_right_bbox));

  Point_d four_points[] = {get_point_d(d, 5, 2, 3, traits), get_point_d(d, 4, 3, 4, traits), get_point_d(d, 1, 1, 5, traits), get_point_d(d, -1, -1, -8, traits)};
  Split_tree four_points_split_tree(d, four_points, four_points + 4);
  std::vector<Iso_box_d> four_points_bboxes(four_points_split_tree.bounding_box_begin(), four_points_split_tree.bounding_box_end());
  Iso_box_d four_points_expected_root_bbox(four_points[3], get_point_d(d, 5, 3, 3, traits));
  Iso_box_d four_points_expected_left_bbox(four_points[3], four_points[2]);
  Iso_box_d four_points_expected_right_bbox(get_point_d(d, 4, 2, 4, traits), get_point_d(d, 5, 3, 8, traits));
  assert(four_points_split_tree.root() != NULL);
  assert(four_points_split_tree.root()->bounding_box() == four_points_expected_root_bbox);
  assert(four_points_split_tree.root()->left() != NULL);
  assert(four_points_split_tree.root()->left()->is_leaf() == false);
  assert(four_points_split_tree.root()->left()->bounding_box() == four_points_expected_left_bbox);
  assert(four_points_split_tree.root()->right() != NULL);
  assert(four_points_split_tree.root()->right()->is_leaf() == false);
  assert(four_points_split_tree.root()->right()->bounding_box() == four_points_expected_right_bbox);
  assert(four_points_bboxes.size() == 7);
  assert(in(four_points_bboxes, four_points_expected_root_bbox));
  assert(in(four_points_bboxes, four_points_expected_left_bbox));
  assert(in(four_points_bboxes, four_points_expected_right_bbox));


  Point_d ambiguous_split_points[] = {get_point_d(d, 0, 0, 1, traits), get_point_d(d, 2, 0, 5, traits), get_point_d(d, 2, 1, 4, traits), get_point_d(d, 2, -1, 4, traits), get_point_d(d, 4, 0, 2, traits)};
  Split_tree ambiguous_split_tree(d, ambiguous_split_points, ambiguous_split_points + 5);
  std::vector<Iso_box_d> ambiguous_split_points_bboxes(ambiguous_split_tree.bounding_box_begin(), ambiguous_split_tree.bounding_box_end());
  assert(ambiguous_split_points_bboxes.size() == 9);
  Iso_box_d first_possibility_left_bbox(get_point_d(d, 0, 0, 4, traits), get_point_d(d, 0, 0, 3, traits));
  Iso_box_d first_possibility_right_bbox(get_point_d(d, 2, -1, 5, traits), get_point_d(d, 4, 1, 2, traits));
  Iso_box_d second_possibility_left_bbox(get_point_d(d, 0, -1, 10, traits), get_point_d(d, 2, 1, 2, traits));
  Iso_box_d second_possibility_right_bbox(get_point_d(d, 4, 0, 8, traits), get_point_d(d, 4, 0, 1, traits));
  assert((in(ambiguous_split_points_bboxes, first_possibility_left_bbox) && in(ambiguous_split_points_bboxes, first_possibility_right_bbox))
          || (in(ambiguous_split_points_bboxes, second_possibility_left_bbox) && in(ambiguous_split_points_bboxes, second_possibility_left_bbox)));


  // Construction of a linear split tree where each dimension in split once
  std::vector<Point_d> points_d;
  std::vector< std::vector<RT> > corners_coord(d, std::vector<RT>(d, 0));
  for(int i = 0; i < d; i++) {
    for(int j = i; j < d; j++) {
      corners_coord[j][i] = i;
    }

    std::vector<RT> coord(d, 0);
    coord[i] = i;
    points_d.push_back(traits.construct_point_d_object()(d, coord.begin(), coord.end(), 1));
  }
  std::vector<Point_d> corners;
  for(int i = 0; i < d; i++) {
    corners.push_back(traits.construct_point_d_object()(d, corners_coord[i].begin(), corners_coord[i].end(), 1));
  }
  Split_tree points_d_split_tree(d, points_d.begin(), points_d.end());
  std::vector<Iso_box_d> points_d_bboxes(points_d_split_tree.bounding_box_begin(), points_d_split_tree.bounding_box_end());
  Node_const_handle current_node = points_d_split_tree.root();
  for(int i = d-1; i >= 0; i--) {
    Iso_box_d points_d_bbox(get_point_d(d, 0, 0, traits), corners[i]);
    assert(in(points_d_bboxes, points_d_bbox));

    assert(current_node->bounding_box() == points_d_bbox);
    if(i != 0) {
      assert(current_node->right() != NULL);
      assert(current_node->right()->is_leaf());
    }
    else {
      assert(current_node->is_leaf());
    }
    current_node = current_node->left();
  }

  // Construction of a linear split tree where the same dimension is split again
  // and again.
  const int NB_POINTS_LINEAR_SPLIT = 12;
  std::vector<Point_d> points_linear_split;
  int current_coord = 1;
  for(int i = 0; i < NB_POINTS_LINEAR_SPLIT + 1; i++) {
    points_linear_split.push_back(get_point_d(d, 0, current_coord, traits));
    current_coord *= 2;
  }
  Split_tree points_linear_split_tree(d, points_linear_split.begin(), points_linear_split.end());
  std::vector<Iso_box_d> points_linear_bboxes(points_linear_split_tree.bounding_box_begin(), points_linear_split_tree.bounding_box_end());
  Node_const_handle points_linear_current_node = points_linear_split_tree.root();
  for(int i = NB_POINTS_LINEAR_SPLIT; i >= 0; i--) {
    Iso_box_d points_linear_bbox(points_linear_split[0], points_linear_split[i]);
    assert(in(points_linear_bboxes, points_linear_bbox));

    assert(points_linear_current_node->bounding_box() == points_linear_bbox);
    if(i != 0) {
      assert(points_linear_current_node->right() != NULL);
      assert(points_linear_current_node->right()->is_leaf());
    }
    else {
      assert(points_linear_current_node->is_leaf());
    }
    points_linear_current_node = points_linear_current_node->left();
  }


  std::vector<Point_d> points;
  std::ifstream input("data/points.txt");
  double x,y,D;
  while (input >> x >> y >> D) {
    points.push_back(get_point_d(d, x, y, D, traits));
  }
  Split_tree points_split_tree(d, points.begin(), points.end());
  test_points(d, traits, points_split_tree, points);

  Split_tree add_points_split_tree(d);
  add_points_split_tree.add(points.begin(), points.end());
  test_points(d, traits, add_points_split_tree, points);

  Split_tree set_points_split_tree(d);
  set_points_split_tree.set(d, points.begin(), points.end());
  test_points(d, traits, set_points_split_tree, points);

  Split_tree add_set_points_split_tree(d);
  add_set_points_split_tree.add(points.begin(), points.end());
  add_set_points_split_tree.compute();
  add_set_points_split_tree.set(d, points.begin(), points.end());
  test_points(d, traits, add_set_points_split_tree, points);

  Split_tree set_add_points_split_tree(d);
  set_add_points_split_tree.set(d, points.begin(), points.begin() + 2);
  set_add_points_split_tree.compute();
  set_add_points_split_tree.add(points.begin() + 2, points.end());
  test_points(d, traits, set_add_points_split_tree, points);
  set_add_points_split_tree.clear();
  set_add_points_split_tree.compute();
  set_add_points_split_tree.add(points.begin(), points.end());
  test_points(d, traits, set_add_points_split_tree, points);
  set_add_points_split_tree.clear();
  set_add_points_split_tree.compute();
  set_add_points_split_tree.set(d, points.begin(), points.end());
  test_points(d, traits, set_add_points_split_tree, points);
  set_add_points_split_tree.clear();
  set_add_points_split_tree.compute();
  set_add_points_split_tree.set(d, points.begin(), points.begin() + 2);
  set_add_points_split_tree.compute();
  set_add_points_split_tree.add(points.begin() + 2, points.end());
  test_points(d, traits, set_add_points_split_tree, points);

  std::cout << "Testing split tree...done" << std::endl;
  return true;
}

} //namespace CGAL

#endif // SPLIT_TREE_TEST_H
