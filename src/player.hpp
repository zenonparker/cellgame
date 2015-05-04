////////////////////////////////////////////////////////////////////////////////
///
/// @file player.hpp
/// @brief Class representing a player in Cell.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_PLAYER_HPP
#define CELL_PLAYER_HPP

#include <cstdint>
#include <location.hpp>

namespace cell {

typedef int PlayerId;

class Player {
public:

  explicit Player(PlayerId pid);
  Player() : Player(-1) {}

  // Accessors
  PlayerId& id();
  const PlayerId& id() const;
  const Location& location() const;
  Location& location();

private:
  PlayerId id_;
  Location location_ = Location{0, 0};

};

inline Player::Player(PlayerId pid) : id_(pid) {
  // TODO: Location?
}

inline PlayerId& Player::id() { return id_;  }
inline const PlayerId& Player::id() const { return id_; }

inline const Location& Player::location() const { return location_; }
inline Location& Player::location() { return location_; }

} // end namespace cell

#endif // CELL_PLAYER_HPP

