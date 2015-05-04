////////////////////////////////////////////////////////////////////////////////
///
/// @file scan.cpp
/// @brief Implementation of the Scan class.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#include <scan.hpp>

namespace cell {

  const int Scan::RING_RANGES[Scan::NUM_RINGS * 2] = { 20, 100, 100, 1000 };
  const int Scan::RING_RANGE_VARIANCE[Scan::NUM_RINGS] = { 10, 25 };

} // end namespace cell

