#include <CGAL/Simple_cartesian.h>
#include <CGAL/Cartesian_d.h>
#include <CGAL/Split_tree_traits_2.h>
#include <CGAL/Split_tree_traits_3.h>
#include <CGAL/Split_tree_traits_d.h>

#ifdef CGAL_USE_LEDA
#include <CGAL/leda_integer.h>
#include <CGAL/leda_rational.h>
#endif

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
#endif

#include <CGAL/CORE_Expr.h>

#include <CGAL/test_split_tree.h>

int main(int argc, char* argv[])
{
  std::cout << "In 2 dimensions:" << std::endl;
#ifdef CGAL_USE_LEDA
  CGAL::Split_tree_traits_2< CGAL::Simple_cartesian<leda_rational> > split_tree_2_S_rational;
  std::cout << "SimpleCartesian<rational>:     ";
  CGAL::split_tree__batch_test(2, split_tree_2_S_rational);
#endif

#ifdef CGAL_USE_GMP
  CGAL::Split_tree_traits_2< CGAL::Simple_cartesian<CGAL::Quotient<CGAL::Gmpz> > > split_tree_S_Qgmp;
  std::cout << "SimpleCartesian<Quotient<Gmpz> > >:     ";
  CGAL::split_tree__batch_test(2, split_tree_S_Qgmp);
#endif

  CGAL::Split_tree_traits_2< CGAL::Simple_cartesian<double> > split_tree_S_double;
  std::cout << "SimpleCartesian<double>:     ";
  CGAL::split_tree__batch_test(2, split_tree_S_double);

  CGAL::Split_tree_traits_2< CGAL::Simple_cartesian<CORE::Expr> > split_tree_S_core;
  std::cout << "SimpleCartesian<CORE::Expr>:     ";
  CGAL::split_tree__batch_test(2, split_tree_S_core);


  std::cout << "In 3 dimensions:" << std::endl;
#ifdef CGAL_USE_LEDA
  CGAL::Split_tree_traits_3< CGAL::Simple_cartesian<leda_rational> > split_tree_3_S_rational;
  std::cout << "SimpleCartesian<rational>:     ";
  CGAL::split_tree__batch_test(3, split_tree_3_S_rational);
#endif

#ifdef CGAL_USE_GMP
  CGAL::Split_tree_traits_3< CGAL::Simple_cartesian<CGAL::Quotient<CGAL::Gmpz> > > split_tree_3_S_Qgmp;
  std::cout << "SimpleCartesian<Quotient<Gmpz> > >:     ";
  CGAL::split_tree__batch_test(3, split_tree_3_S_Qgmp);
#endif

  CGAL::Split_tree_traits_3< CGAL::Simple_cartesian<double> > split_tree_3_S_double;
  std::cout << "SimpleCartesian<double>:     ";
  CGAL::split_tree__batch_test(3, split_tree_3_S_double);

  CGAL::Split_tree_traits_3< CGAL::Simple_cartesian<CORE::Expr> > split_tree_3_S_core;
  std::cout << "SimpleCartesian<CORE::Expr>:     ";
  CGAL::split_tree__batch_test(3, split_tree_3_S_core);


  int d = 4;
  std::cout << "In d dimensions (d = 4):" << std::endl;
#ifdef CGAL_USE_LEDA
  CGAL::Split_tree_traits_d< CGAL::Cartesian_d<leda_rational> > split_tree_d_S_rational;
  std::cout << "Cartesian_d<rational>:     ";
  CGAL::split_tree__batch_test(d, split_tree_d_S_rational);
#endif

#ifdef CGAL_USE_GMP
  CGAL::Split_tree_traits_d< CGAL::Cartesian_d<CGAL::Quotient<CGAL::Gmpz> > > split_tree_d_S_Qgmp;
  std::cout << "Cartesian_d<Quotient<Gmpz> > >:     ";
  CGAL::split_tree__batch_test(d, split_tree_d_S_Qgmp);
#endif

  typedef CGAL::Split_tree_traits_d< CGAL::Cartesian_d<double> > Traits;
  CGAL::Split_tree_traits_d< CGAL::Cartesian_d<double> > split_tree_d_S_double;
  std::cout << "Cartesian_d<double>:     ";
  CGAL::split_tree__batch_test(d, split_tree_d_S_double);

  CGAL::Split_tree_traits_d< CGAL::Cartesian_d<CORE::Expr> > split_tree_d_S_core;
  std::cout << "Cartesian_d<CORE::Expr>:     ";
  CGAL::split_tree__batch_test(d, split_tree_d_S_core);
  return 0;
}
