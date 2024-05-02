#include <gtest/gtest.h>

#include "../headers/my_map.h"

TEST(map, Test_1) {
  my::map<int, char> mp;
  EXPECT_EQ(mp.empty(), true);

  mp.insert({1, 'f'});
  mp.insert(2, 'g');
  mp.insert(2, 'a');

  EXPECT_EQ(mp.size(), 2);

  EXPECT_EQ(mp[1], 'f');
  EXPECT_EQ(mp[2], 'g');

  mp.insert_or_assign(3, '4');
  mp.insert_or_assign(2, 'R');

  EXPECT_EQ(mp[3], '4');
  EXPECT_EQ(mp[2], 'R');
  EXPECT_EQ(mp.size(), 3);

  for (int i = 1; i < 4; i++) {
    mp.erase(mp.find(i));
  }

  EXPECT_EQ(mp.empty(), true);
}

TEST(map, Test_2) {
  my::map<char, int> mp{{'a', 1}, {'b', 2}, {'c', 3}};

  EXPECT_EQ(mp.size(), 3);

  EXPECT_EQ(mp.at('a'), 1);
  EXPECT_EQ(mp['b'], 2);

  mp.erase(mp.find('c'));
  mp.erase(mp.find('b'));
  EXPECT_EQ(mp.size(), 1);
}

TEST(map, Test_3) {
  my::map<int, char> mp = {{1, '1'}, {2, '2'}, {3, '3'}, {4, '4'}};
  EXPECT_EQ(mp.size(), 4);
  auto it = mp.begin();
  int i = 1;
  for (; it != mp.end(); ++it) {
    EXPECT_EQ((*it).first, i);
    EXPECT_EQ((*it).second, i + '0');
    i++;
  }
}

TEST(map, Test_4) {
  my::map<int, int> mp{{1, 10}, {2, 20}, {3, 30}};
  my::map<int, int> mp1(mp);

  EXPECT_EQ(mp1.size(), mp.size());

  for (int i = 1; i < 4; i++) EXPECT_EQ(mp.at(i), mp1.at(i));
  EXPECT_THROW(mp.at(23), std::runtime_error);
}

TEST(map, Test_5) {
  my::map<int, int> mp{{1, 10}, {2, 20}, {3, 30}};
  my::map<int, int> mp1(std::move(mp));

  EXPECT_EQ(mp1.size(), 3);
  EXPECT_EQ(mp.size(), 0);

  for (int i = 1; i < 4; i++) EXPECT_EQ(i * 10, mp1.at(i));
}

TEST(map, Test_6) {
  my::map<int, int> mp{{1, 10}, {2, 20}, {3, 30}};
  my::map<int, int> mp1 = std::move(mp);

  EXPECT_EQ(mp1.size(), 3);
  EXPECT_EQ(mp.size(), 0);
  EXPECT_EQ(mp1.max_size() > 0, true);

  for (int i = 1; i < 4; i++) EXPECT_EQ(i * 10, mp1.at(i));
}

TEST(map, Test_7) {
  my::map<int, char> mp{{1, '1'}, {2, '2'}, {3, '3'}, {4, '4'}};

  my::map<int, char> mp1{{3, '3'}, {4, '4'}, {5, '5'}, {6, '6'}};

  mp.merge(mp1);

  EXPECT_EQ(mp.size(), 6);
  EXPECT_EQ(mp.at(1), '1');
  EXPECT_EQ(mp.at(2), '2');
  EXPECT_EQ(mp[3], '3');
  EXPECT_EQ(mp[4], '4');
  EXPECT_EQ(mp[5], '5');
  EXPECT_EQ(mp[6], '6');
}

TEST(map, Test_8) {
  my::map<int, char> mp{{1, '1'}, {2, '2'}, {3, '3'}, {4, '4'}};

  my::map<int, char> mp1{{5, '5'}, {6, '6'}};

  mp.swap(mp1);

  EXPECT_EQ(mp.size(), 2);
  EXPECT_EQ(mp1.size(), 4);
  EXPECT_EQ(mp1.at(1), '1');
  EXPECT_EQ(mp1.at(2), '2');
  EXPECT_EQ(mp1[3], '3');
  EXPECT_EQ(mp1[4], '4');
  EXPECT_EQ(mp[5], '5');
  EXPECT_EQ(mp[6], '6');
}

TEST(map, Test_9) {
  my::map<int, int> mp;

  for (int i = 0; i < 100; i++) {
    mp.insert(i, i);
  }
  EXPECT_EQ(mp.size(), 100);
  for (int i = 0; i < 100; i++) mp.erase(mp.find(i));

  EXPECT_EQ(mp.size(), 0);
  for (int i = 100; i > 0; i--) {
    mp.insert(i, i);
  }
  EXPECT_EQ(mp.size(), 100);
  for (int i = 100; i > 0; i--) mp.erase(mp.find(i));
  EXPECT_EQ(mp.size(), 0);
}

TEST(map, Test_10) {
  my::map<int, char> mp;

  mp.insert(34, 'a');
  mp.insert(2, 'b');
  mp.insert(3, 'a');
  mp.insert(23, 'b');
  mp.insert(35, 'a');
  mp.insert(14, 'b');
  mp.insert(36, 'a');
  mp.insert(18, 'b');
  mp.insert(28, 'a');
  mp.insert(9, 'b');
  mp.insert(14, 'a');
  mp.insert(56, 'b');

  mp.erase(mp.find(34));
  mp.erase(mp.find(2));
  mp.erase(mp.find(3));
  mp.erase(mp.find(23));
  mp.erase(mp.find(35));
  mp.erase(mp.find(14));
  mp.erase(mp.find(18));
  mp.erase(mp.find(28));
  mp.erase(mp.find(9));
  mp.erase(mp.find(14));
  mp.erase(mp.find(36));

  mp.erase(mp.find(56));
}

TEST(map, Test_11) {
  my::map<int, int> mp{{1, 11}, {2, 22}, {3, 33}};

  auto res =
      mp.insert_many<std::pair<int, int>>(std::pair(4, 44), std::pair(3, 333));

  EXPECT_EQ(mp.size(), 4);
  EXPECT_EQ(res[0].second, true);
  EXPECT_EQ(res[1].second, false);
}