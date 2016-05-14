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

#ifndef CGAL_WSPD_SPANNER_H
#define CGAL_WSPD_SPANNER_H
#include <CGAL/WSPD.h>
#include <CGAL/Point_container.h>
#include <CGAL/constructions_d.h>

#include <utility>
#include <algorithm>

#include <boost/graph/graph_traits.hpp>
#include <boost/iterator/transform_iterator.hpp>

namespace CGAL {

template <class Traits>
struct Default_representative_chooser {
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Node_const_handle                           Node_const_handle;

  template <class OutputIterator>
  void operator()(Node_const_handle node, Node_const_handle other_node_in_pair, OutputIterator it) const {
    *it = **node->point_container().begin();
    ++it;
  }
};

template <class Traits>
class WSPD_spanner_edge {
private:
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Node_const_handle                           Node_const_handle;
  typedef typename Traits::Point_d                                   Point_d;
public:
  WSPD_spanner_edge(Point_d from_, Point_d to_, Node_const_handle node_from_, Node_const_handle node_to_) :
                      from(from_), to(to_), node_from(node_from_), node_to(node_to_) { }
  Point_d from;
  Point_d to;
  Node_const_handle node_from;
  Node_const_handle node_to;
};

template <class Traits, class R>
class WSPD_spanner_edge_iterator;

template <class Traits, class R>
class WSPD_spanner_vertex_edge_iterator;

template <class Traits, class R>
class WSPD_spanner_adjacency_iterator;

template <class Traits, class R = Default_representative_chooser<Traits> >
class WSPD_spanner {
public:
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Well_separated_pair                         Well_separated_pair;
  typedef typename WSPD::Well_separated_pair_decomposition           Well_separated_pair_decomposition;
  typedef typename WSPD::Well_separated_pair_iterator                Well_separated_pair_iterator;
  typedef typename WSPD::Node_const_handle                           Node_const_handle;

  typedef typename Traits::Point_d                                   Point_d;
  typedef typename Traits::FT                                        FT;

  typedef std::vector<Point_d>                                       Point_vector;
  typedef typename Point_vector::const_iterator                      Point_iterator;

  typedef Point_iterator                                             Vertex_iterator;
  typedef WSPD_spanner_edge<Traits>                                  Edge;
  typedef WSPD_spanner_edge_iterator<Traits,R>                       Edge_iterator;
  typedef WSPD_spanner_vertex_edge_iterator<Traits,R>                Vertex_edge_iterator;
  typedef WSPD_spanner_adjacency_iterator<Traits,R>                  Adjacency_iterator;

  WSPD_spanner(const WSPD& wspd_, R rep_chooser = R()) :
                    wspd(wspd_.wspd()), points(wspd_.points_begin(), wspd_.points_end()), choose_rep(rep_chooser), computed(false) { }

  void compute() const {
    if(!computed) {
      nb_edges = 0;
      for(Well_separated_pair_iterator it = wspd.begin(); it != wspd.end(); it++) {
        const Well_separated_pair& pair = *it;
        Node_const_handle a = pair.a();
        Node_const_handle b = pair.b();
        Point_vector rep_a, rep_b;
        choose_rep(a, b, std::back_inserter(rep_a));
        choose_rep(b, a, std::back_inserter(rep_b));
        nb_edges += rep_a.size() * rep_b.size();
        for(Point_iterator point_it = rep_a.begin(); point_it != rep_a.end(); point_it++) {
          edges[*point_it].push_back(std::make_pair(a, b));
        }
        for(Point_iterator point_it = rep_b.begin(); point_it != rep_b.end(); point_it++) {
          edges[*point_it].push_back(std::make_pair(b, a));
        }
      }
      computed = true;
    }
  }

  std::pair<Vertex_iterator, Vertex_iterator> point_iterator() const {
    return std::pair<Vertex_iterator, Vertex_iterator>(points.begin(), points.end());
  }

  int num_points() const {
    return points.size();
  }

  std::pair<Edge_iterator, Edge_iterator> edge_iterator() const {
    compute();
    return std::make_pair(Edge_iterator::begin(wspd, choose_rep), Edge_iterator::end(wspd, choose_rep));
  }

  int num_edges() const {
    compute();
    return nb_edges;
  }

