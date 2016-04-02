#include <iostream>
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
typedef typename WSPD::Well_separated_pair_iterator Well_separated_pair_iterator;
typedef typename WSPD::Well_separated_pair Well_separated_pair;
typedef typename WSPD::Split_tree Split_tree;


int main(int argc, char* argv[]) {
  const unsigned int D = 4;
  const unsigned int N = 100000;
  CGAL::Random rand(42);
  Random_points_iterator rpit(D, 1.0, rand);
  std::vector<Point_d> pts;
  pts.reserve(N);
  for(int i = 0; i < N; i++) {
    pts.push_back(*rpit++);
  }
  WSPD wspd(D, 1.0, pts.begin(), pts.end());

  /*for(Well_separated_pair_iterator it = wspd.wspd_begin(); it < wspd.wspd_end(); it++) {
    Well_separated_pair &pair = *it;
    std::cout << "(" << pair.first->point_container().size() << ", " << pair.second->point_container().size() << ")" << std::endl;
  }*/

  std::cout << wspd.size() << std::endl;
  std::cout << (wspd.size() == 9392739 ? "true" : "false") << std::endl;
  return 0;
}
