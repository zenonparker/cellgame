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

#include <grid_multimap.hpp>
#include <player.hpp>

namespace cell {

class Grid {
public:

  Grid() { }

  const GridMultiMap<Player*>& player_grid() const;
  GridMultiMap<Player*>& player_grid();

private:

  GridMultiMap<Player*> player_grid_;

  // No copy construction/assignment.
  Grid(const Grid&) = delete;
  Grid& operator=(const Grid&) = delete;

};

const GridMultiMap<Player*>& Grid::player_grid() const {
  return player_grid_;
}

GridMultiMap<Player*>& Grid::player_grid() {
  return player_grid_;
}

} // end namespace cell

#endif // CELL_GRID_HPP

