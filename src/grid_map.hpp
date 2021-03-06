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

  /** @brief Removes and returns a copy of the removed object in the 
    *        reference provided.
    * @return True if an item was found and removed, false otherwise.
    */
  bool remove(const Location &l, T& t);

  /** @brief Obtains a vector of all objects in this grid between two locations.
    *        It is important that the locations are provided in the correct order.
    *        Range is INCLUSIVE.
    * @param bottom_left   BOTTOM LEFT location of the rectangle of points to search.
    * @param top_right     TOP RIGHT location of the rectangle of points to search.
    * @return Vector of all objects found in the range.
    */
  std::vector<std::pair<Location, T> > find(const Location &bottom_left, const Location &top_right) const;
  std::vector<std::pair<Location, T*> > find_ptr(const Location &bottom_left, const Location &top_right);

private:

  std::map<Location,T> grid_map_;

};

////////////////////////////////////////////////////////////////////////////////
///                             IMPLEMENTATION                               ///
////////////////////////////////////////////////////////////////////////////////

template<class T>
void GridMap<T>::insert(const Location &l, const T &t) {
  grid_map_.emplace(l, t);
}

template<class T>
void GridMap<T>::erase(const Location &l) {
  grid_map_.erase(l);
}

template<class T>
bool GridMap<T>::remove(const Location &l, T& t) {
  const auto it = grid_map_.find(l);
  if (it != grid_map_.end()) {
    t = it->second;
    grid_map_.erase(it);
    return true;
  }
  return false;
}

template<class T>
std::vector<std::pair<Location,T> > GridMap<T>::find(const Location &bottom_left, const Location &top_right) const {
  if (top_right.x < bottom_left.x || bottom_left.y > top_right.y) {
    return{};
  }
  const auto begin = grid_map_.lower_bound(bottom_left);
  auto end = grid_map_.upper_bound(top_right);
  if ( begin == end && begin == grid_map_.end() ) {
    return{};
  }
  std::vector<std::pair<Location, T> > ret;
  for (auto it = begin; it != end; ++it) {
    if (it->first.y >= bottom_left.y && it->first.y <= top_right.y) {
      ret.emplace_back(*it);
    }
  }
  return ret;
}

template<class T>
std::vector<std::pair<Location, T*> > GridMap<T>::find_ptr(const Location &bottom_left, const Location &top_right) {
  if (top_right.x < bottom_left.x || bottom_left.y > top_right.y) {
    return{};
  }
  const auto begin = grid_map_.lower_bound(bottom_left);
  auto end = grid_map_.upper_bound(top_right);
  if (begin == end && begin == grid_map_.end()) {
    return{};
  }
  typedef std::vector<std::pair<Location, T*> > RetType;
  RetType ret;
  for (auto it = begin; it != end; ++it) {
    if (it->first.y >= bottom_left.y && it->first.y <= top_right.y) {
      ret.emplace_back(it->first, &(it->second));
    }
  }
  return ret;
}

} // end namespace cell

#endif // CELL_GRID_HPP

