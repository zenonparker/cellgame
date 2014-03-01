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

namespace cell {

class World {
public:
  
  World() { }

private:

  // No copy construction/assignment.
  World(const World&) = delete;
  World& operator=(const World&) = delete;

};

} // end namespace cell

#endif // CELL_WORLD_HPP

