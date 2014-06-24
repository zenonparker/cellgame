////////////////////////////////////////////////////////////////////////////////
///
/// @file rewardclass.hpp
/// @brief This class manages all rewards of a given allocation class.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_REWARD_CLASS_HPP
#define CELL_REWARD_CLASS_HPP

#include <reward.hpp>
#include <vector>

namespace cell {

class RewardClass {
public:
  typedef std::vector<Reward> reward_vec_t;

  /// Create a RewardClass to take ownership of a given vector of rewards.
  RewardClass(const reward_vec_t&& rewards);

  /// Distributes a given quantity.
  void distribute_quantity(uint64_t quantity);

  // Accessors
  const uint64_t total_quantity() const { return total_quantity_; }
  uint64_t& total_quantity() { return total_quantity_; }
  const reward_vec_t& rewards() const { return rewards_; }

private:

  reward_vec_t rewards_;
  reward_vec_t::iterator cur_reward_;
  uint64_t total_quantity_ = 0;

};

} // end namespace cell

#endif // CELL_REWARD_CLASS_HPP

