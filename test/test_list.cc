#include <gtest/gtest.h>

#include "../headers/my_list.h"

using namespace my;
using namespace std;

TEST(MYListTestConst, ConstructorTest) {
  List<int> list;
  EXPECT_EQ(list.size(), 0);
}

TEST(MYListTestConst, ConstructorTest1) {
  List<int> list(5);
  EXPECT_EQ(list.size(), 5);
  EXPECT_EQ(list.front(), 0);
  EXPECT_EQ(list.back(), 0);
}

TEST(MYListTestConst, ConstructorTest2) {
  List<int> list = {1, 2, 3, 4, 5};
  EXPECT_EQ(list.size(), 5);
  EXPECT_EQ(list.front(), 1);
  EXPECT_EQ(list.back(), 5);
}

TEST(MYListTestConst, ConstructorTest3) {
  const List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  List<int> list(list1);
  EXPECT_EQ(list.size(), 7);
  EXPECT_EQ(list.front(), 1);
  EXPECT_EQ(list.back(), 7);
}

TEST(MYListTestConst, ConstructorTest4) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  List<int> list2(std::move(list1));
  EXPECT_EQ(list2.size(), 7);
  EXPECT_EQ(list2.front(), 1);
  EXPECT_EQ(list2.back(), 7);
  EXPECT_EQ(list1.size(), 0);
}

TEST(MYListTestOperator, Operator_Equal) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  List<int> list2;

  list2 = std::move(list1);

  EXPECT_EQ(list2.size(), 7);
  EXPECT_EQ(list2.front(), 1);
  EXPECT_EQ(list2.back(), 7);
  EXPECT_EQ(list1.size(), 0);
}

TEST(MYListTest, Method_Front) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(list1.front(), 1);
}

TEST(MYListTest, Method_Back) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(list1.back(), 7);
}

TEST(MYListTest, Method_Empty) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(list1.empty(), false);
}

TEST(MYListTest, Method_Clear) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  list1.clear();
  EXPECT_EQ(list1.empty(), true);
}

TEST(MYListTest, Method_Push_back) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  list1.push_back(8);
  EXPECT_EQ(list1.size(), 8);
  EXPECT_EQ(list1.back(), 8);
}

TEST(MYListTest, Method_Push_front) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  list1.push_front(8);
  EXPECT_EQ(list1.size(), 8);
  EXPECT_EQ(list1.front(), 8);
}

TEST(MYListTest, Method_Pop_back) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  list1.pop_back();
  EXPECT_EQ(list1.size(), 6);
  EXPECT_EQ(list1.back(), 6);
}

TEST(MYListTest, Method_Pop_front) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  list1.pop_front();
  EXPECT_EQ(list1.size(), 6);
  EXPECT_EQ(list1.front(), 2);
}

TEST(MYListTest, Method_insert) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  auto it = list1.begin();
  ++it;
  auto new_pos = list1.insert(it, -1);
  EXPECT_EQ(list1.size(), 8);
  EXPECT_EQ(new_pos.getNode()->value_, -1);
}

TEST(MYListTest, Method_insert_many) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  auto it = list1.begin();
  ++it;
  list1.insert_many(it, -1, -2, -3);
  EXPECT_EQ(list1.size(), 10);
}

TEST(MYListTest, Method_insert_many_back) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  list1.insert_many_back(-1, -2, -3);
  EXPECT_EQ(list1.size(), 10);
  EXPECT_EQ(list1.back(), -3);
}

TEST(MYListTest, Method_insert_many_front) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  list1.insert_many_front(-1, -2, -3);
  EXPECT_EQ(list1.size(), 10);
  EXPECT_EQ(list1.front(), -3);
}

TEST(MYListTest, Method_Swap) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  List<int> list2 = {7, 6, 5, 4, 3, 2, 1, 0};
  list1.swap(list2);
  EXPECT_EQ(list1.size(), 8);
  EXPECT_EQ(list2.size(), 7);
  EXPECT_EQ(list2.front(), 1);
  EXPECT_EQ(list1.front(), 7);
}

TEST(MYListTest, Method_Erase) {
  List<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  List<int>::ListIterator it = list1.begin();
  it++;
  it++;
  it++;
  list1.erase(it);
  EXPECT_EQ(list1.size(), 6);
}

TEST(MYListTest, Method_Merge) {
  List<int> list1 = {5, 9, 1, 3, 3};
  List<int> list2 = {12, 7, 2, 3, 4, 4};
  list1.merge(list2);
  EXPECT_EQ(list1.size(), 11);
  EXPECT_EQ(list1.front(), 1);
  EXPECT_EQ(list1.back(), 12);
}

TEST(MYListTest, Method_Splice) {
  List<int> list1{1, 2, 3, 4, 5};
  List<int> list2{10, 20, 30, 40, 50};
  auto it = list1.begin();
  it++;
  it++;
  list1.splice(it, list2);
  EXPECT_EQ(list1.size(), 10);
  EXPECT_EQ(list1.front(), 1);
  EXPECT_EQ(list1.back(), 5);
}

TEST(MYListTest, Method_Reverse) {
  List<int> list1{10, 20, 30, 40, 50};
  list1.reverse();
  EXPECT_EQ(list1.front(), 50);
  EXPECT_EQ(list1.back(), 10);
}

TEST(MYListTest, Method_Unique) {
  List<int> list1{1, 2, 2, 3, 3, 2, 1, 1, 2};
  list1.unique();
  EXPECT_EQ(list1.size(), 6);
}

TEST(MYListTest, Method_Sort) {
  List<int> list1{8, 7, 5, 9, 0, 1, 3, 2, 6, 4};
  list1.sort();
  EXPECT_EQ(list1.front(), 0);
  EXPECT_EQ(list1.back(), 9);
}
