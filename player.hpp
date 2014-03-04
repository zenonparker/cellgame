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
  Player();
  PlayerId& player_id();
  const PlayerId& player_id() const;
  const Location& location() const;
  Location& location();

private:
  PlayerId player_id_;
  Location location_;

};

inline Player::Player() :location_(0, 0){ /*TODO STUB*/ }

inline PlayerId& Player::player_id() { return player_id_;  }
inline const PlayerId& Player::player_id() const { return player_id_; }

inline const Location& Player::location() const { return location_; }
inline Location& Player::location() { return location_; }

} // end namespace cell

#endif // CELL_PLAYER_HPP

