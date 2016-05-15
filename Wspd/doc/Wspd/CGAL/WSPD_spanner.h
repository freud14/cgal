namespace CGAL {

/*!
\ingroup PkgWspdTraitsClasses

The class `Default_representative_chooser` can be used as a representative
chooser for a %WSPD spanner. This chooser chooses one representative arbitrarily
from the node. There is no garantee on the point chosen.

\tparam Traits must be a model of the concept
`SplitTreeTraits`, for example `Split_tree_traits_2<Simple_cartesian<double> >`.
This must be the same traits as the one passed to `WSPD_spanner<Traits>`.

\cgalModels `RepresentativeChooser`

\sa `Split_tree_traits_2<Kernel>`
\sa `Split_tree_traits_3<Kernel>`
\sa `Split_tree_traits_d<Kernel>`
*/
template <class Traits>
struct Default_representative_chooser {

/// \name Operations
/// @{

/*!
Chooses one representative arbitrarily in `node`.
*/
template <class OutputIterator>
void operator()(Node_const_handle node, Node_const_handle other_node_in_pair, OutputIterator it) const;

/// @}
};

/*!
\ingroup PkgWspdAlgorithmClasses

The class `WSPD_spanner_edge` defines an edge for a WSPD spanner. An edge in a
%WSPD spanner is undirected but, for the purpose of this class, an edge has a
source and a target.

\tparam Traits must be the same as `WSPD_spanner`.

\sa `CGAL::WSPD_spanner<Traits>`
*/
template <class Traits>
class WSPD_spanner_edge {
public:
/// \name Creation
/// @{

WSPD_spanner_edge(Point_d from, Point_d to, Node_const_handle node_from, Node_const_handle node_to);

/// @}

/// \name Public Attributes
/// @{

/*!
The source of the edge.
*/
Point_d from;

/*!
The target of the edge.
*/
Point_d to;

/*!
The node in which source is.
*/
Node_const_handle node_from;

/*!
The node in which target is.
*/
Node_const_handle node_to;

/// @}
};


/*!
\ingroup PkgWspdAlgorithmClasses

The class `WSPD_spanner` defines a WSPD spanner using from a WSPD.

\tparam Traits must be a model of the concept
`SplitTreeTraits`, for example `Split_tree_traits_2<Simple_cartesian<double> >`.
\tparam R must be a model of the concept
`RepresentativeChooser`, for example `Default_representative_chooser<Traits>`.

\sa `CGAL::WSPD<Traits>`
\sa `CGAL::Split_tree_traits_2<Kernel>`
\sa `CGAL::Split_tree_traits_3<Kernel>`
\sa `CGAL::Split_tree_traits_d<Kernel>`
*/
template <class Traits, class R = Default_representative_chooser<Traits> >
class WSPD_spanner {
public:
/// \name Types
/// @{

/*!
%WSPD class.
*/
typedef CGAL::WSPD<Traits>                                       WSPD;

/*!
Point class.
*/
typedef typename Traits::Point_d                                 Point_d;

/*!
An const iterator of `Point_d`.
*/
typedef unspecified_type                                         Vertex_iterator;

/*!
The edge class of the %WSPD spanner.
*/
typedef WSPD_spanner_edge<Traits>                                Edge;

/*!
An const iterator of `Edge`.
*/
typedef unspecified_type                                         Edge_iterator;

/*!
An const iterator of `Edge`.
*/
typedef unspecified_type                                         Vertex_edge_iterator;

/*!
An const iterator of `Point_d`.
*/
typedef unspecified_type                                         Adjacency_iterator;

/// @}

/// \name Creation
/// @{

/*!
Constructs a %WSPD spanner from a %WSPD. This constructor copies the pair of
the %WSPD.
*/
WSPD_spanner(const WSPD& wspd, const R& rep_chooser = R());

/// @}

/// \name Operations
/// @{

/*!
Computes the %WSPD spanner and caches it in the object.
*/
void compute() const;

/*!
Returns a const range for the points in the %WSPD spanner.
*/
std::pair<Vertex_iterator, Vertex_iterator> point_iterator() const;

/*!
Returns the number of points in the %WSPD spanner.
*/
int num_points() const;

/*!
Returns a const range to the edges of the %WSPD spanner. Because this is a
undirected graph, each edge \f$[u,v]\f$ of the graph may be returned as either
\f$[u,v]\f$ or \f$[v,u]\f$ but never both.

The computation of the %WSPD spanner may be done on the call of this method if
it was not done before.
*/
std::pair<Edge_iterator, Edge_iterator> edge_iterator() const;

/*!
Returns the number of edges of the %WSPD spanner.

The computation of the %WSPD spanner may be done on the call of this method if
it was not done before.
*/
int num_edges() const;

/*!
Returns a const range to the edges adjacent to a point in the %WSPD spanner.
Note that the source of each edge is the point `p`.

The computation of the %WSPD spanner may be done on the call of this method if
it was not done before.
*/
std::pair<Vertex_edge_iterator, Vertex_edge_iterator> vertex_edge_iterator(Point_d p) const;

/*!
Returns a const range to the neighbors of a point in the %WSPD spanner.

The computation of the %WSPD spanner may be done on the call of this method if
it was not done before.
*/
std::pair<Adjacency_iterator, Adjacency_iterator> adjacency_iterator(Point_d p) const;

/// @}
};

} // End namespace
