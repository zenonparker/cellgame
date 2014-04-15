////////////////////////////////////////////////////////////////////////////////
///
/// @file grid.cpp
/// @brief Implementation of Grid class.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#include <grid.hpp>

namespace {

constexpr static double PI = 3.141592653589793238462;
constexpr static double PI2_BY_DIRS = (PI * 2.0) / double(cell::InfluenceRing::NUM_DIRECTIONS * 2);

}

namespace cell {

void Grid::add_reward(const Reward& reward, unsigned int level /* = RewardManager::DEFAULT_LEVEL */)
{
  reward_grid_.insert(reward.location(), reward_man_.add_reward(reward, level));
}

void Grid::remove_reward(const Location& location)
{
  Reward* r;
  if (reward_grid_.remove(location, r)) {
    reward_man_.remove_reward(*r);
  }
}

Scan Grid::scan_player(const Player& player)
{
  Scan result;
  for (int i = 0; i < Scan::NUM_RINGS; ++i) {
    // A scan with range [min, max] will be [min +- var, max +- var].
    std::uniform_int_distribution<int> dist(-Scan::RING_RANGE_VARIANCE[i],
                                             Scan::RING_RANGE_VARIANCE[i]);
    // Declaring these beforehand rather than inline so that we are guaranteed
    // the order of the random number generations (this makes unit testing with
    // a given seed possible).
    int var1 = dist(rand_gen_);
    int var2 = dist(rand_gen_);
    result.rings()[i] = scan_single_ring(player, 
                                         Scan::RING_RANGES[i*2] + var1,
                                         Scan::RING_RANGES[(i*2)+1] + var2);
  }
  return result;
}

Scan Grid::scan_player_fixed(const Player& player)
{
  Scan result;
  for (int i = 0; i < Scan::NUM_RINGS; ++i) {
    result.rings()[i] = scan_single_ring(player, 
                                         Scan::RING_RANGES[i*2],
                                         Scan::RING_RANGES[(i*2)+1]);
  }
  return result;
}

InfluenceRing Grid::scan_single_ring(const Player& player, int minDist, int maxDist)
{
  InfluenceRing result;

  const Location& ploc = player.location();

  // Obtain vector of possible rewards based on the rectangle around the player's
  // location that encompasses the largest scan ring.
  Location bottom_left{ploc.x - maxDist, ploc.y - maxDist};
  Location top_right{ploc.x + maxDist, ploc.y + maxDist};
  auto potential_rewards = reward_grid_.find(bottom_left, top_right);

  for (auto& reward_pair : potential_rewards) {
    const Location& loc = reward_pair.first;

    // Distance check first.
    double dist = ploc.distanceTo(loc);

    if (dist >= double(minDist) && dist <= double(maxDist)) {

      // Satisfied distance contraint, determine which slice of 
      // the scan pie this rewards falls into.
      double rads = atan2(double(loc.y - ploc.y), double(loc.x - ploc.x));
      int slice = (int(rads / PI2_BY_DIRS) + 
                   (InfluenceRing::NUM_DIRECTIONS * 2)) % 
                    (InfluenceRing::NUM_DIRECTIONS * 2);

      // slice is now in range [0, num_dirs*2], actual slice to go in will be
      // ((slice + 1) / 2) % num_dirs
      // For example, slice index 15 = (15 + 1)/2 = 8 % 8 = zeroth slice. (east)
      //              slice index 0 = (0 + 1)/2 = 0.5 % 8 = zeroth slice. (east)
      //              slice index 1 = (1 + 1)/2 = 1 % 8 = index 1 slice (north-east)
      result.vals()[ ((slice + 1) / 2) % InfluenceRing::NUM_DIRECTIONS ] += 
        reward_pair.second->get_influence(player, loc, minDist, maxDist);
    }
  }

  return result;
}

void Grid::reset_seed(std::mt19937::result_type seed)
{
  rand_gen_.seed(seed);
}

} // end namespace cell

