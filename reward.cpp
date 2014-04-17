////////////////////////////////////////////////////////////////////////////////
///
/// @file reward.cpp
/// @brief Implementation of Reward class.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

// std
#include <algorithm>
#include <chrono>

// cell
#include <reward.hpp>
#include <scan.hpp>

namespace cell {

int Reward::get_influence(const Player& player, int minDist, int maxDist)
{
  int influence = 0;

  uint64_t cur_time = std::chrono::duration_cast<std::chrono::milliseconds>
                        (std::chrono::system_clock::now().time_since_epoch()).count();
  double dist_range = maxDist - minDist;

  switch (type_) {
    case RewardType::TRIVIAL:
      influence = quantity_;
      break;
    case RewardType::DISTANCE:
      {
        double dist = player.location().distanceTo(location_);
        double dist_mod = 1.0 - std::max(0.0, std::min(1.0, dist / dist_range));
        influence = int(double(quantity_) * dist_mod);
      }
      break;
    case RewardType::DIST_TIME:
      {
        double dist = player.location().distanceTo(location_);
        double dist_mod = 1.0 - std::max(0.0, std::min(1.0, dist / dist_range));
        double time_mod = std::min(double(cur_time - last_scanned_) / double(RECOVERY_TIME), 1.0);
        influence = int(double(quantity_) * time_mod * dist_mod);
      }
      break;
    default:
      influence = quantity_;
  }

  scans_++;
  if (scans_ >= NUM_SCANS_BEFORE_RESET) {
    last_scanned_ = cur_time;
    scans_ = 0;
  }

  return influence;
}

int Reward::value_from_location(const Location& loc) const
{
  double dist = loc.distanceTo(location_);
  double reward_percent = pow(((HIT_RADIUS - dist) / HIT_RADIUS), HIT_VALUE_FALLOFF);

  // Minimum reward from any hit is always one!
  return std::max(dist <= HIT_RADIUS ? 1 : 0, int(reward_percent * double(quantity_)));
}

} // end namespace cell

