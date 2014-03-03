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

  // Operators
  friend bool operator==(const Scan& s1, const Scan& s2);
  friend std::ostream& operator<<(std::ostream& out, const Scan& s);

private:

  std::vector<InfluenceRing> rings_{NUM_RINGS, InfluenceRing{}};

};

bool operator==(const Scan& s1, const Scan& s2) {
  for (int i = 0; i < s1.rings_.size(); ++i) {
    if (!(s1.rings_[i] == s2.rings_[i])) return false;
  }
  return true;
}

std::ostream& operator<<(std::ostream& out, const Scan& s) {
  out << "{ ";
  for (int i = 0; i < s.rings_.size(); ++i) {
    if (i > 0) out << "\n  ";
    out << "[" << i << "] " << s.rings_[i];
  }
  return out << " }";
}

} // end namespace cell

#endif // CELL_SCAN_HPP

