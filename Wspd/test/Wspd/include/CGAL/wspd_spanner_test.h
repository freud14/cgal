#ifndef WSPD_SPANNER_TEST_H
#define WSPD_SPANNER_TEST_H

#include <cassert>
#include <iostream>

#include <boost/graph/graph_traits.hpp>

#include <CGAL/WSPD.h>
#include <CGAL/WSPD_spanner.h>
#include <CGAL/boost/graph/graph_traits_WSPD_spanner.h>

#include <CGAL/wspd_test_util.h>

namespace CGAL {

template <class Traits, class R>
bool strictly_equal(const typename CGAL::WSPD_spanner<Traits,R>::Edge& e1, const typename CGAL::WSPD_spanner<Traits,R>::Edge& e2) {
  return (e1.from == e2.from && e1.to == e2.to && e1.node_from == e2.node_from && e1.node_to == e2.node_to);
}

template <class Traits, class R>
bool equal(const typename CGAL::WSPD_spanner<Traits,R>::Edge& e1, const typename CGAL::WSPD_spanner<Traits,R>::Edge& e2) {
  return strictly_equal<Traits,R>(e1, e2) ||
          (e1.from == e2.to && e1.to == e2.from && e1.node_from == e2.node_to && e1.node_to == e2.node_from);
}

template <class Traits>
class Boundary_representative_chooser {
public:
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Node_const_handle                           Node_const_handle;

  typedef typename Traits::Point_d                                   Point_d;
  typedef typename Traits::Iso_box_d                                 Iso_box_d;

  typedef std::vector<Point_d>                                       Point_vector;
  typedef typename Point_vector::iterator                            Point_iterator;

  Boundary_representative_chooser(int d_, Node_const_handle root) : d(d_) {
    if(root != NULL) {
      compute_tree_nodes_representatives(root);
    }
  }

  template <class OutputIterator>
  void operator()(Node_const_handle node, Node_const_handle other_node_in_pair, OutputIterator it) const {
    Point_vector ret = node_representatives[node];
    for(Point_iterator point_it = ret.begin(); point_it != ret.end(); point_it++) {
      *it = *point_it;
      ++it;
    }
  }
private:
  Point_vector compute_tree_nodes_representatives(Node_const_handle node) const {
    Point_vector current_boundary;
    if(node->is_leaf()) {
      Point_d p = *node->points_begin();
      current_boundary.push_back(p);
    }
    else {
      Point_vector left_boundary = compute_tree_nodes_representatives(node->left());
      Point_vector right_boundary = compute_tree_nodes_representatives(node->right());

      for(Point_iterator it = left_boundary.begin(); it != left_boundary.end(); it++) {
        if(has_on_boundary(node->bounding_box(), *it)) {
          current_boundary.push_back(*it);
        }
      }
      for(Point_iterator it = right_boundary.begin(); it != right_boundary.end(); it++) {
        if(has_on_boundary(node->bounding_box(), *it)) {
          current_boundary.push_back(*it);
        }
      }
    }
    node_representatives[node].assign(current_boundary.begin(), current_boundary.end());
    return current_boundary;
  }

