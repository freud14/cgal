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

  WSPD_spanner(const WSPD& wspd_, const R& rep_chooser = R()) :
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
  const R& choose_rep;
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
    cur_reps.clear();
    cur_rep_i = 0;
    if(current_edge != e.end()) {
      choose_rep(current_edge->second, current_edge->first, std::back_inserter(cur_reps));
    }
  }
public:
  WSPD_spanner_vertex_edge_iterator& operator++() {
    if(cur_rep_i == cur_reps.size() - 1) {
      current_edge++;
      setup_edges_iterator();
    }
    else {
      cur_rep_i++;
    }
    return *this;
  }
  inline WSPD_spanner_vertex_edge_iterator operator++(int) { WSPD_spanner_vertex_edge_iterator ret = *this; this->operator++(); return ret; }
  inline bool operator==(const WSPD_spanner_vertex_edge_iterator& rhs) const { return current_edge == rhs.current_edge && cur_rep_i == rhs.cur_rep_i; }
  inline bool operator!=(const WSPD_spanner_vertex_edge_iterator& rhs) const { return !this->operator==(rhs); }
  Node_const_handle from() const {
    return current_edge->first;
  }
  Node_const_handle to() const {
    return current_edge->second;
  }
  Edge operator*() const { return Edge(p, cur_reps[cur_rep_i], from(), to()); }
  Edge operator->() const { return **this; }
private:
  Point_d p;
  const Edges& e;
  Edges_iterator current_edge;
  Point_vector cur_reps;
  int cur_rep_i;
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

#endif // CGAL_WSPD_SPANNER_H
