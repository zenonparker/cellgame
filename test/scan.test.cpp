// gtest
#include <gtest/gtest.h>
#include <world.hpp>
#include <location.hpp>
#include <scan.hpp>
#include <influence_ring.hpp>
#include <cell.test.hpp>
#include <sstream>

using namespace std;
using namespace cell;
using namespace cell::test;

////////////////////////////////////////////////////////////////////////////////
///                         REUSABLE TEST FUNCTIONS                          ///
////////////////////////////////////////////////////////////////////////////////

void directional_from_player_location(int x, int y) {

  Player p;
  p.location() = Location{x, y};

  {
    // East Slice
    Grid grid;
    grid.add_reward(Location{x + 50, y + 0}, Reward{100});
    InfluenceRing ir1{vector<int>{100, 0, 0, 0, 0, 0, 0, 0}};
    EXPECT_EQ(ir1, grid.scan_single_ring(p, 1, 100));
  }

  {
    // North-East Slice
    Grid grid;
    grid.add_reward(Location{x + 50, y + 50}, Reward{100});
    InfluenceRing ir2{vector<int>{0, 100, 0, 0, 0, 0, 0, 0}};
    EXPECT_EQ(ir2, grid.scan_single_ring(p, 1, 100));
  }

  {
    // North Slice
    Grid grid;
    grid.add_reward(Location{x + 0, y + 50}, Reward{100});
    InfluenceRing ir3{vector<int>{0, 0, 100, 0, 0, 0, 0, 0}};
    EXPECT_EQ(ir3, grid.scan_single_ring(p, 1, 100));
  }

  {
    // North-West Slice
    Grid grid;
    grid.add_reward(Location{x + -50, y + 50}, Reward{100});
    InfluenceRing ir4{vector<int>{0, 0, 0, 100, 0, 0, 0, 0}};
    EXPECT_EQ(ir4, grid.scan_single_ring(p, 1, 100));
  }

  {
    // West Slice
    Grid grid;
    grid.add_reward(Location{x + -50, y + 0}, Reward{100});
    InfluenceRing ir5{vector<int>{0, 0, 0, 0, 100, 0, 0, 0}};
    EXPECT_EQ(ir5, grid.scan_single_ring(p, 1, 100));
  }

  {
    // South-West Slice
    Grid grid;
    grid.add_reward(Location{x + -50, y + -50}, Reward{100});
    InfluenceRing ir6{vector<int>{0, 0, 0, 0, 0, 100, 0, 0}};
    EXPECT_EQ(ir6, grid.scan_single_ring(p, 1, 100));
  }

  {
    // South Slice
    Grid grid;
    grid.add_reward(Location{x + 0, y + -50}, Reward{100});
    InfluenceRing ir7{vector<int>{0, 0, 0, 0, 0, 0, 100, 0}};
    EXPECT_EQ(ir7, grid.scan_single_ring(p, 1, 100));
  }

  {
    // South-East Slice
    Grid grid;
    grid.add_reward(Location{x + 50, y + -50}, Reward{100});
    InfluenceRing ir8{vector<int>{0, 0, 0, 0, 0, 0, 0, 100}};
    EXPECT_EQ(ir8, grid.scan_single_ring(p, 1, 100));
  }

}

void edge_scan_from_player_location(int x, int y) {
  Player p;
  p.location() = Location{x, y};

  {
    // East Slice Edges
    Grid grid;
    grid.add_reward(Location{x + 93, y + 38}, Reward{100});
    grid.add_reward(Location{x + 93, y + -38}, Reward{100});
    InfluenceRing ir1{vector<int>{200, 0, 0, 0, 0, 0, 0, 0}};
    EXPECT_EQ(ir1, grid.scan_single_ring(p, 1, 200));
  }

  {
    // North West Slice Edges
    Grid grid;
    grid.add_reward(Location{x + -39, y + 92}, Reward{110});
    grid.add_reward(Location{x + -37, y + 93}, Reward{120});
    InfluenceRing ir1{vector<int>{0, 0, 120, 110, 0, 0, 0, 0}};
    EXPECT_EQ(ir1, grid.scan_single_ring(p, 1, 200));
  }
}

void distance_test_from_player_location(int x, int y) {
  Player p;
  p.location() = Location{x, y};

  {
    Grid grid;
    grid.add_reward(Location{x + 100, y + 0}, Reward{100});
    grid.add_reward(Location{x + 0, y + 101}, Reward{100});
    InfluenceRing ir1{vector<int>{100, 0, 0, 0, 0, 0, 0, 0}};
    EXPECT_EQ(ir1, grid.scan_single_ring(p, 1, 100));
  }

  {
    Grid grid;
    grid.add_reward(Location{x + -50, y + -50}, Reward{110});
    grid.add_reward(Location{x + -51, y + -51}, Reward{120});
    InfluenceRing ir1{vector<int>{0, 0, 0, 0, 0, 110, 0, 0}};
    InfluenceRing ir2{vector<int>{0, 0, 0, 0, 0, 230, 0, 0}};
    InfluenceRing ir3{vector<int>{0, 0, 0, 0, 0, 120, 0, 0}};
    EXPECT_EQ(ir1, grid.scan_single_ring(p, 1, 71));
    EXPECT_EQ(ir2, grid.scan_single_ring(p, 1, 73));
    EXPECT_EQ(ir3, grid.scan_single_ring(p, 72, 73));
  }
}

