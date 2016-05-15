// Copyright (c) 2016 University of Ottawa (Canada)
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
// You can redistribute it and/or modify it under the terms of the GNU
// General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
//
// Licensees holding a valid commercial license may use this file in
// accordance with the commercial license agreement provided with the software.
//
// This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
// WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//
// $URL$
// $Id$
//
// Author(s)     : Frédérik Paradis (<fpara058@uottawa.ca>)

#ifndef CGAL_BOOST_GRAPH_TRAITS_WSPD_SPANNER_H
#define CGAL_BOOST_GRAPH_TRAITS_WSPD_SPANNER_H
#include <CGAL/WSPD_spanner.h>

#include <boost/graph/graph_traits.hpp>

namespace boost {

template <class Traits, class R>
struct graph_traits< CGAL::WSPD_spanner<Traits,R> > {

  struct graph_traversal_category :
    public virtual bidirectional_graph_tag,
    public virtual adjacency_graph_tag,
    public virtual edge_list_graph_tag,
    public virtual vertex_list_graph_tag { };

  typedef CGAL::WSPD_spanner<Traits,R> Spanner;

  typedef typename Spanner::Node_const_handle                   node_descriptor;

  typedef typename Spanner::Point_d                             vertex_descriptor;
  typedef typename Spanner::Edge                                edge_descriptor;
  typedef typename Spanner::Edge_iterator                       edge_iterator;

  typedef typename Spanner::Vertex_iterator                     vertex_iterator;
  typedef typename Spanner::Vertex_edge_iterator                out_edge_iterator;
  typedef typename Spanner::Vertex_edge_iterator                in_edge_iterator;
  typedef typename Spanner::Adjacency_iterator                  adjacency_iterator;

  typedef undirected_tag                                        directed_category;
  typedef disallow_parallel_edge_tag                            edge_parallel_category;
  typedef graph_traversal_category                              traversal_category;
  typedef int                                                   size_type;
  typedef size_type                                             vertices_size_type;
  typedef size_type                                             edges_size_type;
  typedef size_type                                             degree_size_type;
};

} // namespace boost

namespace CGAL {

template <class Traits, class R>
typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::vertex_descriptor
source(typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::edge_descriptor e,
       const CGAL::WSPD_spanner<Traits,R>& g)
{
  return e.from;
}

template <class Traits, class R>
typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::vertex_descriptor
target(typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::edge_descriptor e,
       const CGAL::WSPD_spanner<Traits,R>& g)
{
  return e.to;
}

template <class Traits, class R>
typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::node_descriptor
source_node(typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::edge_descriptor e,
       const CGAL::WSPD_spanner<Traits,R>& g)
{
  return e.node_from;
}

template <class Traits, class R>
typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::node_descriptor
target_node(typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::edge_descriptor e,
       const CGAL::WSPD_spanner<Traits,R>& g)
{
  return e.node_to;
}

template <class Traits, class R>
inline std::pair<
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::vertex_iterator,
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::vertex_iterator >
vertices(const CGAL::WSPD_spanner<Traits,R>& g)
{
  return g.point_iterator();
}


template <class Traits, class R>
inline std::pair<
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::edge_iterator,
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::edge_iterator >
edges(const CGAL::WSPD_spanner<Traits,R>& g)
{
  return g.edge_iterator();
}

template <class Traits, class R>
inline std::pair<
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::out_edge_iterator,
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::out_edge_iterator >
out_edges(
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::vertex_descriptor u,
  const CGAL::WSPD_spanner<Traits,R>& g)
{
  return g.vertex_edge_iterator(u);
}

template <class Traits, class R>
inline std::pair<
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::in_edge_iterator,
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::in_edge_iterator >
in_edges(
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::vertex_descriptor u,
  const CGAL::WSPD_spanner<Traits,R>& g)
{
  return g.vertex_edge_iterator(u);
}

template <class Traits, class R>
inline std::pair<
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::adjacency_iterator,
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::adjacency_iterator >
adjacent_vertices(
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::vertex_descriptor u,
  const CGAL::WSPD_spanner<Traits,R>& g)
{
  return g.adjacency_iterator(u);
}

template <class Traits, class R>
typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::vertices_size_type
num_vertices(const CGAL::WSPD_spanner<Traits,R>& g)
{
  return g.num_points();
}

template <class Traits, class R>
typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::edges_size_type
num_edges(const CGAL::WSPD_spanner<Traits,R>& g)
{
  return  g.num_edges();
}

template <class Traits, class R>
typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::degree_size_type
out_degree(
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::vertex_descriptor u,
  const CGAL::WSPD_spanner<Traits,R>& g)
{
  return degree(u, g);
}

template <class Traits, class R>
typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::degree_size_type
in_degree(
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::vertex_descriptor u,
  const CGAL::WSPD_spanner<Traits,R>& g)
{
  return degree(u, g);
}

template <class Traits, class R>
typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::degree_size_type
degree(
  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::vertex_descriptor u,
  const CGAL::WSPD_spanner<Traits,R>& g)
{
  typedef typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::adjacency_iterator adjacency_iterator;

  typename boost::graph_traits< CGAL::WSPD_spanner<Traits,R> >::degree_size_type deg = 0;
  std::pair<adjacency_iterator, adjacency_iterator> iterators = adjacent_vertices(u, g);
  for(adjacency_iterator it = iterators.first; it != iterators.second; it++) {
    ++deg;
  }

  return deg;
}

} // namespace CGAL

#endif // CGAL_BOOST_GRAPH_TRAITS_WSPD_SPANNER_H
