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

class RewardManager {
public:

  /// Default allocation level for rewards.
  constexpr static unsigned int DEFAULT_LEVEL = 3;

  /// Number of allocation levels for rewards.
  constexpr static unsigned int NUM_LEVELS = 4;
  // 0 = Grand prize.
  // 1 = Large prize.
  // 2 = Medium prize.
  // 3 = Small prize.

  RewardManager() : reward_levels_{NUM_LEVELS} { }

  /// Used for testing purposes and perhaps loading from database. New rewards
  /// in the live game should be generated elsewhere by the RewardManager.
  Reward* add_reward(const Reward& reward, unsigned int level);

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

