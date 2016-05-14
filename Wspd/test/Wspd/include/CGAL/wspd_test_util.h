#include <vector>
#include <functional>

namespace CGAL {

template <class Container, class T, class BinaryPredicate>
bool
in(const Container& container, const T& element, BinaryPredicate pred) {
  for(typename Container::const_iterator it = container.begin(); it != container.end(); it++) {
    if(pred(*it, element)) return true;
  }
  return false;
}

template <class Container, class T>
bool
in(const Container& container, const T& element) {
  return in(container, element, std::equal_to<T>());
}

template <class Container, class Container2, class BinaryPredicate>
bool
are_identical_set(const Container& container, const Container2& container2, BinaryPredicate pred) {
  if(container.size() != container2.size()) return false;

  for(typename Container2::const_iterator it = container2.begin(); it != container2.end(); it++) {
    if(!in(container, *it, pred)) return false;
  }
  return true;
}

template <class Container, class Container2>
bool
are_identical_set(const Container& container, const Container2& container2) {
  return are_identical_set(container, container2, std::equal_to<typename Container::value_type>());
}

template <class Traits>
typename Traits::Point_d
get_point_d(int d, typename Traits::RT a, typename Traits::RT b, const Traits& traits) {
  return get_point_d(d, a, b, 1, traits);
}

template <class Traits>
typename Traits::Point_d
get_point_d(int d, typename Traits::RT a, typename Traits::RT b, typename Traits::RT D, const Traits& traits) {
  std::vector<typename Traits::RT> coord(d, 0);
  coord[d - 2] = a*D;
  coord[d - 1] = b*D;
  return traits.construct_point_d_object()(d, coord.begin(), coord.end(), D);
}

}
