namespace CGAL {

/*!
\ingroup PkgWspdAlgorithmClasses

The class `Split_tree` defines a split tree.

\tparam Traits must be a model of the concept
`SplitTreeTraits`, for example `Split_tree_traits_2<Simple_cartesian<double> >`.

\sa `CGAL::Split_tree_traits_2<Kernel>`
\sa `CGAL::Split_tree_traits_3<Kernel>`
\sa `CGAL::Split_tree_traits_d<Kernel>`
\sa `CGAL::WSPD<Traits>`
*/
template <class Traits>
class Split_tree {
public:

/// \name Types
/// @{

/*!
Split tree node class.
*/
typedef Split_tree_node<Traits> Node;

/*!
Point class.
*/
typedef typename Traits::Point_d Point_d;

/*!
Box class.
*/
typedef typename Traits::Iso_box_d Iso_box_d;

/*!
An const iterator of `Iso_box_d`.
*/
typedef unspecified_type Bounding_box_iterator;

/// @}

/// \name Creation
/// @{

/*!
Constructs an empty split tree.
*/
Split_tree(int d);

/*!
Constructs a split tree on the elements from the sequence
`[begin, end)` for the dimension `d`. The value type of the `InputIterator`
must be `Point_d`.
*/
template <class InputIterator>
Split_tree(int d, InputIterator begin, InputIterator end);

/*!
Constructs a copy of a split tree.
*/
Split_tree(const Split_tree<Traits>& split_tree);

/*!
Replaces the current split tree by another split tree. The split tree will need
to be recomputed.
*/
Split_tree<Traits>& operator=(const Split_tree<Traits>& split_tree);

/// @}

/// \name Operations
/// @{

/*!
Computes the split tree and caches it in the object. The split tree can be
recovered using the method `root`.
*/
void compute() const;

/*!
Computes the set of bounding boxes of the split tree and caches it in the
object. The bounding boxes can be recovered using the methods
`bounding_box_begin` and `bounding_box_end`. The computation of the split tree
may be done on the call of this method if it was not done before.
*/
void compute_bounding_boxes() const;

/*!
Replaces the point set by the new point set defined from the sequence
`[begin, end)` for the dimension `d`. The value type of the `InputIterator`
must be `Point_d`. The split tree will need to be recomputed.
*/
template <class InputIterator>
void set(int d, InputIterator begin, InputIterator end);

/*!
Adds the new point set defined from the sequence `[begin, end)`. The value
type of the `InputIterator` must be `Point_d`. The split tree will need to be
recomputed.
*/
template <class InputIterator>
void add(InputIterator begin, InputIterator end);

/*!
Removes all points from the split tree.
*/
void clear();

/*!
Returns a pointer to the root node of the split tree or NULL if the point set of
the split tree is empty.
*/
const Node* root() const;

/*!
Returns a const iterator to the first bounding box in the split tree. The order
of the bounding boxes is not guaranteed. The computation of the bounding boxes
and the split tree may be done on the call of this method if it was not done
before.
*/
Bounding_box_iterator bounding_box_begin() const;

/*!
Returns a past-the-end const iterator of the bounding boxes in the split tree.
The computation of the bounding boxes and the split tree may be done on the
call of this method if it was not done before.
*/
Bounding_box_iterator bounding_box_end() const;

/// @}

/// \name Destruction
/// @{

/*!
Frees the memory of the nodes.
*/
~Split_tree();

/// @}
};


}
