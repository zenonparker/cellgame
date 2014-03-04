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

  void player_join(PlayerId id);
  void player_leave(PlayerId id);
  
  Grid& grid();
  const Grid& grid() const;

private:
  // No copy construction/assignment.
  World(const World&) = delete;
  World& operator=(const World&) = delete;
  std::map<PlayerId, Player> players_;
  Grid grid_;
};

inline void World::player_join(PlayerId id) {
  const auto &player = players_.emplace(id,Player()).first;
}

inline void World::player_leave(PlayerId id) {
  const auto cit = players_.find(id);
  if (cit != players_.end()) {
    grid_.player_grid().erase_one(cit->second.location(), &cit->second);
    players_.erase(cit);
  }
}

inline Grid& World::grid() { return grid_; }
inline const Grid& World::grid() const { return grid_; }

} // end namespace cell

#endif // CELL_WORLD_HPP

