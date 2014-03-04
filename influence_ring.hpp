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
#include <climits>
#include <vector>
#include <iostream>

namespace cell {

/** @brief Sections or slices in the InfluenceRing are indexed in the range
  *        [0, NUM_DIRECTIONS] counter clockwise from the vector (1,0):
  *
  *                   Y ^
  *                     |
  *                     |
  *                     |
  *                     +------> (1,0) X
  *        
  *        The zeroth slice is the one whose CENTER is the vector (1,0). For
  *        example in an eight direction ring this would be the slice facing
  *        directly east.
  */
class InfluenceRing {
public:
  /// Number of directions in this influence ring: North, North East, East, etc...
  static constexpr int NUM_DIRECTIONS = 8;

  /// The max value for influence in any given direction.
  /// Influence will be in the range [0, MAX_INFLUENCE]
  static constexpr int MAX_INFLUENCE = INT_MAX;
  // TODO: WHEN VisualStudio doesn't suck ass: std::numeric_limits<int>::max();

  // Constructors
  InfluenceRing() { }
  explicit InfluenceRing(const std::vector<int>& vals) : vals_(vals) { }

  // Accessors
  const std::vector<int>& vals() const { return vals_; }
  std::vector<int>& vals() { return vals_; }

  // Operators
  friend bool operator==(const InfluenceRing& ir1, const InfluenceRing& ir2);
  friend std::ostream& operator<<(std::ostream& out, const InfluenceRing& ir);

private:

  std::vector<int> vals_ = std::vector<int>(NUM_DIRECTIONS, 0);

};

inline bool operator==(const InfluenceRing& ir1, const InfluenceRing& ir2) {
  for (int i = 0; i < ir1.vals_.size(); ++i) {
    if (ir1.vals_[i] != ir2.vals_[i]) return false;
  }
  return true;
}

inline std::ostream& operator<<(std::ostream& out, const InfluenceRing& ir) {
  out << "(";
  auto it = ir.vals_.begin();
  while (it != ir.vals_.end()) {
    out << *it++;
    while (it != ir.vals_.end()) {
      out << ", " << *it++;
    }
  }
  return out << ")";
}

} // end namespace cell

#endif // CELL_INFLUENCE_RING_HPP

