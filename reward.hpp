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

  /// Hit radius should be equivalent to the scan deadzone's radius.
  constexpr static double HIT_RADIUS = 10.0;

  /// The exponent in the equation for reward value based on distance.
  /// value = (((radius - distance) / radius) ^ falloff) * quantity
  constexpr static double HIT_VALUE_FALLOFF = 3.0;

  /// Time after a reset of last scanned that this reward will exert full
  /// influence again. (in milliseconds)
  constexpr static uint64_t RECOVERY_TIME = 60000;

  /// After this many scans the reward will reset influence and gradually
  /// regain it over RECOVERY_TIME milliseconds.
  constexpr static int NUM_SCANS_BEFORE_RESET = 3;

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
    * @param minDist  Minimum distance for this scan.
    * @param maxDist  Maximum distance for this scan.
    * @return Influence value.
    */
  int get_influence(const Player& player, const Location& loc, int minDist, int maxDist);

  /** @brief Obtains the value of this reward given that it was "hit" by a player
    *        at the provided location. The goal of this mechanic is to allow a player
    *        to obtain a portion of a reward based on how accurately they guessed the
    *        location.
    * @param qloc Location from which the value of this reward is being queried.
    * @param rloc Location of this reward.
    * @return Value of the reward as seen from the provided location. 
    *         Always in the range [0, quantity_].
    */
  int value_from_location(const Location& qloc, const Location& rloc) const;

private:

  RewardType type_;
  int quantity_;
  int scans_ = 0;
  uint64_t last_scanned_ = 0;

};

} // end namespace cell

#endif // CELL_REWARD_HPP

