// test utility functions
#ifndef CELL_TESTUTILS_HPP
#define CELL_TESTUTILS_HPP
#include <vector>

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

}
}
#endif

