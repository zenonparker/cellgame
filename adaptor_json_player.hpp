////////////////////////////////////////////////////////////////////////////////
///
/// @file adaptor_json_player.hpp
/// @brief Allows for a player to be converted to it's json format and vice-versa
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef ADAPTOR_JSON_PLAYER_HPP
#define ADAPTOR_JSON_PLAYER_HPP

#include <player.hpp>
#include <util_js.hpp>
#include <util_swiss.hpp>

namespace cell {
  inline bool adaptor(const Player &player, util::JS::Node &n) {
    //TODO
    return true;
  }

  inline bool adaptor(util::JS::Node &n, Player &player) {
    player.player_id() = util::Swiss::throw_null(n["id"])->int32();
    player.location() = Location(util::Swiss::throw_null(n["x"])->int32(), util::Swiss::throw_null(n["y"])->int32());
    return true;
  }

} // end namespace cell

#endif // ADAPTOR_JSON_PLAYER_HPP

