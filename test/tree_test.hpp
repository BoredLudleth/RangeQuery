#pragma once
#include <gtest/gtest.h>

#include "range_query.hpp"
#include "range_query_tree.hpp"
#include "tree.hpp"
#include <climits>

namespace search_tree_space {

TEST(DistTestRangeQuery, TreeTest) {
  using search_tree_space::range_query;

  tree<int> tr{};
  tr.insert(1);
  tr.insert(2);
  tr.insert(3);
  tr.insert(4);
  tr.insert(10);
  tr.insert(11);
  tr.insert(12);
  tr.insert(13);
  tr.insert(14);
  tr.insert(18);

  EXPECT_EQ(range_query(tr, 5, 8), 0);
  EXPECT_EQ(range_query(tr, 5, 5), 0);
  EXPECT_EQ(range_query(tr, 1, 1), 1);
  EXPECT_EQ(range_query(tr, 3, 12), 5);
  EXPECT_EQ(range_query(tr, 1, 4), 4);
  EXPECT_EQ(range_query(tr, 3, 100), 8);
  EXPECT_EQ(range_query(tr, -10, 11), 6);
  EXPECT_EQ(range_query(tr, -10, 100), 10);
  EXPECT_EQ(range_query(tr, 1, 2), 2);
  EXPECT_EQ(range_query(tr, 11, 4), 0);
}

TEST(IterTest, TreeTest) {
  tree<int> tr{};

  for (int i = 1; i < 15; ++i) {
    tr.insert(i);
  }

  int i = 1;
  tree<int>::iterator it = tr.begin();
  for (it = tr.begin(); it != tr.end(); ++it) {
    EXPECT_EQ(it->get_key(), i);
    ++i;
  }

  EXPECT_EQ(it, tr.end());
}

TEST(RiterTest, TreeTest) {
  tree<int> tr{};

  for (int i = 1; i < 15; ++i) {
    tr.insert(i);
  }

  int i = 14;
  tree<int>::iterator it = tr.begin();
  for (it = --tr.end(); it != tr.begin(); --it) {
    EXPECT_EQ(it->get_key(), i);
    --i;
  }

  EXPECT_EQ(it, tr.begin());
}

TEST(BeginTest, TreeTest) {
  tree<int> tr{};
  tr.insert(3);
  tr.insert(1);
  tr.insert(2);

  EXPECT_EQ(tr.begin()->get_key(), 1);
}

TEST(DistTest, SetTest) {
  using common_space::range_query;
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
