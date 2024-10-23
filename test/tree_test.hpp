#pragma once
#include <gtest/gtest.h>

#include "set_range_query.hpp"
#include "tree.hpp"

namespace search_tree_space {
TEST(InsertTest, TreeTest) {
  tree<int> tr{1};

  EXPECT_EQ(tr.get_top()->get_key(), 1);

  tr.insert(2);

  EXPECT_EQ(tr.get_top()->get_key(), 1);
  EXPECT_EQ(tr.get_top()->right->get_key(), 2);

  tr.insert(3);

  EXPECT_EQ(tr.get_top()->get_key(), 2);
  EXPECT_EQ(tr.get_top()->left->get_key(), 1);
  EXPECT_EQ(tr.get_top()->right->get_key(), 3);

  tr.insert(4);

  EXPECT_EQ(tr.get_top()->get_key(), 3);
  EXPECT_EQ(tr.get_top()->left->get_key(), 2);
  EXPECT_EQ(tr.get_top()->right->get_key(), 4);
  EXPECT_EQ(tr.get_top()->left->left->get_key(), 1);
}

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

TEST(DistTest, SetTest) {
  using set_space::range_query;
  std::set<int> s;
  s.insert(1);
  s.insert(2);
  s.insert(3);
  s.insert(4);
  s.insert(10);
  s.insert(11);
  s.insert(12);
  s.insert(13);
  s.insert(14);
  s.insert(18);

  EXPECT_EQ(range_query(s, 5, 8), 0);
  EXPECT_EQ(range_query(s, 5, 5), 0);
  EXPECT_EQ(range_query(s, 1, 1), 1);
  EXPECT_EQ(range_query(s, 3, 12), 5);
  EXPECT_EQ(range_query(s, 1, 4), 4);
  EXPECT_EQ(range_query(s, 3, 100), 8);
  EXPECT_EQ(range_query(s, -10, 11), 6);
  EXPECT_EQ(range_query(s, -10, 100), 10);
  EXPECT_EQ(range_query(s, 1, 2), 2);
  EXPECT_EQ(range_query(s, 11, 4), 0);
}
}  // namespace search_tree_space