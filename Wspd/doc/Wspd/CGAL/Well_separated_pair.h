namespace CGAL {

/*!
\ingroup PkgWspdAlgorithmClasses

The class `Well_separated_pair` represents a well-separated pair with the two
split tree nodes.

\tparam Traits must be a model of the concept
`SplitTreeTraits`, for example `Split_tree_traits_2<Simple_cartesian<double> >`.

\sa `CGAL::WSPD<Traits>`
\sa `CGAL::Split_tree<Traits>`
\sa `CGAL::Split_tree_traits_2<Kernel>`
\sa `CGAL::Split_tree_traits_3<Kernel>`
\sa `CGAL::Split_tree_traits_d<Kernel>`
*/
template <class Traits>
class Well_separated_pair {
public:
/// \name Types
/// @{

/*!
Split tree class.
*/
typedef CGAL::Split_tree<Traits>                                   Split_tree;

/*!
Split tree node const handle.
*/
typedef typename Split_tree::Node_const_handle                     Node_const_handle;

/// @}

/// \name Creation
/// @{

/*!
Intialize a well-separated pair with two well-separated split tree nodes.
*/
Well_separated_pair(Node_const_handle a, Node_const_handle b);

/// @}

/// \name Operations
/// @{

/*!
Returns a pointer to the first split tree node of the well-separated pair.
*/
inline Node_const_handle a() const;

/*!
Returns a pointer to the second split tree node of the well-separated pair.
*/
inline Node_const_handle b() const;

/// @}
};

}
