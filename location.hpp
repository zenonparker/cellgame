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

namespace cell {

struct Location {
  Location(int xin, int yin) : x(xin), y(yin) {}
  int x;
  int y;
};

bool operator==(const Location& l1, const Location& l2) {
  return (l1.x == l2.x && l1.y == l2.y);
}

bool operator<(const Location& l1, const Location& l2) {
  return (l1.x < l2.x ? true :
            l1.x == l2.x && l1.y < l2.y ? true : 
              false);
}

std::ostream& operator<<(std::ostream& out, const Location& l) {
  return out << "(" << l.x << ", " << l.y << ")";
}

} // end namespace cell

#endif // CELL_LOCATION_HPP

