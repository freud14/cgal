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

#include <CGAL/wspd_test.h>

int main(int argc, char* argv[])
{
  std::cout << "In 2 dimensions:" << std::endl;
#ifdef CGAL_USE_LEDA
  CGAL::Split_tree_traits_2< CGAL::Simple_cartesian<leda_rational> > wspd_2_S_rational;
  std::cout << "SimpleCartesian<rational>:     ";
  CGAL::wspd__batch_test(2, wspd_2_S_rational);
#endif

#ifdef CGAL_USE_GMP
  CGAL::Split_tree_traits_2< CGAL::Simple_cartesian<CGAL::Quotient<CGAL::Gmpz> > > wspd_S_Qgmp;
  std::cout << "SimpleCartesian<Quotient<Gmpz> > >:     ";
  CGAL::wspd__batch_test(2, wspd_S_Qgmp);
#endif

  CGAL::Split_tree_traits_2< CGAL::Simple_cartesian<double> > wspd_S_double;
  std::cout << "SimpleCartesian<double>:     ";
  CGAL::wspd__batch_test(2, wspd_S_double);

  CGAL::Split_tree_traits_2< CGAL::Simple_cartesian<CORE::Expr> > wspd_S_core;
  std::cout << "SimpleCartesian<CORE::Expr>:     ";
  CGAL::wspd__batch_test(2, wspd_S_core);


  std::cout << "In 3 dimensions:" << std::endl;
#ifdef CGAL_USE_LEDA
  CGAL::Split_tree_traits_3< CGAL::Simple_cartesian<leda_rational> > wspd_3_S_rational;
  std::cout << "SimpleCartesian<rational>:     ";
  CGAL::wspd__batch_test(3, wspd_3_S_rational);
#endif

#ifdef CGAL_USE_GMP
  CGAL::Split_tree_traits_3< CGAL::Simple_cartesian<CGAL::Quotient<CGAL::Gmpz> > > wspd_3_S_Qgmp;
  std::cout << "SimpleCartesian<Quotient<Gmpz> > >:     ";
  CGAL::wspd__batch_test(3, wspd_3_S_Qgmp);
#endif

  CGAL::Split_tree_traits_3< CGAL::Simple_cartesian<double> > wspd_3_S_double;
  std::cout << "SimpleCartesian<double>:     ";
  CGAL::wspd__batch_test(3, wspd_3_S_double);

  CGAL::Split_tree_traits_3< CGAL::Simple_cartesian<CORE::Expr> > wspd_3_S_core;
  std::cout << "SimpleCartesian<CORE::Expr>:     ";
  CGAL::wspd__batch_test(3, wspd_3_S_core);


  int d = 4;
  std::cout << "In d dimensions (d = 4):" << std::endl;
#ifdef CGAL_USE_LEDA
  CGAL::Split_tree_traits_d< CGAL::Cartesian_d<leda_rational> > wspd_d_S_rational;
  std::cout << "Cartesian_d<rational>:     ";
  CGAL::wspd__batch_test(d, wspd_d_S_rational);
#endif

#ifdef CGAL_USE_GMP
  CGAL::Split_tree_traits_d< CGAL::Cartesian_d<CGAL::Quotient<CGAL::Gmpz> > > wspd_d_S_Qgmp;
  std::cout << "Cartesian_d<Quotient<Gmpz> > >:     ";
  CGAL::wspd__batch_test(d, wspd_d_S_Qgmp);
#endif

  typedef CGAL::Split_tree_traits_d< CGAL::Cartesian_d<double> > Traits;
  CGAL::Split_tree_traits_d< CGAL::Cartesian_d<double> > wspd_d_S_double;
  std::cout << "Cartesian_d<double>:     ";
  CGAL::wspd__batch_test(d, wspd_d_S_double);

  CGAL::Split_tree_traits_d< CGAL::Cartesian_d<CORE::Expr> > wspd_d_S_core;
  std::cout << "Cartesian_d<CORE::Expr>:     ";
  CGAL::wspd__batch_test(d, wspd_d_S_core);
  return 0;
}
