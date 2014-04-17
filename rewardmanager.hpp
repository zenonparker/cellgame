////////////////////////////////////////////////////////////////////////////////
///
/// @file rewardmanager.hpp
/// @brief Class that manages all rewards in the grid including the distribution
///        to and creation of rewards.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_REWARD_MANAGER_HPP
#define CELL_REWARD_MANAGER_HPP

#include <reward.hpp>
#include <vector>
#include <map>

namespace cell {

// TODO: Reward manager should be able to:
//    - Remove a reward and redistribute the remaining quantity.
//    - Generate a new reward based on some strategy for reward distribution.
//    - When removing a reward, first generate at least one replacement
//      in an appropriate allocation class.
//    - Distribute some provided quantity over existing rewards and potentially
//      generate new rewards if applicable.
//    - Track the total quantity in the world of rewards, and total quantity
//      in each reward allocation class.

class RewardManager {
public:

  RewardManager() : reward_levels_(Reward::RewardLevel::NUM_LEVELS) { }

  /// Used for testing purposes and perhaps loading from database. New rewards
  /// in the live game should be generated elsewhere by the RewardManager.
  Reward* add_reward(const Reward& reward);

  /** @brief Remove a reward.
    * @return True if a reward was removed, false otherwise.
    */
  bool remove_reward(const Reward& reward);

private:

  /// Each entry in the vector represents a "level" of reward type.
  std::vector<std::map<RewardId, Reward>> reward_levels_;

};

} // end namespace cell

#endif // CELL_REWARD_MANAGER_HPP