////////////////////////////////////////////////////////////////////////////////
///                             ACTUAL TESTS                                 ///
////////////////////////////////////////////////////////////////////////////////

TEST(CellTests, basicScanTest) {
  directional_from_player_location(0, 0);
}

TEST(CellTests, edgeScanTest) {
  edge_scan_from_player_location(0, 0);
}

TEST(CellTests, distanceScanTest) {
  distance_test_from_player_location(0, 0);
}

TEST(CellTests, basicScanTestOffset) {
  directional_from_player_location(-500, -25000000);
  directional_from_player_location(10000000, 19293191);
  directional_from_player_location(std::numeric_limits<int>::max() - 1000, 
                                   std::numeric_limits<int>::max() - 1000);
  directional_from_player_location(-500000, 2019319123);
}

TEST(CellTests, edgeScanTestOffset) {
  edge_scan_from_player_location(-123141, -2311019);
  edge_scan_from_player_location(9999999, 18188822);
  edge_scan_from_player_location(std::numeric_limits<int>::max() - 1000, 
                                 std::numeric_limits<int>::max() - 1000);
  edge_scan_from_player_location(-1002301, 2123456789);
}

TEST(CellTests, distanceScanTestOffset) {
  distance_test_from_player_location(-123141, -2311019);
  distance_test_from_player_location(9999999, 18188822);
  distance_test_from_player_location(std::numeric_limits<int>::max() - 1000, 
                                     std::numeric_limits<int>::max() - 1000);
  distance_test_from_player_location(-1002301, 2123456789);
}

TEST(CellTests, influenceChangeTest) {
  // Influence should drop after scanning NUM_SCANS_BEFORE_RESET times.
  int x = 100, y = 100;
  Player p;
  p.location() = Location{x, y};

  {
    Grid grid;
    grid.add_reward(Location{x + 50, y + 0}, Reward{100, Reward::RewardType::DIST_TIME});
    InfluenceRing ir1{vector<int>{49, 0, 0, 0, 0, 0, 0, 0}};
    InfluenceRing resetIr{vector<int>{0, 0, 0, 0, 0, 0, 0, 0}};
    for (int i = 0; i < Reward::NUM_SCANS_BEFORE_RESET; ++i) {
      EXPECT_EQ(ir1, grid.scan_single_ring(p, 1, 100));
    }
    EXPECT_EQ(resetIr, grid.scan_single_ring(p, 1, 100));
  }
}

TEST(CellTests, distanceInfluenceTest) {
  int x = 100, y = 100;
  Player p;
  p.location() = Location{x, y};

  {
    Grid grid;
    grid.add_reward(Location{x + 50, y + 0}, Reward{100, Reward::RewardType::DISTANCE});
    grid.add_reward(Location{x + 60, y + 60}, Reward{1000, Reward::RewardType::DISTANCE});
    grid.add_reward(Location{x - 60, y - 60}, Reward{1000, Reward::RewardType::DISTANCE});
    InfluenceRing ir1{vector<int>{49, 142, 0, 0, 0, 142, 0, 0}};
    InfluenceRing ir2{vector<int>{94, 915, 0, 0, 0, 915, 0, 0}};
    EXPECT_EQ(ir1, grid.scan_single_ring(p, 1, 100));
    EXPECT_EQ(ir2, grid.scan_single_ring(p, 1, 1000));
  }
}

TEST(CellTests, randScanTest) {
  int x = 100, y = 100;
  Player p;
  p.location() = Location{x, y};

  {
    Grid grid;
    grid.reset_seed(42);
    // This random seed should:
    // Scan from 17 to 106
    // Scan from 123 to 984
    grid.add_reward(Location{x + 16, y}, Reward{1});
    grid.add_reward(Location{x + 18, y}, Reward{2});
    grid.add_reward(Location{x, y + 105}, Reward{3});
    grid.add_reward(Location{x, y + 107}, Reward{4});
    grid.add_reward(Location{x - 122, y}, Reward{5});
    grid.add_reward(Location{x - 124, y}, Reward{6});
    grid.add_reward(Location{x, y - 983}, Reward{7});
    grid.add_reward(Location{x, y - 985}, Reward{8});
    InfluenceRing ir1{vector<int>{2, 0, 3, 0, 0, 0, 0, 0}};
    InfluenceRing ir2{vector<int>{0, 0, 0, 0, 6, 0, 7, 0}};
    Scan s = grid.scan_player(p);
    EXPECT_EQ(ir1, s.rings()[0]);
    EXPECT_EQ(ir2, s.rings()[1]);
  }
}


