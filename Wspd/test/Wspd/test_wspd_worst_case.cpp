#include <iostream>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/CORE_Expr.h>
#include <CGAL/WSPD.h>
#include <CGAL/Split_tree_traits_2.h>

typedef CGAL::Simple_cartesian<CORE::Expr> Kernel;
typedef Kernel::Point_2 Point_2;
typedef Kernel::FT FT;

typedef CGAL::Split_tree_traits_2<Kernel> Traits;
typedef CGAL::WSPD<Traits> WSPD;
typedef typename WSPD::Well_separated_pair_iterator Well_separated_pair_iterator;
typedef typename WSPD::Well_separated_pair Well_separated_pair;
typedef typename WSPD::Split_tree Split_tree;


int main(int argc, char* argv[]) {
  const unsigned int N = 3000;
  std::vector<Point_2> pts;
  pts.reserve(N);
  for(int i = 0; i < N; i++) {
    pts.push_back(Point_2(FT(0), CORE::pow(FT(2), i)));
  }
  WSPD wspd(2, 1.0, pts.begin(), pts.end());
  std::cout << wspd.size() << std::endl;
  return 0;
}
