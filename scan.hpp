////////////////////////////////////////////////////////////////////////////////
///
/// @file scan.hpp
/// @brief Represents the results of a scan in the game world.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_SCAN_HPP
#define CELL_SCAN_HPP

#include <influence_ring.hpp>

namespace cell {

class Scan {
public:
  /// Number of rings of influence in a given Scan.
  CONSTVAL_PF int NUM_RINGS = 2;

  // Constructors
  Scan() { }
  explicit Scan(const std::vector<InfluenceRing>& rings) : rings_(rings) { }

  // Accessors
  const std::vector<InfluenceRing>& rings() const { return rings_; }
  std::vector<InfluenceRing>& rings() { return rings_; }

private:

  std::vector<InfluenceRing> rings_{NUM_RINGS, InfluenceRing{}};

};

} // end namespace cell

#endif // CELL_SCAN_HPP

