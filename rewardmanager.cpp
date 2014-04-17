////////////////////////////////////////////////////////////////////////////////
///
/// @file rewardmanager.cpp
/// @brief Implementation of RewardManager class.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

// cell
#include <rewardmanager.hpp>

namespace cell {

Reward* RewardManager::add_reward(const Reward& reward)
{
  auto ret = reward_levels_[reward.level()].emplace(reward.id(), reward);
  return &(ret.first->second);
}

bool RewardManager::remove_reward(const Reward& reward)
{
  return false;
}

} // end namespace cell

