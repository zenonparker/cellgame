////////////////////////////////////////////////////////////////////////////////
///
/// @file location.hpp
/// @brief Represents a location on the Cell game board (grid).
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_LOCATION_HPP
#define CELL_LOCATION_HPP

#include <iostream>
#include <math.h>

namespace cell {

struct Location {
  Location(int xin, int yin) : x(xin), y(yin) {}
  int x;
  int y;
  bool between(const Location& top_left, const Location& bottom_right);
  double distanceTo(const Location& loc) const;
};

inline bool Location::between(const Location& bottom_left, const Location& top_right) {
  return x >= bottom_left.x && x <= top_right.x && y >= bottom_left.y && y <= top_right.y;
}

inline double Location::distanceTo(const Location& loc) const {
  double xd = double(x - loc.x);
  double yd = double(y - loc.y);
  return sqrt(xd*xd + yd*yd);
}

inline bool operator==(const Location& l1, const Location& l2) {
  return (l1.x == l2.x && l1.y == l2.y);
}

inline bool operator<(const Location& l1, const Location& l2) {
  return (l1.x < l2.x ? true :
            l1.x == l2.x && l1.y < l2.y ? true : 
              false);
}

inline std::ostream& operator<<(std::ostream& out, const Location& l) {
  return out << "(" << l.x << ", " << l.y << ")";
}

} // end namespace cell

#endif // CELL_LOCATION_HPP

