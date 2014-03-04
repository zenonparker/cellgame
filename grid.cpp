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
#include <math>

namespace {

CONSTVAL_PF static double PI = 3.141592653589793238462;
CONSTVAL_PF static double PI2_BY_DIRS = (PI2 * 2.0) / double(InfluenceRing::NUM_DIRECTIONS * 2);

/*
InfluenceRing doRing(const vector<pair<Location, int>>& locs, const Location& ploc,
                     int minDist, int maxDist)
{
*/
}

namespace cell {

void Grid::add_reward(const Location& location, const Reward& reward)
{
  rewards_.insert(location, reward);
}

void Grid::remove_reward(const Location& location)
{
  rewards_.erase(location);
}

Scan Grid::scan_player(const Player& player) const
{
  Scan result;
  for (int i = 0; i < Scan::NUM_RINGS; ++i) {
    result.rings()[i] = scan_ring(player, 
                                  Scan::RING_RANGES[i*2],
                                  Scan::RING_RANGES[(i*2)+1]);
  }
  return result;
}

// Private

InfluenceRing Scan::scan_ring(const Player& player, int min, int max) const
{
  InfluenceRing result;

  const Location& ploc = player.location();

  // Obtain vector of possible rewards based on the rectangle around the player's
  // location that encompasses the largest scan ring.
  Location bottom_left{ploc.x - max, ploc.y - max};
  Location top_right{ploc.x + max, ploc.y + max};
  auto potential_rewards = rewards_.find_with_location(bottom_left, top_right);

  for (const auto& reward_pair : potential_rewards) {
    const Location& loc = reward_pair.first;

    // Distance check first.
    double xd = double(ploc.x - loc.x);
    double yd = double(ploc.y - loc.y);
    double dist = sqrt(xd*xd + yd*yd);

    if (dist >= double(minDist) && dist <= double(maxDist)) {
      // Determine which slice of the scan pie this falls into.
      double rads = atan2(double(loc.y), double(loc.x));
      cout << loc << " : " << rads << endl;
      int slice = (int(rads / PI2_BY_DIRS) + (InfluenceRing::NUM_DIRECTIONS * 2)) % 
                   (InfluenceRing::NUM_DIRECTIONS * 2);
      // slice is now in range [0, num_dirs*2], actual slice to go in will be
      // ((slice + 1) / 2) % num_dirs
      // For example, slice index 15 = (15 + 1)/2 = 8 % 8 = zeroth slice. (east)
      //              slice index 0 = (0 + 1)/2 = 0.5 % 8 = zeroth slice. (east)
      //              slice index 1 = (1 + 1)/2 = 1 % 8 = index 1 slice (north-east)
      ir1.vals()[((slice + 1) / 2) % InfluenceRing::NUM_DIRECTIONS] += p.second;
    }
  }

  return result;
}

} // end namespace cell

