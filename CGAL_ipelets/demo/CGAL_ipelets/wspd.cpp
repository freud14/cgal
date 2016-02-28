#include <iostream>
#include <CGAL/Cartesian_d.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/number_utils.h>
#include <CGAL/CGAL_Ipelet_base.h>
#include <CGAL/Split_tree.h>
#include <CGAL/WSPD.h>
#include <CGAL/Split_tree_traits_2.h>
#include <cfloat>

namespace CGAL_wspd {

typedef CGAL::Exact_predicates_inexact_constructions_kernel     Epick;
typedef CGAL::Split_tree_traits_2<Epick>                        Kernel;
typedef CGAL::Split_tree<Kernel>                                Split_tree;
typedef CGAL::WSPD<Kernel>                                      WSPD;
typedef typename WSPD::Node                                     Node;
typedef typename WSPD::Well_separated_pair                      Well_separated_pair;

const std::string labels[] = {  "Split tree bounding boxes", "WSPD", "t-spanner (using t)", "t-spanner (using s)", "Help" };
const std::string hmsg[] = {
  "Compute the bounding boxes of a split tree.",
  "Compute a WSPD computed with a split tree.",
  "Compute a t-spanner using a WSPD computed with a split tree (using t).",
  "Compute a t-spanner using a WSPD computed with a split tree (using s)."
};

class Wspd_ipelet
  : public CGAL::Ipelet_base<Epick,5>{
public:
  Wspd_ipelet()
    :CGAL::Ipelet_base<Epick,5>("WSPD",labels,hmsg){}
  void protected_run(int);
private:
  double request_double_from_user(double min, double default_value, const std::string& message, const char*) const;
  Segment_2 segment_between_circles(const Circle_2& c1, const Circle_2& c2) const;
};

void Wspd_ipelet::protected_run(int fn)
{
  std::vector<Point_2> lst;
  switch (fn){
    case 0:
    case 1:
    case 2:
    case 3:
    {
      std::vector<Point_2> points_read;
      read_active_objects(
        CGAL::dispatch_or_drop_output<Point_2>(std::back_inserter(points_read))
      );

      if (points_read.empty()) {
        print_error_message("No mark selected");
        return;
      }
      for(std::vector<Point_2>::iterator it = points_read.begin(); it < points_read.end(); it++) {
        if(std::find(points_read.begin(), it, *it) == it) {
          lst.push_back(*it);
        }
      }
      break;
    }
    case 4:
      show_help();
      return;
  }

  switch (fn){
    case 0:
    {
      Split_tree tree(2, lst.begin(), lst.end());
      std::vector<Iso_rectangle_2> rect;
      tree.bounding_boxes(std::back_inserter(rect));
      for(std::vector<Iso_rectangle_2>::iterator it = rect.begin(); it < rect.end(); it++) {
        draw_in_ipe(*it);
      }
      group_selected_objects_();
      return;
    }
    case 1:
    {
      double s = request_double_from_user(1.0, 2.0, "Enter separation ratio s (default: 2)", "Invalid separation ratio");
      if(s == -1) return;
      WSPD wspd(2, s, lst.begin(), lst.end());
      std::vector<Well_separated_pair> pairs;
      wspd.compute(std::back_inserter(pairs));
      for(std::vector<Well_separated_pair>::iterator it = pairs.begin(); it < pairs.end(); it++) {
        Well_separated_pair &pair = *it;
        Circle_2 c1 = pair.first->enclosing_circle();
        Circle_2 c2 = pair.second->enclosing_circle();
        if(!pair.first->is_leaf()) {
          draw_in_ipe(c1);
        }
        if(!pair.second->is_leaf()) {
          draw_in_ipe(c2);
        }
        draw_in_ipe(segment_between_circles(c1, c2));
      }
      group_selected_objects_();
      return;
    }
    case 2:
    case 3:
    {
      double s;
      if(fn == 2) {
        double t = request_double_from_user(1.0+DBL_EPSILON, 1.0+DBL_EPSILON, "Enter spanning ratio t (default: 1 + Epsilon)", "Invalid spanning ratio");
        if(t == -1) return;
        s = 4*(t+1)/(t-1);
      }
      else {
        s = request_double_from_user(1.0, 2.0, "Enter separation ratio s (default: 2)", "Invalid separation ratio");
      }

      WSPD wspd(2, s, lst.begin(), lst.end());
      std::vector<Well_separated_pair> pairs;
      wspd.compute(std::back_inserter(pairs));
      for(std::vector<Well_separated_pair>::iterator it = pairs.begin(); it < pairs.end(); it++) {
        Well_separated_pair &pair = *it;
        draw_in_ipe(Segment_2(**pair.first->point_container().begin(), **pair.second->point_container().begin()));
      }
      group_selected_objects_();
      return;
    }
  }
}

double Wspd_ipelet::request_double_from_user(double min, double default_value, const std::string& message, const char* invalid) const{
  std::pair<int, double> res = request_value_from_user<double>(message);
  if(res.first == -1 || (res.first == 1 && res.second < min)) {
    print_error_message(invalid);
    return -1;
  }
  else if(res.first == 1) {
    return res.second;
  }
  return default_value;
}

Wspd_ipelet::Segment_2 Wspd_ipelet::segment_between_circles(const Circle_2& c1, const Circle_2& c2) const {
  Point_2 p1 = c1.center();
  Point_2 p2 = c2.center();
  FT dx = p1.x() - p2.x();
  FT dy = p1.y() - p2.y();
  FT length = CGAL::sqrt(CGAL::squared_distance(p1, p2));
  FT r1 = CGAL::sqrt(c1.squared_radius());
  FT r2 = CGAL::sqrt(c2.squared_radius());
  Point_2 s1 = Point_2(p1.x() - r1/length*dx, p1.y() - r1/length*dy);
  Point_2 s2 = Point_2(p1.x() - (1-r2/length)*dx, p1.y() - (1-r2/length)*dy);
  return Segment_2(s1, s2);
}

}

CGAL_IPELET(CGAL_wspd::Wspd_ipelet)
