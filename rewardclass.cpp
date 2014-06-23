////////////////////////////////////////////////////////////////////////////////
///
/// @file rewardclass.cpp
/// @brief Implementation of RewardClass class.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

// cell
#include <rewardclass.hpp>

namespace cell {

RewardClass::RewardClass(const reward_vec_t&& rewards) : rewards_(std::move(rewards))
{
  cur_reward_ = rewards_.begin();
  total_quantity_ = 0;
  for (const auto& r : rewards_) total_quantity_ += r.quantity();
}

void RewardClass::distribute_quantity(uint64_t quantity)
{
  while (quantity > 0) {
    uint64_t avg = total_quantity_ / rewards_.size();
    while (quantity > 0 && cur_reward_ != rewards_.end()) {
      if (cur_reward_->quantity() <= avg) {
        uint64_t dist_amount = std::max(std::max(quantity / 10, uint64_t(1)),
                                        std::min(quantity, avg - cur_reward_->quantity()));
        total_quantity_ += dist_amount;
        quantity -= dist_amount;
      } else {
        // Only interate to the next reward if this one has met its target.
        ++cur_reward_;
      }
    }
    // If we get here and quantity isn't zero, we must've reached the end of our
    // vector of rewards. Recalculate the average and restart from the beginning.
    if (cur_reward_ == rewards_.end()) cur_reward_ = rewards_.begin();
  }
}

} // end namespace cell

