////////////////////////////////////////////////////////////////////////////////
///
/// @file reward.cpp
/// @brief Implementation of Reward class.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#include <reward.hpp>

namespace cell {

int Reward::get_influence(const Player& player, const Location& loc)
{
  switch (type_) {
    case RewardType::TRIVIAL:
      return quantity_;
      break;
    case RewardType::DISTANCE:
      break;
    case RewardType::DIST_TIME:
      break;
    default:
      return quantity_;
  }
}

} // end namespace cell

