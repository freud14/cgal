#include <CGAL/Simple_cartesian.h>
#include <CGAL/Cartesian.h>
#include <CGAL/Homogeneous.h>
#include <CGAL/Cartesian_d.h>
#include <CGAL/Homogeneous_d.h>
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
  CGAL::Split_tree_traits_2< CGAL::Simple_cartesian<CGAL::Quotient<CGAL::Gmpz> > > wspd_2_S_Qgmp;
  std::cout << "SimpleCartesian<Quotient<Gmpz> > >:     ";
  CGAL::wspd__batch_test(2, wspd_2_S_Qgmp);
#endif

  CGAL::Split_tree_traits_2< CGAL::Simple_cartesian<double> > wspd_2_S_double;
  std::cout << "SimpleCartesian<double>:     ";
  CGAL::wspd__batch_test(2, wspd_2_S_double);

  CGAL::Split_tree_traits_2< CGAL::Simple_cartesian<CORE::Expr> > wspd_2_S_core;
  std::cout << "SimpleCartesian<CORE::Expr>:     ";
  CGAL::wspd__batch_test(2, wspd_2_S_core);

#ifdef CGAL_USE_LEDA
  CGAL::Split_tree_traits_2< CGAL::Cartesian<leda_rational> > wspd_2_C_rational;
  std::cout << "Cartesian<rational>:     ";
  CGAL::wspd__batch_test(2, wspd_2_C_rational);
#endif

#ifdef CGAL_USE_GMP
  CGAL::Split_tree_traits_2< CGAL::Cartesian<CGAL::Quotient<CGAL::Gmpz> > > wspd_2_C_Qgmp;
  std::cout << "Cartesian<Quotient<Gmpz> > >:     ";
  CGAL::wspd__batch_test(2, wspd_2_C_Qgmp);
#endif

  CGAL::Split_tree_traits_2< CGAL::Cartesian<double> > wspd_2_C_double;
  std::cout << "Cartesian<double>:     ";
  CGAL::wspd__batch_test(2, wspd_2_C_double);

  CGAL::Split_tree_traits_2< CGAL::Cartesian<CORE::Expr> > wspd_2_C_core;
  std::cout << "Cartesian<CORE::Expr>:     ";
  CGAL::wspd__batch_test(2, wspd_2_C_core);

#ifdef CGAL_USE_LEDA
  CGAL::Split_tree_traits_2< CGAL::Homogeneous<leda_rational> > wspd_2_H_rational;
  std::cout << "Homogeneous<rational>:     ";
  CGAL::wspd__batch_test(2, wspd_2_H_rational);
#endif

#ifdef CGAL_USE_GMP
  CGAL::Split_tree_traits_2< CGAL::Homogeneous<CGAL::Gmpz> > wspd_2_H_gmp;
  std::cout << "Homogeneous<Gmpz>:     ";
  CGAL::wspd__batch_test(2, wspd_2_H_gmp);
#endif

  CGAL::Split_tree_traits_2< CGAL::Homogeneous<double> > wspd_2_H_double;
  std::cout << "Homogeneous<double>:     ";
  CGAL::wspd__batch_test(2, wspd_2_H_double);

  CGAL::Split_tree_traits_2< CGAL::Homogeneous<CORE::Expr> > wspd_2_H_core;
  std::cout << "Homogeneous<CORE::Expr>:     ";
  CGAL::wspd__batch_test(2, wspd_2_H_core);


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

#ifdef CGAL_USE_LEDA
  CGAL::Split_tree_traits_3< CGAL::Cartesian<leda_rational> > wspd_3_C_rational;
  std::cout << "Cartesian<rational>:     ";
  CGAL::wspd__batch_test(3, wspd_3_C_rational);
#endif

#ifdef CGAL_USE_GMP
  CGAL::Split_tree_traits_3< CGAL::Cartesian<CGAL::Quotient<CGAL::Gmpz> > > wspd_3_C_Qgmp;
  std::cout << "Cartesian<Quotient<Gmpz> > >:     ";
  CGAL::wspd__batch_test(3, wspd_3_C_Qgmp);
#endif

  CGAL::Split_tree_traits_3< CGAL::Cartesian<double> > wspd_3_C_double;
  std::cout << "Cartesian<double>:     ";
  CGAL::wspd__batch_test(3, wspd_3_C_double);

  CGAL::Split_tree_traits_3< CGAL::Cartesian<CORE::Expr> > wspd_3_C_core;
  std::cout << "Cartesian<CORE::Expr>:     ";
  CGAL::wspd__batch_test(3, wspd_3_C_core);

#ifdef CGAL_USE_LEDA
  CGAL::Split_tree_traits_3< CGAL::Homogeneous<leda_rational> > wspd_3_H_rational;
  std::cout << "Homogeneous<rational>:     ";
  CGAL::wspd__batch_test(3, wspd_3_H_rational);
#endif

#ifdef CGAL_USE_GMP
  CGAL::Split_tree_traits_3< CGAL::Homogeneous<CGAL::Gmpz> > wspd_3_H_gmp;
  std::cout << "Homogeneous<Gmpz>:     ";
  CGAL::wspd__batch_test(3, wspd_3_H_gmp);
#endif

  CGAL::Split_tree_traits_3< CGAL::Homogeneous<double> > wspd_3_H_double;
  std::cout << "Homogeneous<double>:     ";
  CGAL::wspd__batch_test(3, wspd_3_H_double);

  CGAL::Split_tree_traits_3< CGAL::Homogeneous<CORE::Expr> > wspd_3_H_core;
  std::cout << "Homogeneous<CORE::Expr>:     ";
  CGAL::wspd__batch_test(3, wspd_3_H_core);


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

  CGAL::Split_tree_traits_d< CGAL::Cartesian_d<double> > wspd_d_S_double;
  std::cout << "Cartesian_d<double>:     ";
  CGAL::wspd__batch_test(d, wspd_d_S_double);

  CGAL::Split_tree_traits_d< CGAL::Cartesian_d<CORE::Expr> > wspd_d_S_core;
  std::cout << "Cartesian_d<CORE::Expr>:     ";
  CGAL::wspd__batch_test(d, wspd_d_S_core);

#ifdef CGAL_USE_LEDA
  CGAL::Split_tree_traits_d< CGAL::Homogeneous_d<leda_rational> > wspd_d_H_rational;
  std::cout << "Homogeneous_d<rational>:     ";
  CGAL::wspd__batch_test(d, wspd_d_H_rational);
#endif

#ifdef CGAL_USE_GMP
  CGAL::Split_tree_traits_d< CGAL::Homogeneous_d<CGAL::Gmpz> > wspd_d_H_gmp;
  std::cout << "Homogeneous_d<Gmpz>:     ";
  CGAL::wspd__batch_test(d, wspd_d_H_gmp);
#endif

  CGAL::Split_tree_traits_d< CGAL::Homogeneous_d<double> > wspd_d_H_double;
  std::cout << "Homogeneous_d<double>:     ";
  CGAL::wspd__batch_test(d, wspd_d_H_double);

  CGAL::Split_tree_traits_d< CGAL::Homogeneous_d<CORE::Expr> > wspd_d_H_core;
  std::cout << "Homogeneous_d<CORE::Expr>:     ";
  CGAL::wspd__batch_test(d, wspd_d_H_core);

  return 0;
}