  /*
    For the tests, we consider that the representatives of a bounding box are the points
    that their last two coordinates are on the boundary of the bounding box.
  */
  bool has_on_boundary(Iso_box_d bbox, Point_d p) const {
    return bbox.min()[d-1] == p[d-1] || bbox.max()[d-1] == p[d-1] || bbox.min()[d-2] == p[d-2] || bbox.max()[d-2] == p[d-2];
  }
private:
  mutable std::map<Node_const_handle, Point_vector> node_representatives;
  int d;
};

template <class Traits, class R>
void assert_spanner(const WSPD_spanner<Traits,R>& spanner, std::vector<typename Traits::Point_d> points, std::vector<typename WSPD_spanner<Traits,R>::Edge> expected_edges) {
  typedef typename Traits::Point_d Point_d;

  typedef CGAL::WSPD_spanner<Traits, R> WSPD_spanner;
  typedef typename WSPD_spanner::Edge Edge;
  typedef bool (*Edge_comparison)(const Edge&, const Edge&);
  Edge_comparison edge_equal = equal<Traits, R>;
  Edge_comparison edge_strict_equal = strictly_equal<Traits, R>;

  typedef boost::graph_traits<WSPD_spanner> graph_traits;
  typedef typename graph_traits::adjacency_iterator adjacency_iterator;
  typedef typename graph_traits::edge_iterator edge_iterator;
  typedef typename graph_traits::vertex_iterator vertex_iterator;
  typedef typename graph_traits::out_edge_iterator vertex_edge_iterator;

  typedef std::vector<Point_d> Point_vector;
  typedef typename Point_vector::iterator Point_iterator;
  typedef std::vector<Edge> Edge_vector;
  typedef typename Edge_vector::iterator Edge_iterator;

  assert(num_vertices(spanner) == points.size());
  std::pair<vertex_iterator, vertex_iterator> vertices_it = vertices(spanner);
  Point_vector vertices(vertices_it.first, vertices_it.second);
  assert(vertices.size() == points.size());
  assert(are_identical_set(vertices, points));

  assert(num_edges(spanner) == expected_edges.size());
  std::pair<edge_iterator, edge_iterator> edges_it = edges(spanner);
  Edge_vector edges(edges_it.first, edges_it.second);
  assert(edges.size() == expected_edges.size());
  assert(are_identical_set(edges, expected_edges, edge_equal));

  for(Point_iterator point_it = points.begin(); point_it != points.end(); point_it++) {
    Point_d p = *point_it;
    Edge_vector expected_vertex_edge;
    Point_vector expected_adjacent;
    for(Edge_iterator edge_it = expected_edges.begin(); edge_it != expected_edges.end(); edge_it++) {
      Edge e = *edge_it;
      if(e.from == p) {
        expected_vertex_edge.push_back(e);
        expected_adjacent.push_back(e.to);
      }
      else if(e.to == p) {
        // Swap of from and to from strict comparison of the edges
        expected_vertex_edge.push_back(Edge(e.to, e.from, e.node_to, e.node_from));
        expected_adjacent.push_back(e.from);
      }
    }
    assert(degree(p, spanner) == expected_vertex_edge.size());
    std::pair<vertex_edge_iterator, vertex_edge_iterator> v_edges_it = out_edges(p, spanner);
    Edge_vector vertex_edge(v_edges_it.first, v_edges_it.second);
    assert(vertex_edge.size() == expected_vertex_edge.size());
    assert(are_identical_set(vertex_edge, expected_vertex_edge, edge_strict_equal));

    assert(degree(p, spanner) == expected_adjacent.size());
    std::pair<adjacency_iterator, adjacency_iterator> adj_it = adjacent_vertices(p, spanner);
    Point_vector adjacent(adj_it.first, adj_it.second);
    assert(adjacent.size() == expected_adjacent.size());
    assert(are_identical_set(adjacent, expected_adjacent));
  }
}

/*!
  Assumptions on the way the algorithm works are done in these tests (e.g.
  left node is visited before right node in computeWspd).
*/
template <class Traits>
bool
wspd_spanner__batch_test(int d, const Traits& traits)
{
  /*
  Random homogeneous denominator have been put for the creation of the points to test the
  homogeneous kernels. The coordinates of a point are multiplied by the denominator before
  its creation so that it is clear what is the point in the Cartesian coordinate system.
  */
  typedef typename Traits::Point_d Point_d;
  typedef CGAL::WSPD<Traits> WSPD;
  typedef typename WSPD::Node_const_handle Node_const_handle;
  typedef Boundary_representative_chooser<Traits> R;

  typedef CGAL::WSPD_spanner<Traits> WSPD_spanner_default;
  typedef CGAL::WSPD_spanner<Traits, R> WSPD_spanner;
  typedef typename WSPD_spanner::Edge Edge;

  typedef std::vector<Point_d> Point_vector;
  typedef std::vector<Edge> Edge_vector;

  WSPD empty_wspd(d, 2.0);
  R empty_r(d, empty_wspd.split_tree().root());
  WSPD_spanner empty_spanner(empty_wspd, empty_r);
  assert_spanner(empty_spanner, Point_vector(), Edge_vector());


  Point_d single_point[] = {get_point_d(d, 1, 2, 2, traits)};
  Point_vector single_point_vec(single_point, single_point + 1);
  WSPD single_wspd(d, 2.0, single_point_vec.begin(), single_point_vec.end());
  R single_r(d, single_wspd.split_tree().root());
  WSPD_spanner single_spanner(single_wspd, single_r);
  assert_spanner(single_spanner, single_point_vec, Edge_vector());

  Point_d two_points[] = {get_point_d(d, 1, 2, 5, traits), get_point_d(d, 1, 4, 7, traits)};
  Point_vector two_points_vec(two_points, two_points + 2);
  WSPD two_points_wspd(d, 2.0, two_points_vec.begin(), two_points_vec.end());
  // With default representatives.
  WSPD_spanner_default two_points_spanner(two_points_wspd);
  Node_const_handle two_points_root = two_points_wspd.split_tree().root();
  Edge_vector two_points_expected_edges;
  two_points_expected_edges.push_back(Edge(two_points[0], two_points[1], two_points_root->left(), two_points_root->right()));
  assert_spanner(two_points_spanner, two_points_vec, two_points_expected_edges);

  Edge two_points_edge_p0 = *out_edges(two_points[0], two_points_spanner).first;
  assert(source(two_points_edge_p0, two_points_spanner) == two_points[0]);
  assert(target(two_points_edge_p0, two_points_spanner) == two_points[1]);
  assert(source_node(two_points_edge_p0, two_points_spanner) == two_points_root->left());
  assert(target_node(two_points_edge_p0, two_points_spanner) == two_points_root->right());

  Edge two_points_edge_p1 = *out_edges(two_points[1], two_points_spanner).first;
  assert(source(two_points_edge_p1, two_points_spanner) == two_points[1]);
  assert(target(two_points_edge_p1, two_points_spanner) == two_points[0]);
  assert(source_node(two_points_edge_p1, two_points_spanner) == two_points_root->right());
  assert(target_node(two_points_edge_p1, two_points_spanner) == two_points_root->left());



  Point_d three_points[] = {get_point_d(d, 0, 2, 1, traits), get_point_d(d, 0, 0, 3, traits), get_point_d(d, 0, 5, 5, traits)};
  Point_vector three_points_vec(three_points, three_points + 3);
  WSPD three_points_wspd(d, 2.0, three_points_vec.begin(), three_points_vec.end());
  R three_points_r(d, three_points_wspd.split_tree().root());
  WSPD_spanner three_points_spanner(three_points_wspd, three_points_r);
  Node_const_handle three_points_root = three_points_wspd.split_tree().root();
  Edge_vector three_points_expected_edges;
  three_points_expected_edges.push_back(Edge(three_points[0], three_points[1], three_points_root->left()->right(), three_points_root->left()->left()));
  three_points_expected_edges.push_back(Edge(three_points[0], three_points[2], three_points_root->left(), three_points_root->right()));
  three_points_expected_edges.push_back(Edge(three_points[1], three_points[2], three_points_root->left(), three_points_root->right()));
  assert_spanner(three_points_spanner, three_points_vec, three_points_expected_edges);

  Point_d four_points[] = {get_point_d(d, 5, 2, 3, traits), get_point_d(d, 4, 3, 4, traits), get_point_d(d, 0, 1, 5, traits), get_point_d(d, -1, -1, -8, traits)};
  Point_vector four_points_vec(four_points, four_points + 4);
  WSPD four_points_wspd(d, 2.0, four_points_vec.begin(), four_points_vec.end());
  R four_points_r(d, four_points_wspd.split_tree().root());
  WSPD_spanner four_points_spanner(four_points_wspd, four_points_r);
  Node_const_handle four_points_root = four_points_wspd.split_tree().root();
  Edge_vector four_points_expected_edges;
  four_points_expected_edges.push_back(Edge(four_points[2], four_points[3], four_points_root->left()->right(), four_points_root->left()->left()));
  four_points_expected_edges.push_back(Edge(four_points[0], four_points[1], four_points_root->right()->right(), four_points_root->right()->left()));
  four_points_expected_edges.push_back(Edge(four_points[0], four_points[2], four_points_root->right(), four_points_root->left()));
  four_points_expected_edges.push_back(Edge(four_points[0], four_points[3], four_points_root->right(), four_points_root->left()));
  four_points_expected_edges.push_back(Edge(four_points[1], four_points[2], four_points_root->right(), four_points_root->left()));
  four_points_expected_edges.push_back(Edge(four_points[1], four_points[3], four_points_root->right(), four_points_root->left()));
  assert_spanner(four_points_spanner, four_points_vec, four_points_expected_edges);


  Point_vector points;
  std::ifstream input("data/points.txt");
  double x,y,D;
  while (input >> x >> y >> D) {
    points.push_back(get_point_d(d, x, y, D, traits));
  }

  WSPD points_wspd(d, 1.4, points.begin(), points.end());
  R points_r(d, points_wspd.split_tree().root());
  WSPD_spanner points_spanner(points_wspd, points_r);

  Node_const_handle points_root = points_wspd.split_tree().root();
  Edge_vector points_expected_edges;

  points_expected_edges.push_back(Edge(points[5], points[3], points_root->right()->left(), points_root->right()->right()));

  points_expected_edges.push_back(Edge(points[3], points[4], points_root->right()->right(), points_root->left()->right()));

  points_expected_edges.push_back(Edge(points[4], points[5], points_root->left(), points_root->right()->left()));
  points_expected_edges.push_back(Edge(points[6], points[5], points_root->left(), points_root->right()->left()));

  points_expected_edges.push_back(Edge(points[6], points[3], points_root->left()->left()->left(), points_root->right()->right()));

  points_expected_edges.push_back(Edge(points[0], points[3], points_root->left()->left()->right(), points_root->right()->right()));
  points_expected_edges.push_back(Edge(points[2], points[3], points_root->left()->left()->right(), points_root->right()->right()));

  points_expected_edges.push_back(Edge(points[6], points[4], points_root->left()->left()->left(), points_root->left()->right()));

  points_expected_edges.push_back(Edge(points[0], points[4], points_root->left()->left()->right()->left(), points_root->left()->right()));

  points_expected_edges.push_back(Edge(points[1], points[4], points_root->left()->left()->right()->right(), points_root->left()->right()));
  points_expected_edges.push_back(Edge(points[2], points[4], points_root->left()->left()->right()->right(), points_root->left()->right()));

  points_expected_edges.push_back(Edge(points[6], points[0], points_root->left()->left()->left(), points_root->left()->left()->right()->left()));

  points_expected_edges.push_back(Edge(points[6], points[1], points_root->left()->left()->left(), points_root->left()->left()->right()->right()));
  points_expected_edges.push_back(Edge(points[6], points[2], points_root->left()->left()->left(), points_root->left()->left()->right()->right()));

  points_expected_edges.push_back(Edge(points[0], points[1], points_root->left()->left()->right()->left(), points_root->left()->left()->right()->right()));
  points_expected_edges.push_back(Edge(points[0], points[2], points_root->left()->left()->right()->left(), points_root->left()->left()->right()->right()));

  points_expected_edges.push_back(Edge(points[1], points[2], points_root->left()->left()->right()->right()->left(), points_root->left()->left()->right()->right()->right()));

  assert_spanner(points_spanner, points, points_expected_edges);

  std::cout << "Testing WSPD...done" << std::endl;
  return true;
}

} //namespace CGAL

#endif // WSPD_SPANNER_TEST_H
