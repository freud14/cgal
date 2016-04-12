#ifndef TEST_SPLIT_TREE_2
#define TEST_SPLIT_TREE_2

#include <cassert>
#include <iostream>
#include <CGAL/Split_tree.h>


namespace CGAL {

template <class Traits>
bool
in(const std::vector<typename Traits::Iso_box_d>& points, const typename Traits::Iso_box_d& p) {
  return std::find(points.begin(), points.end(), p) != points.end();
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
split_tree__batch_test(int d, const Traits& traits)
{
  typedef typename Traits::Point_d Point_d;
  typedef typename Traits::Iso_box_d Iso_box_d;
  typedef typename Traits::FT FT;
  typedef CGAL::Split_tree<Traits> Split_tree;
  typedef typename Split_tree::Node Node;

  Point_d zero_point = get_point_d(d, 0, 0, traits);
  Iso_box_d zero_rect(zero_point, zero_point);

  Split_tree empty_split_tree(d);
  assert(empty_split_tree.root() == NULL);
  assert(empty_split_tree.bounding_box_begin() == empty_split_tree.bounding_box_end());

  Point_d single_point[] = {get_point_d(d, 1, 2, traits)};
  Iso_box_d expected_single_rect(single_point[0], single_point[0]);
  Split_tree single_split_tree(d, single_point, single_point + 1);
  assert(single_split_tree.root() != NULL);
  assert(single_split_tree.root()->is_leaf());
  assert(single_split_tree.root()->left() == NULL);
  assert(single_split_tree.root()->right() == NULL);
  assert(*single_split_tree.bounding_box_begin() == expected_single_rect);
  assert(single_split_tree.bounding_box_begin() + 1 == single_split_tree.bounding_box_end());


  Point_d two_points[] = {get_point_d(d, 1, 2, traits), get_point_d(d, 1, 4, traits)};
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
  assert(in<Traits>(two_points_bboxes, two_points_expected_root_bbox));
  assert(in<Traits>(two_points_bboxes, two_points_expected_left_bbox));
  assert(in<Traits>(two_points_bboxes, two_points_expected_right_bbox));

  Point_d three_points[] = {get_point_d(d, 5, 2, traits), get_point_d(d, 4, 3, traits), get_point_d(d, 1, 1, traits)};
  Split_tree three_points_split_tree(d, three_points, three_points + 3);
  std::vector<Iso_box_d> three_points_bboxes(three_points_split_tree.bounding_box_begin(), three_points_split_tree.bounding_box_end());
  Iso_box_d three_points_expected_root_bbox(three_points[2], get_point_d(d, 5, 3, traits));
  Iso_box_d three_points_expected_left_bbox(three_points[2], three_points[2]);
  Iso_box_d three_points_expected_right_bbox(get_point_d(d, 4, 2, traits), get_point_d(d, 5, 3, traits));
  assert(three_points_split_tree.root() != NULL);
  assert(three_points_split_tree.root()->bounding_box() == three_points_expected_root_bbox);
  assert(three_points_split_tree.root()->left() != NULL);
  assert(three_points_split_tree.root()->left()->is_leaf());
  assert(three_points_split_tree.root()->left()->bounding_box() == three_points_expected_left_bbox);
  assert(three_points_split_tree.root()->right() != NULL);
  assert(three_points_split_tree.root()->right()->is_leaf() == false);
  assert(three_points_split_tree.root()->right()->bounding_box() == three_points_expected_right_bbox);
  assert(three_points_bboxes.size() == 5);
  assert(in<Traits>(three_points_bboxes, three_points_expected_root_bbox));
  assert(in<Traits>(three_points_bboxes, three_points_expected_left_bbox));
  assert(in<Traits>(three_points_bboxes, three_points_expected_right_bbox));

  Point_d four_points[] = {get_point_d(d, 5, 2, traits), get_point_d(d, 4, 3, traits), get_point_d(d, 1, 1, traits), get_point_d(d, -1, -1, traits)};
  Split_tree four_points_split_tree(d, four_points, four_points + 4);
  std::vector<Iso_box_d> four_points_bboxes(four_points_split_tree.bounding_box_begin(), four_points_split_tree.bounding_box_end());
  Iso_box_d four_points_expected_root_bbox(four_points[3], get_point_d(d, 5, 3, traits));
  Iso_box_d four_points_expected_left_bbox(four_points[3], four_points[2]);
  Iso_box_d four_points_expected_right_bbox(get_point_d(d, 4, 2, traits), get_point_d(d, 5, 3, traits));
  assert(four_points_split_tree.root() != NULL);
  assert(four_points_split_tree.root()->bounding_box() == four_points_expected_root_bbox);
  assert(four_points_split_tree.root()->left() != NULL);
  assert(four_points_split_tree.root()->left()->is_leaf() == false);
  assert(four_points_split_tree.root()->left()->bounding_box() == four_points_expected_left_bbox);
  assert(four_points_split_tree.root()->right() != NULL);
  assert(four_points_split_tree.root()->right()->is_leaf() == false);
  assert(four_points_split_tree.root()->right()->bounding_box() == four_points_expected_right_bbox);
  assert(four_points_bboxes.size() == 7);
  assert(in<Traits>(four_points_bboxes, four_points_expected_root_bbox));
  assert(in<Traits>(four_points_bboxes, four_points_expected_left_bbox));
  assert(in<Traits>(four_points_bboxes, four_points_expected_right_bbox));


  Point_d ambiguous_split_points[] = {get_point_d(d, 0, 0, traits), get_point_d(d, 2, 0, traits), get_point_d(d, 2, 1, traits), get_point_d(d, 2, -1, traits), get_point_d(d, 4, 0, traits)};
  Split_tree ambiguous_split_tree(d, ambiguous_split_points, ambiguous_split_points + 5);
  std::vector<Iso_box_d> ambiguous_split_points_bboxes(ambiguous_split_tree.bounding_box_begin(), ambiguous_split_tree.bounding_box_end());
  assert(ambiguous_split_points_bboxes.size() == 9);
  Iso_box_d first_possibility_left_bbox(get_point_d(d, 0, 0, traits), get_point_d(d, 0, 0, traits));
  Iso_box_d first_possibility_right_bbox(get_point_d(d, 2, -1, traits), get_point_d(d, 4, 1, traits));
  Iso_box_d second_possibility_left_bbox(get_point_d(d, 0, -1, traits), get_point_d(d, 2, 1, traits));
  Iso_box_d second_possibility_right_bbox(get_point_d(d, 4, 0, traits), get_point_d(d, 4, 0, traits));
  assert((in<Traits>(ambiguous_split_points_bboxes, first_possibility_left_bbox) && in<Traits>(ambiguous_split_points_bboxes, first_possibility_right_bbox))
          || (in<Traits>(ambiguous_split_points_bboxes, second_possibility_left_bbox) && in<Traits>(ambiguous_split_points_bboxes, second_possibility_left_bbox)));


  // Construction of a linear split tree where each dimension in split once
  std::vector<Point_d> points_d;
  std::vector< std::vector<FT> > corners_coord(d, std::vector<FT>(d, 0));
  for(int i = 0; i < d; i++) {
    for(int j = i; j < d; j++) {
      corners_coord[j][i] = i;
    }

    std::vector<FT> coord(d, 0);
    coord[i] = i;
    points_d.push_back(traits.construct_point_d_object()(d, coord.begin(), coord.end()));
  }
  std::vector<Point_d> corners;
  for(int i = 0; i < d; i++) {
    corners.push_back(traits.construct_point_d_object()(d, corners_coord[i].begin(), corners_coord[i].end()));
  }
  Split_tree points_d_split_tree(d, points_d.begin(), points_d.end());
  std::vector<Iso_box_d> points_d_bboxes(points_d_split_tree.bounding_box_begin(), points_d_split_tree.bounding_box_end());
  const Node* current_node = points_d_split_tree.root();
  for(int i = d-1; i >= 0; i--) {
    Iso_box_d points_d_bbox(get_point_d(d, 0, 0, traits), corners[i]);
    assert(in<Traits>(points_d_bboxes, points_d_bbox));

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

  std::cout << "Testing split tree...done" << std::endl;
  return true;
}

} //namespace CGAL

#endif // TEST_SPLIT_TREE_2
