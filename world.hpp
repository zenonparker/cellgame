////////////////////////////////////////////////////////////////////////////////
///
/// @file world.hpp
/// @brief Handles the state of the world in the Cell game.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_WORLD_HPP
#define CELL_WORLD_HPP

#include <player.hpp>
#include <grid.hpp>
#include <map>

namespace cell {

class World {
public:
  
  World() { }

  /** @brief Add a player to the world.
    * @return True if the player was added, false if they already existed.
    */
  bool player_join(PlayerId id);

  /** @brief Remove a player from the world.
    * @return True if the player was removed, false otherwise.
    */
  bool player_leave(PlayerId id);
  
  Grid& grid();
  const Grid& grid() const;

private:
  // No copy construction/assignment.
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  std::map<PlayerId, Player> players_;
  Grid grid_;
};

inline bool World::player_join(PlayerId id) {
  auto ret = players_.emplace(id,Player(id));
  if (ret.second) {
    auto &player = ret.first->second;
    grid_.player_grid().insert(player.location(), &player);
  }
  return ret.second;
}

inline bool World::player_leave(PlayerId id) {
  const auto cit = players_.find(id);
  if (cit != players_.end()) {
    grid_.player_grid().erase_one(cit->second.location(), &cit->second);
    players_.erase(cit);
    return true;
  }
  return false;
}

inline Grid& World::grid() { return grid_; }
inline const Grid& World::grid() const { return grid_; }

} // end namespace cell

#endif // CELL_WORLD_HPP