  std::pair<Vertex_edge_iterator, Vertex_edge_iterator> vertex_edge_iterator(Point_d p) const {
    compute();
    return std::make_pair(Vertex_edge_iterator::begin(p, edges[p], choose_rep), Vertex_edge_iterator::end(p, edges[p], choose_rep));
  }

  std::pair<Adjacency_iterator, Adjacency_iterator> adjacency_iterator(Point_d p) const {
    compute();
    std::pair<Vertex_edge_iterator, Vertex_edge_iterator> pair = vertex_edge_iterator(p);
    return std::make_pair(Adjacency_iterator(pair.first), Adjacency_iterator(pair.second));
  }
private:
  Well_separated_pair_decomposition wspd;
  Point_vector points;
  R choose_rep;
  mutable bool computed;
  mutable int nb_edges;
  mutable std::map<Point_d, std::vector<std::pair<Node_const_handle,Node_const_handle> > > edges;
};

template <class Traits, class R>
class WSPD_spanner_edge_iterator : public std::iterator<std::input_iterator_tag, WSPD_spanner_edge<Traits> > {
private:
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Well_separated_pair                         Well_separated_pair;
  typedef typename WSPD::Well_separated_pair_decomposition           Well_separated_pair_decomposition;
  typedef typename WSPD::Well_separated_pair_iterator                Well_separated_pair_iterator;
  typedef typename WSPD::Node_const_handle                           Node_const_handle;

  typedef typename Traits::Point_d                                   Point_d;

  typedef std::vector<Point_d>                                       Point_vector;
  typedef typename Point_vector::const_iterator                      Point_iterator;

  typedef WSPD_spanner_edge<Traits>                                  Edge;

  friend class WSPD_spanner<Traits,R>;

  static WSPD_spanner_edge_iterator begin(const Well_separated_pair_decomposition& wspd_, const R& rep_chooser) {
    return WSPD_spanner_edge_iterator(wspd_, wspd_.begin(), rep_chooser);
  }

  static WSPD_spanner_edge_iterator end(const Well_separated_pair_decomposition& wspd_, const R& rep_chooser) {
    return WSPD_spanner_edge_iterator(wspd_, wspd_.end(), rep_chooser);
  }

  WSPD_spanner_edge_iterator(const Well_separated_pair_decomposition& wspd_, Well_separated_pair_iterator it_, const R& rep_chooser) :
                                      wspd(wspd_), it(it_), choose_rep(rep_chooser) {
    setup_edges_iterator();
  }

  void setup_edges_iterator() {
    a_rep.clear();
    a_rep_index = 0;
    b_rep.clear();
    b_rep_index = 0;
    if(it != wspd.end()) {
      choose_rep(it->a(), it->b(), std::back_inserter(a_rep));
      choose_rep(it->b(), it->a(), std::back_inserter(b_rep));
    }
  }
public:
  WSPD_spanner_edge_iterator& operator++() {
    if(b_rep_index == b_rep.size() - 1) {
      a_rep_index++;
      b_rep_index = 0;
    }
    else {
      b_rep_index++;
    }

    if(a_rep_index == a_rep.size()) {
      it++;
      setup_edges_iterator();
    }

    return *this;
  }
  inline WSPD_spanner_edge_iterator operator++(int) { WSPD_spanner_edge_iterator ret = *this; this->operator++(); return ret; }
  inline bool operator==(const WSPD_spanner_edge_iterator& rhs) const { return it == rhs.it && a_rep_index == rhs.a_rep_index && b_rep_index == rhs.b_rep_index; }
  inline bool operator!=(const WSPD_spanner_edge_iterator& rhs) const { return !this->operator==(rhs); }
  Node_const_handle from() const {
    return it->a();
  }
  Node_const_handle to() const {
    return it->b();
  }
  Edge operator*() const { return Edge(a_rep[a_rep_index], b_rep[b_rep_index], from(), to()); }
  Edge operator->() const { return **this; }
private:
  const Well_separated_pair_decomposition& wspd;
  const R& choose_rep;

