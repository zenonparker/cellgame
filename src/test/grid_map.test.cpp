// gtest
#include <gtest/gtest.h>
#include <grid_map.hpp>
#include <cell.test.hpp>
#include <vector>

using namespace std;
using namespace cell;
using namespace cell::test;

TEST(GridMap, findLocations) {
  //small
  Location s1(0, 0), s2(1, 0), s3(1, 1), s4(0, 1);
  //medium
  Location           m2(2, 0), m3(2, 2), m4(0, 2);
  //large
  Location           l2(3, 0), l3(3, 3), l4(0, 3);
  //xtra
  Location           x2(4, 0), x3(4, 4), x4(0, 4);

  GridMap<int> gridmap;
  gridmap.insert(s1, 900);
  gridmap.insert(s2, 910);
  gridmap.insert(s3, 911);
  gridmap.insert(s4, 901);
  gridmap.insert(m2, 920);
  gridmap.insert(m3, 922);
  gridmap.insert(m4, 902);
  gridmap.insert(l2, 930);
  gridmap.insert(l3, 933);
  gridmap.insert(l4, 903);
  gridmap.insert(x2, 940);
  gridmap.insert(x3, 944);
  gridmap.insert(x4, 904);

  typedef std::vector<std::pair<Location,int>> RetType;
  RetType ret = gridmap.find(s1, s1);
  RetType expected_result = { { s1, 900 } };
  EXPECT_TRUE(areEqual(expected_result, ret));

  ret = gridmap.find(s1, s2);
  expected_result = { { s1, 900 }, { s2, 910 } };
  EXPECT_TRUE(areEqual(expected_result, ret));

  ret = gridmap.find(s1, s4);
  expected_result = { { s1, 900 }, { s4, 901 } };
  EXPECT_TRUE(areEqual(expected_result, ret));

  ret = gridmap.find(s1, s3);
  expected_result = { { s1, 900 }, { s4, 901 }, { s2, 910 }, { s3, 911 } };
  EXPECT_TRUE(areEqual(expected_result, ret));

  ret = gridmap.find(s3, s1);
  expected_result = { };
  EXPECT_TRUE(areEqual(expected_result, ret));

  Location m5(2, 3), m6(3, 2);
  gridmap.insert(m5, 923);
  gridmap.insert(m6, 932);

  ret = gridmap.find(m3, l3);
  expected_result = { { m3, 922 }, { m5, 923 }, { m6, 932 }, { l3, 933 } };
  EXPECT_TRUE(areEqual(expected_result, ret));
  
  std::string test = "test";
  GridMap<std::string> grid_map_str;
  grid_map_str.insert({0,0}, test);
  auto ret_str = grid_map_str.find_ptr({ 0, 0 }, { 0, 0 });
  EXPECT_EQ(1,ret_str.size());
  EXPECT_EQ("test", *(ret_str[0].second));

  //test if both pointers are the same
  auto ret_str2 = grid_map_str.find_ptr({ 0, 0 }, { 0, 0 });
  EXPECT_EQ(1, ret_str2.size());
  EXPECT_EQ("test", *(ret_str2[0].second));

  EXPECT_EQ(ret_str[0].second, ret_str2[0].second);
  
}

TEST(GridMap, removeReward) {
  // Valid locations
  Location s1(0, 0), s2(1, 0), s3(1, 1), s4(0, 1);
  // Invalid locations
  Location i1(10,10), i2(11,11);

  GridMap<int> gridmap;
  gridmap.insert(s1, 900);
  gridmap.insert(s2, 910);
  gridmap.insert(s3, 911);
  gridmap.insert(s4, 901);

  typedef std::vector<std::pair<Location,int>> RetType;
  {
    RetType ret = gridmap.find(s1, s1);
    RetType expected_result = { { s1, 900 } };
    EXPECT_TRUE(areEqual(expected_result, ret));
  }

  // Remove nothing.
  int i = -1;
  EXPECT_FALSE(gridmap.remove(i1, i));
  EXPECT_EQ(i, -1);

  // Make sure everything is still there.
  {
    RetType ret = gridmap.find(s1, s1);
    RetType expected_result = { { s1, 900 } };
    EXPECT_TRUE(areEqual(expected_result, ret));

    ret = gridmap.find(s1, s2);
    expected_result = { { s1, 900 }, { s2, 910 } };
    EXPECT_TRUE(areEqual(expected_result, ret));

    ret = gridmap.find(s1, s4);
    expected_result = { { s1, 900 }, { s4, 901 } };
    EXPECT_TRUE(areEqual(expected_result, ret));

    ret = gridmap.find(s1, s3);
    expected_result = { { s1, 900 }, { s4, 901 }, { s2, 910 }, { s3, 911 } };
    EXPECT_TRUE(areEqual(expected_result, ret));

    ret = gridmap.find(s3, s1);
    expected_result = { };
    EXPECT_TRUE(areEqual(expected_result, ret));
  }

  EXPECT_TRUE(gridmap.remove(s1, i));
  EXPECT_EQ(i, 900);

  i = -1;
  EXPECT_TRUE(gridmap.remove(s2, i));
  EXPECT_EQ(i, 910);
}

