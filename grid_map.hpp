////////////////////////////////////////////////////////////////////////////////
///
/// @file grid_map.hpp
/// @brief Helper class for 2d selection operations
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_GRID_MAP_HPP
#define CELL_GRID_MAP_HPP

#include <map>
#include <vector>
#include <location.hpp>

namespace cell {

template<class T>
class GridMap {
public:
  void insert(const Location &l, const T &t);
  void erase(const Location &l);
  std::vector<T> find(const Location &l1, const Location &l2);
private:
  std::map<Location,T> grid_map_;
};

template<class T>
void GridMap<T>::insert(const Location &l, const T &t) {
  grid_map_.emplace(l, t);
}

template<class T>
void GridMap<T>::erase(const Location &l) {
  grid_map_.erase(l);
}

template<class T>
std::vector<T> GridMap<T>::find(const Location &l1, const Location &l2) {
  const auto begin = grid_map_.lower_bound(l1);
  auto end   = grid_map_.upper_bound(l2);
  std::vector<T> ret;
  if (begin == end && begin == grid_map_.end()) {
    return ret;
  }
  for (auto it = begin; it != end; ++it) {
    if (it->first.y >= l1.y && it->first.y <= l2.y) {
      ret.emplace_back(it->second);
    }
  }
  return ret;
}

} // end namespace cell

#endif // CELL_GRID_HPP

