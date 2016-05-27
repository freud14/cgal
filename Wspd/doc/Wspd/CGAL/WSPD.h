namespace CGAL {

/*!
\ingroup PkgWspdAlgorithmClasses

The class `WSPD` defines a Well-separated pair decomposition (%WSPD) using a
split tree.

\tparam Traits must be a model of the concept
`SplitTreeTraits`, for example `Split_tree_traits_2<Simple_cartesian<double> >`.

\sa `CGAL::Split_tree<Traits>`
\sa `CGAL::Split_tree_traits_2<Kernel>`
\sa `CGAL::Split_tree_traits_3<Kernel>`
\sa `CGAL::Split_tree_traits_d<Kernel>`
*/
template <class Traits>
class WSPD {
public:

/// \name Types
/// @{

/*!
Point class.
*/
typedef typename Traits::Point_d                                   Point_d;

/*!
Split tree class.
*/
typedef CGAL::Split_tree<Traits>                                   Split_tree;

/*!
Split tree node.
*/
typedef typename Split_tree::Node                                  Node;

/*!
Split tree node const handle.
*/
typedef typename Split_tree::Node_const_handle                     Node_const_handle;


/*!
A well-separated pair as a pair of node of the split tree.
*/
typedef CGAL::Well_separated_pair<Traits>                          Well_separated_pair;

/*!
A well-separated pair decomposition as a Range of Well_separated_pair's;
*/
typedef unspecified_type                                           Well_separated_pair_decomposition;

/*!
An const iterator of `Well_separated_pair`.
*/
typedef unspecified_type                                           Well_separated_pair_iterator;

/*!
An const iterator of `Point_d`.
*/
typedef unspecified_type                                           Point_iterator;

/*!
Number type.
*/
typedef typename Traits::FT                                        FT;

/// @}

/// \name Creation
/// @{

/*!
Constructs an empty %WSPD.
*/
WSPD(int d, FT separation_ratio);

/*!
Constructs a %WSPD on the elements from the sequence
`[begin, end)` for the dimension `d` using the separation ratio
`separation_ratio`. The value type of the `InputIterator` must be `Point_d`.
*/
template <class InputIterator>
WSPD(int d, FT separation_ratio, InputIterator begin, InputIterator end);

/// @}

/// \name Operations
/// @{

/*!
Computes the %WSPD and caches it in the object. The %WSPD can be recovered using
the methods `wspd_begin` and `wspd_end`.
*/
virtual void compute() const;


/*!
Returns the dimension.
*/
int dimension() const;

/*!
Returns the separation ratio.
*/
FT separation_ratio() const;

/*!
Changes the separation ratio. The %WSPD will need to be recomputed.
*/
void separation_ratio(FT separation_ratio);

/*!
Replaces the point set by the new point set defined from the sequence
`[begin, end)` for the dimension `d`. The value type of the `InputIterator`
must be `Point_d`. The %WSPD will need to be recomputed.
*/
template <class InputIterator>
void set(int d, InputIterator begin, InputIterator end);

/*!
Adds the new point set defined from the sequence `[begin, end)`. The value
type of the `InputIterator` must be `Point_d`. The %WSPD will need to be
recomputed.
*/
template <class InputIterator>
void add(InputIterator begin, InputIterator end);

/*!
Removes all points from the %WSPD.
*/
void clear();

/*!
Returns the associated split tree.
*/
const Split_tree& split_tree() const;

/*!
Returns the well-separated pair decomposition.
*/
const Well_separated_pair_decomposition& wspd() const;

/*!
Returns a const iterator to the first pair in the %WSPD. The computation of the
%WSPD may be done on the call of this method if it was not done before.
*/
Well_separated_pair_iterator wspd_begin() const;

/*!
Returns a past-the-end const iterator of the pairs in the %WSPD. The computation
of the %WSPD may be done on the call of this method if it was not done before.
*/
Well_separated_pair_iterator wspd_end() const;

/*!
Returns the number of pairs in the %WSPD. The computation of the %WSPD may be
done on the call of this method if it was not done before.
*/
int wspd_size() const;

/*!
Returns a const iterator to the first point in the %WSPD.
*/
Point_iterator points_begin() const;

/*!
Returns a past-the-end const iterator of the points in the %WSPD.
*/
Point_iterator points_end() const;

/*!
Returns the number of points in the %WSPD.
*/
int points_size() const;

/// @}
};


}
