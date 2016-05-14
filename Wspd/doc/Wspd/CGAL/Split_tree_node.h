namespace CGAL {

/*!
\ingroup PkgWspdAlgorithmClasses

The class `Split_tree_node` implements a node class for a split tree.

\tparam Traits must be a model of the concept
`SplitTreeTraits`, for example `Split_tree_traits_2<Simple_cartesian<double> >`.

\sa `CGAL::Split_tree<Traits>`
\sa `CGAL::Split_tree_traits_2<Kernel>`
\sa `CGAL::Split_tree_traits_3<Kernel>`
\sa `CGAL::Split_tree_traits_d<Kernel>`
*/
template <class Traits>
class Split_tree_node {
public:
/// \name Types
/// @{

/*!
Alias for this class.
*/
typedef Split_tree_node<Traits> Node;

/*!
A const handle for this class.
*/
typedef const Node* Node_const_handle;

/*!
Point class.
*/
typedef typename Traits::Point_d Point_d;

/*!
Box class.
*/
typedef typename Traits::Iso_box_d Iso_box_d;

/*!
Sphere class.
*/
typedef typename Traits::Sphere_d Sphere_d;

/*!
The point container class.
*/
typedef CGAL::Point_container<Traits> Point_container;

/// @}

/// \name Creation
/// @{

/*!
Constructs the node for `d` dimensions from the point container `container` and
the instance of `Traits` `traits`.
*/
Split_tree_node(int d, Point_container container, Traits traits);

/*!
Constructs a copy of the node `node`.
*/
Split_tree_node(const Split_tree_node& node);

/// @}

/// \name Operations
/// @{

/*!
Returns a pointer to the left child of the node or `NULL` if the node is a leaf.
*/
inline Node_const_handle left() const;

/*!
Returns a pointer to the right child of the node or `NULL` if the node is a leaf.
*/
inline Node_const_handle right() const;

/*!
Indicates whether a node is a leaf node.
*/
inline bool is_leaf() const;

/*!
Returns the bounding box of the node.
*/
Iso_box_d bounding_box() const;

/*!
Returns the enclosing of the node. This circle is computed if this is the first
time that this method is called. Otherwise, the answer is cached.

\sa `Split_tree_node::center()`
\sa `Split_tree_node::squared_radius()`
*/
Sphere_d enclosing_circle() const;

/*!
Returns the center of the enclosing circle. This method is useful because it is
less expensive than `Split_tree_node::enclosing_circle()`.
*/
Point_d center() const;

/*!
Returns the squared radius of the enclosing circle. This method is useful
because it is less expensive than `Split_tree_node::enclosing_circle()`.
*/
FT squared_radius() const;

/*!
Returns the point container of node.
*/
const Point_container& point_container() const;

/*!
Indicates wheter this node is well-separated with `w` with respect to the
separation ratio `s`. The calculation is done using the centers of the bounding
boxes of the two nodes and the biggest of the two squared radius as radius.

Note that not because two nodes are well-separated that
they are part of a well-separated pair decomposition of a point set.
*/
bool is_well_separated_with(Node_const_handle w, FT s) const;

/*!
Indicates wheter this node has a side that has a length longer than or equal to
the length of any side of `w`.
*/
bool has_longuer_side_than(Node_const_handle w) const;

/// @}

/// \name Destruction
/// @{

/*!
Frees the memory of the left and child nodes.
*/
~Split_tree_node();

/// @}
};

}