  Point_vector a_rep;
  int a_rep_index;
  Point_vector b_rep;
  int b_rep_index;
  Well_separated_pair_iterator it;
};

template <class Traits, class R>
class WSPD_spanner_vertex_edge_iterator : public std::iterator<std::input_iterator_tag, WSPD_spanner_edge<Traits> > {
private:
  typedef CGAL::WSPD<Traits>                                         WSPD;
  typedef typename WSPD::Node_const_handle                           Node_const_handle;

  typedef typename Traits::Point_d                                   Point_d;

  typedef std::vector<Point_d>                                       Point_vector;
  typedef typename Point_vector::const_iterator                      Point_iterator;

  typedef std::vector<std::pair<Node_const_handle,Node_const_handle> > Edges;
  typedef typename Edges::const_iterator                               Edges_iterator;

  typedef WSPD_spanner_edge<Traits>                                  Edge;

  friend class WSPD_spanner<Traits,R>;

  static WSPD_spanner_vertex_edge_iterator begin(Point_d point, const Edges& edges, const R& rep_chooser) {
    return WSPD_spanner_vertex_edge_iterator(point, edges, edges.begin(), rep_chooser);
  }

  static WSPD_spanner_vertex_edge_iterator end(Point_d point, const Edges& edges, const R& rep_chooser) {
    return WSPD_spanner_vertex_edge_iterator(point, edges, edges.end(), rep_chooser);
  }

  WSPD_spanner_vertex_edge_iterator(Point_d point, const Edges& edges, Edges_iterator it, const R& rep_chooser) :
                                      p(point), e(edges), current_edge(it), choose_rep(rep_chooser) {
    setup_edges_iterator();
  }

  void setup_edges_iterator() {
    current_representatives.clear();
    current_representative_index = 0;
    if(current_edge != e.end()) {
      choose_rep(current_edge->second, current_edge->first, std::back_inserter(current_representatives));
    }
  }
public:
  WSPD_spanner_vertex_edge_iterator& operator++() {
    if(current_representative_index == current_representatives.size() - 1) {
      current_edge++;
      setup_edges_iterator();
    }
    else {
      current_representative_index++;
    }
    return *this;
  }
  inline WSPD_spanner_vertex_edge_iterator operator++(int) { WSPD_spanner_vertex_edge_iterator ret = *this; this->operator++(); return ret; }
  inline bool operator==(const WSPD_spanner_vertex_edge_iterator& rhs) const { return current_edge == rhs.current_edge && current_representative_index == rhs.current_representative_index; }
  inline bool operator!=(const WSPD_spanner_vertex_edge_iterator& rhs) const { return !this->operator==(rhs); }
  Node_const_handle from() const {
    return current_edge->first;
  }
  Node_const_handle to() const {
    return current_edge->second;
  }
  Edge operator*() const { return Edge(p, current_representatives[current_representative_index], from(), to()); }
  Edge operator->() const { return **this; }
private:
  Point_d p;
  const Edges& e;
  Edges_iterator current_edge;
  Point_vector current_representatives;
  int current_representative_index;
  const R& choose_rep;
};


template <class Traits>
struct WSPD_spanner_adjacency_iterator_function {
  typedef WSPD_spanner_edge<Traits> argument_type;
  typedef typename Traits::Point_d result_type;
  result_type operator() (argument_type edge) const {return edge.to;}
};

template <class Traits, class R>
struct WSPD_spanner_adjacency_iterator_transform {
  typedef WSPD_spanner_adjacency_iterator_function<Traits> Function;
  typedef WSPD_spanner_vertex_edge_iterator<Traits,R> Vertex_edge_iterator;
  typedef boost::transform_iterator<Function, Vertex_edge_iterator> type;
};

template <class Traits, class R>
class WSPD_spanner_adjacency_iterator : public WSPD_spanner_adjacency_iterator_transform<Traits,R>::type {
private:
  typedef WSPD_spanner_adjacency_iterator_transform<Traits,R> Transform;
  typedef typename Transform::type Base;
  typedef typename Transform::Function Function;
  typedef typename Transform::Vertex_edge_iterator Vertex_edge_iterator;

  friend class WSPD_spanner<Traits,R>;

  WSPD_spanner_adjacency_iterator(Vertex_edge_iterator it) : Base(it, Function()) { }
};

} // End namespace

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

#endif // CGAL_WSPD_SPANNER_H
