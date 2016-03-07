#include <CGAL/Cartesian_d.h>
#include <CGAL/point_generators_d.h>
#include <CGAL/iterator.h>
#include <CGAL/WSPD.h>
#include <CGAL/Split_tree_traits_d.h>

typedef CGAL::Cartesian_d<double> Kernel;
typedef Kernel::Point_d Point_d;
typedef Kernel::FT FT;

typedef CGAL::Random_points_in_cube_d<Point_d> Random_points_iterator;

typedef CGAL::Split_tree_traits_d<Kernel> Traits;
typedef CGAL::WSPD<Traits> WSPD;
typedef typename WSPD::Well_separated_pair Well_separated_pair;
typedef typename WSPD::Split_tree Split_tree;


int main(int argc, char* argv[]) {
  const unsigned int D = 4;
  const unsigned int N = 1000;
  Random_points_iterator rpit(D, 1.0);
  std::vector<Point_d> pts;
  pts.reserve(N);
  for(int i = 0; i < N; i++) {
    pts.push_back(*rpit++);
  }
  std::vector<Well_separated_pair> pairs;
  WSPD wspd(D, 1.0, pts.begin(), pts.end());
  wspd.compute(std::back_inserter(pairs));
}
