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

Reward* RewardManager::add_reward(const Reward& reward, unsigned int level)
{
  if (level > NUM_LEVELS) return NULL;
  auto ret = reward_levels_[level].emplace(reward.id(), reward);
  return &(ret.first->second);
}

bool RewardManager::remove_reward(const Reward& reward)
{
  return false;
}

} // end namespace cell

