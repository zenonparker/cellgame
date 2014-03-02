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

namespace cell {

typedef int PlayerId;

class Player {
public:
	PlayerId& player_id();
	const PlayerId& player_id() const;
private:
	PlayerId player_id_;

};

inline PlayerId& Player::player_id() { return player_id_;  }
inline const PlayerId& Player::player_id() const { return player_id_; }

} // end namespace cell

#endif // CELL_PLAYER_HPP

