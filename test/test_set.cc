#include <gtest/gtest.h>

#include "../headers/my_set.h"

TEST(Set, Test_1) {
  my::set<int> s;
  EXPECT_EQ(true, s.insert(12).second);
  EXPECT_EQ(true, s.insert(14).second);
  EXPECT_EQ(true, s.insert(1).second);
  EXPECT_EQ(true, s.insert(5).second);

  EXPECT_EQ(true, s.find(14) != s.end());
  EXPECT_EQ(false, s.find(4) != s.end());

  s.erase(s.find(14));
  EXPECT_EQ(false, s.find(14) != s.end());

  EXPECT_EQ(s.contains(12), true);
}

TEST(Set, Test_2) {
  my::set<int> s;

  for (int i = 0; i < 100000; i++) s.insert(i);

  EXPECT_EQ(100000, s.size());

  for (int i = 0; i < 100000; i++) {
    auto it = s.find(i);
    if (it != s.end()) s.erase(it);
  }

  EXPECT_EQ(0, s.size());
}

TEST(Set, Test_3) {
  my::set<int> s;

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

TEST(Set, Test_4) {
  my::set<int> s;

  for (int i = 100; i > 0; i--) s.insert(i);

  for (int i = 101; i < 200; i++) s.insert(i);

  EXPECT_EQ(199, s.size());

  for (int i = 100; i > 0; i--) {
    auto it = s.find(i);
    if (it != s.end()) s.erase(it);
  }
  s.erase(my::set<int>::iterator(nullptr));

  EXPECT_EQ(99, s.size());
}

TEST(Set, Test_5) {
  my::set<int> s{3, 4, 1, -23, 34, 131};
  my::set<int> s2(s);

  EXPECT_EQ(s.size(), s2.size());
  auto it = s.begin();
  auto it2 = s.begin();
  for (; it != s.end(); it++, it2++) {
    EXPECT_EQ(*it, *it2);
  }
}

TEST(Set, Test_6) {
  my::set<char> s{'2', 'd', '$', 'H'};
  my::set<char> s2(std::move(s));
  EXPECT_EQ(s.empty(), true);
  EXPECT_EQ(s2.size(), 4);

  my::set<char> s3 = std::move(s2);
  EXPECT_EQ(s2.empty(), true);
  EXPECT_EQ(s3.size(), 4);

  EXPECT_EQ(s3.max_size() > 0, true);
}

TEST(Set, Test_7) {
  my::set<int> s{3,   5,    1,  4,  6,  1,  2,  12,  43,
                  -13, -134, 24, 45, 67, 24, 43, 234, 56};
  EXPECT_EQ(s.size(), 15);
  s.erase(s.find(3));
  s.erase(s.find(5));
  s.erase(s.find(3));
  s.erase(s.find(43));
  s.erase(s.find(234));
  s.erase(s.find(56));
  s.erase(s.find(-13));
  s.erase(s.find(67));
  s.erase(s.find(1));
  s.erase(s.find(4));
  s.erase(s.find(6));
  s.erase(s.find(2));
}

TEST(Set, Test_8) {
  my::set<int> s{1, 1, 1, 1, 1};
  EXPECT_EQ(s.size(), 1);

  my::set<int> s1{1, 2, 3, 4, 5};
  my::set<int> s2{3, 4, 5, 6, 7, 8};
  s1.merge(s2);
  auto it = s1.begin();
  for (int i = 1; i < 9; i++, ++it) {
    EXPECT_EQ(i, *it);
  }
}

TEST(Set, Test_9) {
  my::set<int> s1{1, 2, 3, 4, 5};
  my::set<int> s2{3, 4, 5, 6, 7, 8};
  s1.swap(s2);

  EXPECT_EQ(s1.size(), 6);
  EXPECT_EQ(s2.size(), 5);
  EXPECT_EQ(*s1.begin(), 3);
  EXPECT_EQ(*s2.begin(), 1);

  EXPECT_EQ(s1.contains(8), true);
  EXPECT_EQ(s2.contains(2), true);
  EXPECT_EQ(s1.end() == my::set<int>::iterator(nullptr), true);
}

TEST(Set, Test_10) {
  my::set<int> s{1, 2, 3, 4};

  auto it = s.insert_many(3, 4, 5, 6, 7, 10);
  EXPECT_EQ(s.size(), 8);
  EXPECT_EQ(it[0].second, false);
  EXPECT_EQ(it[1].second, false);
  EXPECT_EQ(*(it[2].first), 5);
}