#pragma once
#include <gtest/gtest.h>

#include "tree.hpp"

namespace search_tree_space {
TEST(DistTest, TreeTest) {
  tree<int> tr(1);
  tr.insert(2);
  tr.insert(3);
  tr.insert(4);
  tr.insert(10);
  tr.insert(11);
  tr.insert(12);
  tr.insert(13);
  tr.insert(14);
  tr.insert(18);

  EXPECT_EQ(tr.distance(5, 8), 0);
  EXPECT_EQ(tr.distance(5, 5), 0);
  EXPECT_EQ(tr.distance(1, 1), 1);
  EXPECT_EQ(tr.distance(3, 12), 5);
  EXPECT_EQ(tr.distance(1, 4), 4);
  EXPECT_EQ(tr.distance(3, 100), 8);
  EXPECT_EQ(tr.distance(-10, 11), 6);
  EXPECT_EQ(tr.distance(-10, 100), 10);
  EXPECT_EQ(tr.distance(1, 2), 2);
  EXPECT_EQ(tr.distance(11, 4), 0);
}
}  // namespace search_tree_space