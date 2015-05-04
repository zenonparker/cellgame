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
#include <location.hpp>

namespace cell {

typedef int RewardId;

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

  enum RewardLevel {
    GRAND = 0,   // One of these per game board.
    LARGE,       // Various other sizes, allocation strategy configurable.
    MEDIUM,
    SMALL,
    NUM_LEVELS
  };

  /// Constructor
  Reward(RewardId id, RewardType type) : id_(id), type_(type) {}

  /** @brief Obtain the influence that this reward is exerting on the given player.
    * @param player   Player whom is performing the scan.
    * @param minDist  Minimum distance for this scan.
    * @param maxDist  Maximum distance for this scan.
    * @return Influence value.
    */
  int get_influence(const Player& player, int minDist, int maxDist);

  /** @brief Obtains the value of this reward given that it was "hit" by a player
    *        at the provided location. The goal of this mechanic is to allow a player
    *        to obtain a portion of a reward based on how accurately they guessed the
    *        location.
    * @param loc Location from which the value of this reward is being queried.
    * @return Value of the reward as seen from the provided location. 
    *         Always in the range [0, quantity_].
    */
  int value_from_location(const Location& loc) const;

  // Accessors

  const RewardLevel& level() const { return level_; }
  RewardLevel& level() { return level_; }

  const int quantity() const { return quantity_; }
  int& quantity() { return quantity_; }

  const Location& location() const { return location_; }
  Location& location() { return location_; }

  const RewardType& type() const { return type_; }
  const RewardId& id() const { return id_; }

  friend std::ostream& operator<<(std::ostream& out, const Reward& r);

private:

  RewardId id_;
  RewardType type_;
  RewardLevel level_ = RewardLevel::SMALL;
  Location location_ = Location{0, 0};
  int quantity_ = 0;
  int scans_ = 0;
  uint64_t last_scanned_ = 0;

};

inline std::ostream& operator<<(std::ostream& out, const Reward& r) {
  return out << "Reward[" << r.id_ << "] {"
             << " type: " << int(r.type_)
             << " level: " << r.level_
             << " location: " << r.location_
             << " quantity: " << r.quantity_
             << " scans: " << r.scans_
             << " last_scanned: " << r.last_scanned_
             << " }";
}

} // end namespace cell

#endif // CELL_REWARD_HPP

