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
#include <chrono>

// cell
#include <reward.hpp>

namespace cell {

int Reward::get_influence(const Player& player, const Location& loc)
{
  int influence = 0;

  uint64_t cur_time = std::chrono::duration_cast<std::chrono::milliseconds>
                        (std::chrono::system_clock::now().time_since_epoch()).count();

  switch (type_) {
    case RewardType::TRIVIAL:
      influence = quantity_;
      break;
    case RewardType::DISTANCE:
      {
        double dist_mod = 1.0;
        influence = int(double(quantity_) * dist_mod);
      }
      break;
    case RewardType::DIST_TIME:
      {
        double time_mod = std::min(double(cur_time - last_scanned_) / double(RECOVERY_TIME), 1.0);
        double dist_mod = 1.0;
        influence = int(double(quantity_) * time_mod * dist_mod);
      }
      break;
    default:
      influence = quantity_;
  }

  scans_++;
  if (scans_ > NUM_SCANS_BEFORE_RESET) {
    last_scanned_ = cur_time;
    scans_ = 0;
  }

  return influence;
}

} // end namespace cell

