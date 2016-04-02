namespace CGAL {

/*!
\ingroup PkgWspdTraitsClasses

The class `Split_tree_traits_2` can be used as a template parameter of the split
tree and the WSPD classes.

\tparam Kernel must be a model of the concept `Kernel`,
for example `Simple_cartesian<double>` or `Simple_cartesian<Gmpq>`.

\cgalModels `SplitTreeTraits`

\sa `Search_traits_2<Kernel>`
\sa `Split_tree_traits_3<Kernel>`
*/
template <class Kernel >
class Split_tree_traits_2 : public Search_traits_2<Kernel> {
public:
/// \name Types
/// @{

/*!
Kernel type.
*/
typedef Kernel K;

/*!
Vector type.
*/
typedef Kernel::Vector_2 Vector_d;

/*!
Functor with operator to construct a sphere from its diameter.
*/
typedef unspecified_type Construct_sphere_d;

/*!
Functor with operator to construct the point from its dimension and the set
of its coordinates.
*/
typedef unspecified_type Construct_point_d;

/// @}

/// \name Operations
/// @{

/*!
Function used to construct an object of type `Construct_sphere_d`.
*/
Construct_sphere_d construct_sphere_d_object() const;

/*!
Function used to construct an object of type `Construct_point_d`.
*/
Construct_point_d construct_point_d_object() const;

/// @}
};

} // namespace CGAL
