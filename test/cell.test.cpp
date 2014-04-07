// gtest
#include <gtest/gtest.h>
#include <world.hpp>
#include <location.hpp>
#include <scan.hpp>
#include <influence_ring.hpp>
#include <cell.test.hpp>

using namespace std;
using namespace cell;
using namespace cell::test;

TEST(CellTests, locationTest) {
  Location l1(2, 3), l2(2, 3), l3(3, 3), l4(2, 2);
  EXPECT_EQ(l1, l2);
  EXPECT_FALSE(l1 < l2);
  EXPECT_LT(l1, l3);
  EXPECT_LT(l4, l1);
  EXPECT_FALSE(l3 < l1);
  EXPECT_FALSE(l1 < l4);

  stringstream ss;
  ss << l1;
}

TEST(CellTests, scanRingTest) {
  stringstream ss;

  vector<int> v1(InfluenceRing::NUM_DIRECTIONS, 0), v2{1,2,3,4,5,6,7,8};
  InfluenceRing i1;
  InfluenceRing i2{v2};
  EXPECT_TRUE(areEqual(i1.vals(), v1));
  EXPECT_TRUE(areEqual(i2.vals(), v2));

  ss << i1 << endl;
  ss << i2 << endl;

  vector<InfluenceRing> vir{i1, i2};
  Scan s1{vir};
  EXPECT_TRUE(areEqual(s1.rings()[0].vals(), v1));
  EXPECT_TRUE(areEqual(s1.rings()[1].vals(), v2));

  ss << s1 << endl;
}

TEST(CellTests, joinLeaveTest) {
  World w;

  EXPECT_TRUE(w.player_join(Player(1)));
  EXPECT_TRUE(w.player_join(Player(2)));
  EXPECT_TRUE(w.player_join(Player(3)));
  EXPECT_TRUE(w.player_join(Player(4)));

  // Already added.
  EXPECT_FALSE(w.player_join(Player(1)));
  EXPECT_FALSE(w.player_join(Player(2)));

  EXPECT_TRUE(w.player_leave(1));

  // Already left.
  EXPECT_FALSE(w.player_leave(1));

  EXPECT_TRUE(w.player_leave(2));
  EXPECT_TRUE(w.player_leave(3));
  EXPECT_TRUE(w.player_leave(4));

  // Already left.
  EXPECT_FALSE(w.player_leave(4));

  // Everyone should be able to join again.
  EXPECT_TRUE(w.player_join(Player(1)));
  EXPECT_TRUE(w.player_join(Player(2)));
  EXPECT_TRUE(w.player_join(Player(3)));
  EXPECT_TRUE(w.player_join(Player(4)));
}

