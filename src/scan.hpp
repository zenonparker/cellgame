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
#include <iostream>

namespace cell {

class Scan {
public:
  /// Number of rings of influence in a given Scan.
  constexpr static int NUM_RINGS = 2;
  /// Array of range values for distances that the rings encompass. Should always
  /// be equal to 2*NUM_RINGS in size. { ring1_min, ring1_max, ring2_min, etc. }
  const static int RING_RANGES[NUM_RINGS * 2]; /*= { { 20, 100, 100, 1000 } };*/
  const static int RING_RANGE_VARIANCE[NUM_RINGS]; /*= { { 10, 25 } };*/

  // Constructors
  Scan() { }
  explicit Scan(const std::vector<InfluenceRing>& rings) : rings_(rings) { }

  // Accessors
  const std::vector<InfluenceRing>& rings() const { return rings_; }
  std::vector<InfluenceRing>& rings() { return rings_; }

  // Operators
  friend bool operator==(const Scan& s1, const Scan& s2);
  friend std::ostream& operator<<(std::ostream& out, const Scan& s);

private:

  std::vector<InfluenceRing> rings_{NUM_RINGS, InfluenceRing{}};

};

inline bool operator==(const Scan& s1, const Scan& s2) {
  for (int i = 0; i < s1.rings_.size(); ++i) {
    if (!(s1.rings_[i] == s2.rings_[i])) return false;
  }
  return true;
}

inline std::ostream& operator<<(std::ostream& out, const Scan& s) {
  out << "{ ";
  for (int i = 0; i < s.rings_.size(); ++i) {
    if (i > 0) out << "\n  ";
    out << "[" << i << "] " << s.rings_[i];
  }
  return out << " }";
}

} // end namespace cell

#endif // CELL_SCAN_HPP

