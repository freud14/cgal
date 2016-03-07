#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/point_generators_3.h>
#include <CGAL/iterator.h>
#include <CGAL/WSPD.h>
#include <CGAL/Split_tree_traits_3.h>
#include <CGAL/IO/Geomview_stream.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_3 Point_3;
typedef Kernel::Sphere_3 Sphere_3;
typedef Kernel::Segment_3 Segment_3;
typedef Kernel::Iso_cuboid_3 Iso_cuboid_3;
typedef Kernel::FT FT;

typedef CGAL::Random_points_in_cube_3<Point_3> Random_points_iterator;
typedef CGAL::Counting_iterator<Random_points_iterator> N_Random_points_iterator;

typedef CGAL::Split_tree_traits_3<Kernel> Traits;
typedef CGAL::WSPD<Traits> WSPD;
typedef typename WSPD::Well_separated_pair Well_separated_pair;
typedef typename WSPD::Split_tree Split_tree;

typedef CGAL::Geomview_stream Geomview_stream;


Segment_3 segment_between_spheres(const Sphere_3& s1, const Sphere_3& s2);

template <class InputIterator>
void add_to_gv(Geomview_stream& gv, InputIterator begin, InputIterator end);


int main(int argc, char* argv[]) {
  const unsigned int N = 10;
  Random_points_iterator rpit(1.0);
  std::vector<Point_3> pts(N_Random_points_iterator(rpit,0), N_Random_points_iterator(N));
  std::vector<Well_separated_pair> pairs;
  WSPD wspd(3, 1.0, pts.begin(), pts.end());
  wspd.compute(std::back_inserter(pairs));

  Geomview_stream gv(CGAL::Bbox_3(-1, -1, -1, 1, 1, 1));
  gv.clear();
  add_to_gv(gv, pts.begin(), pts.end());

  for(std::vector<Well_separated_pair>::iterator it = pairs.begin(); it < pairs.end(); it++) {
    Well_separated_pair &pair = *it;
    Sphere_3 s1 = pair.first->enclosing_circle();
    Sphere_3 s2 = pair.second->enclosing_circle();
    if(!pair.first->is_leaf()) {
      gv << s1;
    }
    if(!pair.second->is_leaf()) {
      gv << s2;
    }
    gv << segment_between_spheres(s1, s2);
  }

  char ch;
  std::cin >> ch;

  gv.clear();
  add_to_gv(gv, pts.begin(), pts.end());
  const Split_tree& tree = wspd.split_tree();
  std::vector<Iso_cuboid_3> rect;
  tree.bounding_boxes(std::back_inserter(rect));
  for(std::vector<Iso_cuboid_3>::iterator it = rect.begin(); it != rect.end(); it++) {
    gv << (*it).bbox();
  }

  std::cin >> ch;
}

Segment_3 segment_between_spheres(const Sphere_3& s1, const Sphere_3& s2) {
  Point_3 p1 = s1.center();
  Point_3 p2 = s2.center();
  FT dx = p1.x() - p2.x();
  FT dy = p1.y() - p2.y();
  FT dz = p1.z() - p2.z();
  FT length = CGAL::sqrt(CGAL::squared_distance(p1, p2));
  FT r1 = CGAL::sqrt(s1.squared_radius());
  FT r2 = CGAL::sqrt(s2.squared_radius());
  Point_3 ep1 = Point_3(p1.x() - r1/length*dx, p1.y() - r1/length*dy, p1.z() - r1/length*dz);
  Point_3 ep2 = Point_3(p1.x() - (1-r2/length)*dx, p1.y() - (1-r2/length)*dy, p1.z() - (1-r2/length)*dz);
  return Segment_3(ep1, ep2);
}

template <class InputIterator>
void add_to_gv(Geomview_stream& gv, InputIterator begin, InputIterator end) {
  for(InputIterator it = begin; it != end; it++) {
    gv << *it;
  }
}
