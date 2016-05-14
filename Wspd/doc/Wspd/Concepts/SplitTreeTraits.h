#include <Concepts/SearchTraits.h>
#include <Concepts/RangeSearchTraits.h>
/*!
\ingroup PkgWspdConcepts
\cgalConcept

The concept `SplitTreeTraits` defines the requirements for the template
parameter of Split_tree class.
*/

class SplitTreeTraits : public SearchTraits, public RangeSearchTraits {
public:

/// \name Types
/// @{

/*!
Kernel type.
*/
typedef unspecified_type K;

/*!
Vector type.
*/
typedef unspecified_type Vector_d;

/*!
Construction object type for Sphere_d. Must provide
`Sphere_d operator()(int d, const Point_d& a, const Point_d& b) const`. It
 creates a sphere with the diameter defined by `a` and `b`.
*/
typedef unspecified_type Construct_sphere_d;

/*!
Construction object type for Point_d. Must provide
`template<class InputIterator>
Point_d operator()(int d, InputIterator first, InputIterator last, RT D) const`.
*/
typedef unspecified_type Construct_point_d;

/// @}

/// \name Operations
/// The following member functions to create instances of the above construction
/// object types must exist.
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

}; /* end SplitTreeTraits */
