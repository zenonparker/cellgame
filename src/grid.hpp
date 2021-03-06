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

// std
#include <random>

// cell
#include <grid_map.hpp>
#include <grid_multimap.hpp>
#include <player.hpp>
#include <reward.hpp>
#include <scan.hpp>
#include <rewardmanager.hpp>

namespace cell {

class Grid {
public:

  // Constructors
  Grid() { rand_gen_.seed(rand_device_()); }

  /// Adds a reward to the grid. The location at which this reward will be
  /// added is stored inside the Reward itself.
  void add_reward(const Reward& reward);

  /// Removes a reward from the grid at a given location.
  void remove_reward(const Location& location);

  /** @brief Obtains a scan of the rewards around the given player.
    *        The location of the player is not verified in the grid, it
    *        is simply taken from the provided player object.
    */
  Scan scan_player(const Player& player);

  /** @brief Same functionality as scan_player EXCEPT does not randomize
    *        the radii of the scan rings.
    */
  Scan scan_player_fixed(const Player& player);

  /** @brief Obtains influence values for a given range around a player.
    *        This function will typically be used by scan_player to obtain
    *        a full scan.
    */
  InfluenceRing scan_single_ring(const Player& player, int minDist, int maxDist);

  /** @brief Used for testing purposes, reset the random number generator with
    *        a given seed.
    */
  void reset_seed(std::mt19937::result_type seed);

  // Basic Accessors
  const GridMultiMap<Player*>& player_grid() const { return player_grid_; }
  GridMultiMap<Player*>& player_grid() { return player_grid_; }

private:

  // Utilities for random initialization
  std::random_device rand_device_;
  std::mt19937 rand_gen_;

  RewardManager reward_man_;
  GridMap<Reward*> reward_grid_;
  GridMultiMap<Player*> player_grid_;

  // No copy construction/assignment.
  Grid(const Grid&) = delete;
  Grid& operator=(const Grid&) = delete;

};

} // end namespace cell

#endif // CELL_GRID_HPP

