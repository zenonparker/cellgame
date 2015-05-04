// gtest
#include <gtest/gtest.h>
#include <world.hpp>
#include <location.hpp>
#include <scan.hpp>
#include <reward.hpp>
#include <rewardmanager.hpp>
#include <rewardclass.hpp>
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

TEST(RewardTests, rewardClassDistribution) {

  // Base case of zero quantity.
  {
    vector<Reward> rewards;
    RewardClass rc(move(rewards));
    EXPECT_EQ(rc.total_quantity(), 0);
  }

  // Simple total quantity calculation.
  {
    vector<Reward> rewards;
    rewards.push_back(make_reward(1, 10));
    rewards.push_back(make_reward(2, 10));
    rewards.push_back(make_reward(3, 10));
    RewardClass rc(move(rewards));
    EXPECT_EQ(rc.total_quantity(), 30);
    EXPECT_TRUE(quantitiesEqual(rc.rewards(), vector<int>{10, 10, 10}));
  }

  // Very basic distribution.
  {
    vector<Reward> rewards;
    rewards.push_back(make_reward(1, 8));
    rewards.push_back(make_reward(2, 10));
    rewards.push_back(make_reward(3, 12));
    RewardClass rc(move(rewards));
    EXPECT_EQ(rc.total_quantity(), 30);

    rc.distribute_quantity(1);
    EXPECT_EQ(rc.total_quantity(), 31);
    EXPECT_TRUE(quantitiesEqual(rc.rewards(), vector<int>{9, 10, 12}));

    rc.distribute_quantity(3);
    EXPECT_EQ(rc.total_quantity(), 34);
    EXPECT_TRUE(quantitiesEqual(rc.rewards(), vector<int>{11, 11, 12}));

    rc.distribute_quantity(10);
    EXPECT_EQ(rc.total_quantity(), 44);
    EXPECT_TRUE(quantitiesEqual(rc.rewards(), vector<int>{15, 15, 14}));
  }

  // More complicated distribution, multiple iterations.
  {
    vector<Reward> rewards;
    rewards.push_back(make_reward(1, 1));
    rewards.push_back(make_reward(2, 10));
    rewards.push_back(make_reward(3, 100));
    RewardClass rc(move(rewards));
    EXPECT_EQ(rc.total_quantity(), 111);

    rc.distribute_quantity(10);
    EXPECT_EQ(rc.total_quantity(), 121);
    EXPECT_TRUE(quantitiesEqual(rc.rewards(), vector<int>{11, 10, 100}));

    rc.distribute_quantity(100);
    EXPECT_EQ(rc.total_quantity(), 221);
    EXPECT_TRUE(quantitiesEqual(rc.rewards(), vector<int>{60, 61, 100}));
  }

  // Test minimum (10%) of incoming quantity when rewards aren't far from average
  // and the adding of entire quantity if within 120%.
  {
    vector<Reward> rewards;
    rewards.push_back(make_reward(1, 100));
    rewards.push_back(make_reward(2, 100));
    rewards.push_back(make_reward(3, 100));
    rewards.push_back(make_reward(4, 100));
    rewards.push_back(make_reward(5, 100));
    rewards.push_back(make_reward(6, 100));
    rewards.push_back(make_reward(7, 100));
    rewards.push_back(make_reward(8, 100));
    rewards.push_back(make_reward(9, 100));
    rewards.push_back(make_reward(10, 100));
    rewards.push_back(make_reward(11, 100));
    RewardClass rc(move(rewards));
    EXPECT_EQ(rc.total_quantity(), 1100);

    rc.distribute_quantity(90);
    EXPECT_EQ(rc.total_quantity(), 1190);
    EXPECT_TRUE(quantitiesEqual(rc.rewards(), 
                                vector<int>{109, 108, 107, 106, 106, 108, 106, 106, 106, 125, 103}));
  }
}

TEST(RewardTests, rewardManagerRedistribution) {
  RewardManager rm;

}

