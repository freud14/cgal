#ifndef _COMPLEX_2_IN_TRIANGULATION_3_SURFACE_MESH_H
#define _COMPLEX_2_IN_TRIANGULATION_3_SURFACE_MESH_H

#include <CGAL/Complex_2_in_triangulation_3.h>

CGAL_BEGIN_NAMESPACE

template < class Tr >
class Complex_2_in_triangulation_3_surface_mesh :
public Complex_2_in_triangulation_3 <Tr> {
  
 public:
  
  typedef Complex_2_in_triangulation_3_surface_mesh < Tr > Self;
  
  typedef Complex_2_in_triangulation_3 <Tr> Base;

  typedef Tr Triangulation_3;
  
  typedef typename Triangulation_3::Point Point;
  
  typedef typename Triangulation_3::Facet Facet;
  
  typedef typename Triangulation_3::Edge Edge;

  typedef typename Triangulation_3::Vertex_handle Vertex_handle;

  typedef typename Triangulation_3::Cell_handle Cell_handle;

  typedef typename Triangulation_3::Geom_traits GT;

  typedef typename GT::FT FT;

  typedef std::list<Facet> Facets;

  typedef Const_circulator_from_container<Facets> Facet_circulator;

  typedef std::list<Vertex_handle> Vertices;

  typedef typename Vertices::iterator Vertices_iterator;

  // Constructors
  Complex_2_in_triangulation_3_surface_mesh(Triangulation_3& t3)
    :Complex_2_in_triangulation_3<Triangulation_3>(t3){
  }
  
  // Access functions
  
  bool is_facet_visited (const Facet& f) const {
    return (f.first)->is_facet_visited(f.second);
  }
  
  int nb_visits (const Facet& f) const {
    return f.first->nb_visits(f.second);
  }
  
  Point get_facet_surface_center (const Facet& f) const {
    return (f.first)->get_facet_surface_center(f.second);
  }
  
  int get_facet_surface_status(const Facet& f) const {
    return (f.first)->get_facet_surface_status(f.second);
  }
  
  template <class OutputIterator>
  OutputIterator
  incident_vertices(const Vertex_handle& v, OutputIterator voit) const {
    
    triangulation().incident_vertices(v, filter_output_iterator(voit, Not_in_complex()));
    
    return voit;
  }

  Facet_circulator incident_facets (const Edge& e) {
    return Base::incident_facets(e);
  }

  template <typename OutputIterator>
  OutputIterator incident_facets(const Vertex_handle v, OutputIterator it) const {
    return Base::incident_facets(v,it);
  }
  

  FT compute_distance_to_facet_center(const Facet& f, const Vertex_handle v) const {
    Point fcenter = get_facet_surface_center(f);
    Point vpoint = v->point();

    GT gt = Base::tri3.geom_traits();
    
    return gt.compute_squared_distance_3_object()( fcenter, vpoint );
  }

  // Setting functions
  
  void set_facet_visited (const Facet& f) {
    (f.first)->set_facet_visited(f.second);
  }
  
  void inc_visits (const Facet& f) {
    (f.first)->inc_visits(f.second);
  }
  
  void reset_visits (const Facet& f) {
    (f.first)->reset_visits(f.second);
  }
  
  void set_facet_surface_center(const Facet& f, const Point& p, const int status=0) {
    (f.first)->set_facet_surface_center(f.second, p, status);
  }
};

CGAL_END_NAMESPACE

#endif // COMPLEX_2_IN_TRIANGULATION_3_SURFACE_MESH_H
