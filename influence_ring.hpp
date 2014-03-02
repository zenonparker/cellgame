////////////////////////////////////////////////////////////////////////////////
///
/// @file influence_ring.hpp
/// @brief Used to represent a given "ring" of influence levels inside a Scan.
///        A Scan can have multiple of these representing influence at
///        different distances from the scan's origin.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_INFLUENCE_RING_HPP
#define CELL_INFLUENCE_RING_HPP

#include <limits>
#include <cpp11_polyfill.hpp>

namespace cell {

class InfluenceRing {
public:
  /// Number of directions in this influence ring: North, North East, East, etc...
  static CONSTVAL_PF int NUM_DIRECTIONS = 8;

  /// The max value for influence in any given direction.
  /// Influence will be in the range [0, MAX_INFLUENCE]
  CONSTVAL_PF int MAX_INFLUENCE = std::numeric_limits<int>::max();

  // Constructors
  InfluenceRing() { }
  explicit InfluenceRing(const std::vector<int>& vals) : vals_(vals) { }

  // Accessors
  const std::vector<int>& vals() const { return vals_; }
  std::vector<int>& vals() { return vals_; }

private:

  std::vector<int> vals_{NUM_DIRECTIONS, 0};

};

} // end namespace cell

#endif // CELL_INFLUENCE_RING_HPP

