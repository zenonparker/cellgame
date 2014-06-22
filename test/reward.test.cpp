// gtest
#include <gtest/gtest.h>
#include <world.hpp>
#include <location.hpp>
#include <scan.hpp>
#include <reward.hpp>
#include <rewardmanager.hpp>
#include <influence_ring.hpp>
#include <cell.test.hpp>
#include <sstream>

using namespace std;
using namespace cell;
using namespace cell::test;

// Tests based on a falloff of 3.0
TEST(RewardTests, rewardFalloffTest) {
  // BASIC DIRECT HITS
  {
    Location rloc(0, 0), qloc(0, 0);
    Reward r1 = make_reward(1, 100, Reward::RewardType::TRIVIAL, rloc);
    Reward r2 = make_reward(2, 100, Reward::RewardType::DISTANCE, rloc);
    Reward r3 = make_reward(3, 100, Reward::RewardType::DIST_TIME, rloc);
    EXPECT_EQ(100, r1.value_from_location(qloc));
    EXPECT_EQ(100, r2.value_from_location(qloc));
    EXPECT_EQ(100, r3.value_from_location(qloc));
  }
  {
    Location rloc(123456, 98765), qloc(123456, 98765);
    Reward r1 = make_reward(1, 100, Reward::RewardType::TRIVIAL, rloc);
    Reward r2 = make_reward(2, 100, Reward::RewardType::DISTANCE, rloc);
    Reward r3 = make_reward(3, 100, Reward::RewardType::DIST_TIME, rloc);
    EXPECT_EQ(100, r1.value_from_location(qloc));
    EXPECT_EQ(100, r2.value_from_location(qloc));
    EXPECT_EQ(100, r3.value_from_location(qloc));
  }

  // HITS FROM HALF DEADZONE RADIUS
  {
    // 0.5 ^ 3 = 0.125 = 12.5 (double) = 12 (int)
    Location rloc(0, 0), qloc(Reward::HIT_RADIUS / 2, 0);
    Reward r1 = make_reward(1, 100, Reward::RewardType::TRIVIAL, rloc);
    Reward r2 = make_reward(2, 100, Reward::RewardType::DISTANCE, rloc);
    Reward r3 = make_reward(3, 100, Reward::RewardType::DIST_TIME, rloc);
    EXPECT_EQ(12, r1.value_from_location(qloc));
    EXPECT_EQ(12, r2.value_from_location(qloc));
    EXPECT_EQ(12, r3.value_from_location(qloc));
  }
  {
    Location rloc(123456, 98765), qloc(123456, 98765 - (Reward::HIT_RADIUS / 2));
    Reward r1 = make_reward(1, 100, Reward::RewardType::TRIVIAL, rloc);
    Reward r2 = make_reward(2, 100, Reward::RewardType::DISTANCE, rloc);
    Reward r3 = make_reward(3, 100, Reward::RewardType::DIST_TIME, rloc);
    EXPECT_EQ(12, r1.value_from_location(qloc));
    EXPECT_EQ(12, r2.value_from_location(qloc));
    EXPECT_EQ(12, r3.value_from_location(qloc));
  }

  // HITS OUTSIDE DEADZONE RADIUS
  {
    Location rloc(0, 0), qloc(Reward::HIT_RADIUS + 1, 0),
             qloc2(-Reward::HIT_RADIUS - 1, -Reward::HIT_RADIUS - 1);
    Reward r1 = make_reward(1, 100, Reward::RewardType::TRIVIAL, rloc);
    Reward r2 = make_reward(2, 100, Reward::RewardType::DISTANCE, rloc);
    Reward r3 = make_reward(3, 100, Reward::RewardType::DIST_TIME, rloc);
    EXPECT_EQ(0, r1.value_from_location(qloc));
    EXPECT_EQ(0, r2.value_from_location(qloc));
    EXPECT_EQ(0, r3.value_from_location(qloc));
    EXPECT_EQ(0, r1.value_from_location(qloc2));
    EXPECT_EQ(0, r2.value_from_location(qloc2));
    EXPECT_EQ(0, r3.value_from_location(qloc2));
  }

  // HITS ON EDGE
  {
    Location rloc(0, 0), qloc(Reward::HIT_RADIUS, 0), qloc2(Reward::HIT_RADIUS+1, 0);
    Reward r1 = make_reward(1, 100, Reward::RewardType::TRIVIAL, rloc);
    Reward r2 = make_reward(2, 1, Reward::RewardType::TRIVIAL, rloc);
    EXPECT_EQ(1, r1.value_from_location(qloc));
    // Minimum any hit can be is value of 1.
    EXPECT_EQ(1, r2.value_from_location(qloc));
    EXPECT_EQ(0, r2.value_from_location(qloc2));
  }
  
  // OTHER ARBITRARY VALUES
  {
    Location rloc(0, 0);
    Location qloc(3, 4);
    Location qloc2(5, 5);
    Location qloc3(1, 1);
    Reward r1 = make_reward(1, 100, Reward::RewardType::TRIVIAL, rloc);
    EXPECT_EQ(12, r1.value_from_location(qloc));
    EXPECT_EQ(2, r1.value_from_location(qloc2));
    EXPECT_EQ(63, r1.value_from_location(qloc3));
  }
}

TEST(RewardTests, rewardManagerRedistribution) {
  RewardManager rm;

}

