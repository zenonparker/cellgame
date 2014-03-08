////////////////////////////////////////////////////////////////////////////////
///
/// @file reward.hpp
/// @brief Representation of a reward/prize in the Cell game.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_REWARD_HPP
#define CELL_REWARD_HPP

#include <player.hpp>

namespace cell {

class Reward {
public:

  enum class RewardType {
    TRIVIAL,     // Should probably only be used for testing purposes, simply
                 // returns the quantity with no modifiers whatsoever.
    DISTANCE,    // Reduction in influence based on the distance from the player.
    DIST_TIME    // DISTANCE + Reduced influence based on how recently it has
                 // been scanned.
  };

  // Constructors
  explicit Reward(int quantity) : quantity_(quantity), type_(RewardType::TRIVIAL) { }
  Reward(int quantity, RewardType type) : quantity_(quantity), type_(type) { }

  /** @brief Obtain the influence that this reward is exerting on the given player.
    * @param player   Player whom is performing the scan.
    * @param loc      Location of this reward.
    * @return Influence value.
    */
  int get_influence(const Player& player, const Location& loc);

private:

  RewardType type_;
  int quantity_;
  uint64_t last_scanned_;

};

} // end namespace cell

#endif // CELL_REWARD_HPP

