#include <gtest/gtest.h>

#include "../headers/my_multiset.h"

TEST(Multiset, Test_1) {
  my::multiset<int> ms;

  ms.insert(1);
  ms.insert(2);
  ms.insert(3);
  ms.insert(4);
  ms.insert(4);
  EXPECT_EQ(5, ms.size());

  ms.erase(ms.find(1));

  EXPECT_EQ(4, ms.size());
}

TEST(Multiset, Test_2) {
  my::multiset<int> ms{1, 2, 3, 3, 4, 4, 4, 5, 5};

  EXPECT_EQ(ms.size(), 9);

  ms.erase(ms.find(4));
  ms.erase(ms.find(4));
  EXPECT_EQ(ms.find(4) != ms.end(), true);
}

TEST(Multiset, Test_3) {
  my::multiset<int> ms{1, 2, 3, 3, 4, 4, 4, 5, 5};

  my::multiset<int> ms1(ms);

  EXPECT_EQ(ms.size(), ms1.size());

  ms.erase(ms.find(4));
  ms.erase(ms.find(4));
  EXPECT_EQ(ms.find(4) != ms.end(), true);
}

TEST(Multiset, Test_4) {
  my::multiset<int> ms{1, 2, 3, 3, 4, 4, 4, 5, 5};
  EXPECT_TRUE(ms.contains(1));
  EXPECT_TRUE(ms.contains(2));
  EXPECT_TRUE(ms.contains(3));
  EXPECT_TRUE(ms.contains(4));
  EXPECT_TRUE(ms.contains(5));
  EXPECT_FALSE(ms.contains(6));

  ms.erase(ms.find(3));
  ms.erase(ms.find(3));

  EXPECT_FALSE(ms.contains(3));
  EXPECT_EQ(ms.count(3), 0);
  EXPECT_EQ(ms.count(4), 3);
}

TEST(Multiset, Test_5) {
  my::multiset<int> ms{1, 2, 3, 3, 4, 4, 4, 5, 5};

  auto it1 = ms.find(4);

  auto it2 = ms.lower_bound(4);

  EXPECT_EQ(it1 == it2, true);
}

TEST(Multiset, Test_6) {
  my::multiset<int> ms{1, 2, 3, 3, 4, 4, 4, 5, 5};

  auto it1 = ms.find(1);

  auto it2 = ms.upper_bound(2);

  EXPECT_EQ(it1 == it2, true);
}

TEST(Multiset, Test_7) {
  my::multiset<int> s;

  for (int i = 0; i < 100000; i++) s.insert(i);

  EXPECT_EQ(100000, s.size());

  for (int i = 0; i < 100000; i++) {
    auto it = s.find(i);
    if (it != s.end()) s.erase(it);
  }

  EXPECT_EQ(0, s.size());
}

TEST(Multiset, Test_8) {
  my::multiset<int> s;
  EXPECT_EQ(s.max_size() > 1, true);
  for (int i = 100000; i > 0; i--) s.insert(i);

  EXPECT_EQ(100000, s.size());

  for (int i = 100000; i > 0; i--) {
    auto it = s.find(i);
    if (it != s.end()) {
      EXPECT_EQ(false, s.empty());
      s.erase(it);
    }
  }

  EXPECT_EQ(true, s.empty());
  EXPECT_EQ(0, s.size());
}

TEST(Multiset, Test_9) {
  my::multiset<int> s{1, 2, 3, 4};

  auto it = s.insert_many(3, 4, 5, 6, 7, 10);
  EXPECT_EQ(s.size(), 10);
  EXPECT_EQ(it[0].second, true);
  EXPECT_EQ(it[1].second, true);
}

TEST(Multiset, Test_10) {
  my::multiset<int> s{1, 2, 2, 2, 3};

  auto pair_it = s.equal_range(2);

  EXPECT_EQ((*(pair_it.first)).first, 2);
  EXPECT_EQ((*(pair_it.second)).first, 2);
}

TEST(Multiset, Test_11) {
  my::multiset<int> ms1{1, 2};
  my::multiset<int> ms2{3, 4, 5, 6};
  ms1.swap(ms2);

  EXPECT_EQ(ms1.size(), 4);
  EXPECT_EQ(ms2.size(), 2);
}

TEST(Multiset, Test_12) {
  my::multiset<int> ms1{1, 2, 3, 4};
  my::multiset<int> ms2{3, 4, 5, 6};
  ms1.merge(ms2);

  EXPECT_EQ(ms1.size(), 6);
}
