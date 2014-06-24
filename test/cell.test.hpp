// test utility functions
#ifndef CELL_TESTUTILS_HPP
#define CELL_TESTUTILS_HPP
#include <reward.hpp>
#include <vector>

using namespace cell;

namespace cell {
namespace test {

template <typename T>
bool areEqual(const std::vector<T>& v1, const std::vector<T>& v2) {
  if (v1.size() != v2.size()) return false;
  for (int i = 0; i < v1.size(); ++i) {
    if (v1[i] != v2[i]) return false;
  }
  return true;
}

template <typename T>
bool quantitiesEqual(const std::vector<Reward>& v1, const std::vector<T>& v2) {
  if (v1.size() != v2.size()) return false;
  for (int i = 0; i < v1.size(); ++i) {
    if (v1[i].quantity() != v2[i]) return false;
  }
  return true;
}

static Reward make_reward(RewardId id, int quantity) {
  Reward r(id, Reward::RewardType::TRIVIAL);
  r.quantity() = quantity;
  return r;
}

static Reward make_reward(RewardId id, int quantity, Reward::RewardType type) {
  Reward r(id, type);
  r.quantity() = quantity;
  return r;
}

static Reward make_reward(RewardId id, int quantity, Location l) {
  Reward r(id, Reward::RewardType::TRIVIAL);
  r.quantity() = quantity;
  r.location() = l;
  return r;
}

static Reward make_reward(RewardId id, int quantity, Reward::RewardType type, Location l) {
  Reward r(id, type);
  r.quantity() = quantity;
  r.location() = l;
  return r;
}

}
}
#endif

