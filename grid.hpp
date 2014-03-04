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

#include <grid_map.hpp>
#include <grid_multimap.hpp>
#include <player.hpp>
#include <reward.hpp>
#include <scan.hpp>

namespace cell {

class Grid {
public:

  // Constructors
  Grid() { }

  /// Adds a reward to the grid at the given location.
  void add_reward(const Location& location, const Reward& reward);

  /// Removes a reward from the grid at a given location.
  void remove_reward(const Location& location);

  /** @brief Obtains a scan of the rewards around the given player.
    *        The location of the player is not verified in the grid, it
    *        is simply taken from the provided player object.
    */
  Scan scan_player(const Player& player) const;

  const GridMultiMap<Player*>& player_grid() const;
  GridMultiMap<Player*>& player_grid();

private:

  GridMap<Reward> rewards_;
  GridMultiMap<Player*> player_grid_;

  // Helpers
  InfluenceRing scan_ring(const Player& player, int min, int max) const;

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

