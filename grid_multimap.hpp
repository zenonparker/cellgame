////////////////////////////////////////////////////////////////////////////////
///
/// @file grid_multi_map.hpp
/// @brief Helper class for 2d selection operations
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_GRID_MULTIMAP_HPP
#define CELL_GRID_MULTIMAP_HPP

#include <map>
#include <vector>
#include <location.hpp>

namespace cell {

  template<class T>
  class GridMultiMap {
  public:
    void insert(const Location &l, const T &t);
    void erase_one(const Location &l, const T &t);
    void erase_all(const Location &l, const T &t);
    std::vector<std::pair<Location, T> >  find(const Location &l1, const Location &l2) const;
    std::vector<std::pair<Location, T*> >  find_ptr(const Location &l1, const Location &l2);
  private:
    std::multimap<Location, T> grid_map_;
  };

  template<class T>
  void GridMultiMap<T>::insert(const Location &l, const T &t) {
    grid_map_.emplace(l, t);
  }

  template<class T>
  void GridMultiMap<T>::erase_one(const Location &l, const T &t) {
    auto begin = grid_map_.lower_bound(l);
    auto end   = grid_map_.upper_bound(l);
    for (auto it = begin; it != end;) {
      if (it->second == t) {
        grid_map_.erase(it);
        it = end;
      }
      else {
        ++it;
      }
    }
  }

  template<class T>
  void GridMultiMap<T>::erase_all(const Location &l, const T &t) {
    auto begin = grid_map_.lower_bound(l);
    auto end = grid_map_.upper_bound(l);
    for (auto it = begin; it != end;) {
      if (it->second == t) {
        grid_map_.erase(it++);
      }
      else {
        ++it;
      }
    }
  }

  template<class T>
  std::vector<std::pair<Location, T> >  GridMultiMap<T>::find(const Location &bottom_left, const Location &top_right) const {
    if (top_right.x < bottom_left.x || bottom_left.y > top_right.y) {
      return{};
    }
    const auto begin = grid_map_.lower_bound(bottom_left);
    auto end = grid_map_.upper_bound(top_right);
    if (begin == end && begin == grid_map_.end()) {
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
  std::vector<std::pair<Location, T*> >  GridMultiMap<T>::find_ptr(const Location &bottom_left, const Location &top_right) {
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
        ret.emplace_back(RetType::value_type{ it->first, &(it->second) });
      }
    }
    return ret;
  }

} // end namespace cell

#endif // CELL_GRID_HPP

