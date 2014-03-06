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
#include <math.h>

namespace {

CONSTVAL_PF static double PI = 3.141592653589793238462;
CONSTVAL_PF static double PI2_BY_DIRS = (PI * 2.0) / double(cell::InfluenceRing::NUM_DIRECTIONS * 2);

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
    result.rings()[i] = scan_single_ring(player, 
                                         Scan::RING_RANGES[i*2],
                                         Scan::RING_RANGES[(i*2)+1]);
  }
  return result;
}


InfluenceRing Grid::scan_single_ring(const Player& player, int minDist, int maxDist) const
{
  InfluenceRing result;

  const Location& ploc = player.location();

  // Obtain vector of possible rewards based on the rectangle around the player's
  // location that encompasses the largest scan ring.
  Location bottom_left{ploc.x - maxDist, ploc.y - maxDist};
  Location top_right{ploc.x + maxDist, ploc.y + maxDist};
  // TODO: Update this to use find_ptr or something. We need modifiable objects
  //       from the grid in case we need to update reward values after a scan.
  auto potential_rewards = rewards_.find(bottom_left, top_right);

  for (auto& reward_pair : potential_rewards) {
    const Location& loc = reward_pair.first;

    // Distance check first.
    double xd = double(loc.x - ploc.x);
    double yd = double(loc.y - ploc.y);
    double dist = sqrt(xd*xd + yd*yd);

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
        reward_pair.second.get_influence(player, loc);
    }
  }

  return result;
}

} // end namespace cell

