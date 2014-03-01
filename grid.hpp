////////////////////////////////////////////////////////////////////////////////
///
/// @file grid.hpp
/// @brief Representation of the "game board" for Cell.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_GRID_HPP
#define CELL_GRID_HPP

namespace cell {

class Grid {
public:

  Grid() { }

private:

  // No copy construction/assignment.
  Grid(const Grid&) = delete;
  Grid& operator=(const Grid&) = delete;

};

} // end namespace cell

#endif // CELL_GRID_HPP

