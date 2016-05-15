/*!
\ingroup PkgWspdConcepts
\cgalConcept

The concept `RepresentativeChooser` defines the way the representative are
chosen in a %WSPD spanner.
*/
class RepresentativeChooser {
public:

/// \name Operations
/// The following member functions to create instances of the above construction
/// object types must exist.
/// @{

/*!
Function used to get the representative of `node` via the output iterator `it`.
`other_node_in_pair` is the node with which `node` is well-separated in the
WSPD. This parameter is a convience if the choosing method requires it.
*/
template <class OutputIterator>
void operator()(Node_const_handle node, Node_const_handle other_node_in_pair, OutputIterator it) const;

/// @}

}; /* end SplitTreeTraits */
