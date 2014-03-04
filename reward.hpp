////////////////////////////////////////////////////////////////////////////////
///
/// @file reward.hpp
/// @brief Representation of a reward/prize in the Cell game.
/// @author Zenon Parker
/// @author Matthew Avery
/// @date 2014
///
////////////////////////////////////////////////////////////////////////////////

#ifndef CELL_REWARD_HPP
#define CELL_REWARD_HPP

namespace cell {

class Reward {
public:

  // Constructors
  Reward(int quantity) : quantity_(quantity) { }

  // Accessors

  /// Very basic calculation of influence for now...
  int get_influence() { return quantity_; }

private:

  int quantity_;

};

} // end namespace cell

#endif // CELL_REWARD_HPP

