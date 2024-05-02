#include <gtest/gtest.h>

#include "../headers/my_vector.h"

using namespace my;

TEST(MYVectorTest, ConstructorTest) {
  Vector<int> v;
  EXPECT_EQ(v.size(), 0);
  EXPECT_EQ(v.capacity(), 15);
}

TEST(MYVectorTest, ConstructorTest1) {
  Vector<int> v(5);
  EXPECT_EQ(v.size(), 5);
}

TEST(MYVectorTest, ConstructorTest2) {
  Vector<int> v = {1, 2, 3, 4, 5};
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v.front(), 1);
  EXPECT_EQ(v.back(), 5);
}

TEST(MYVectorTest, ConstructorTest3) {
  const Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  Vector<int> vec(v);
  EXPECT_EQ(vec.size(), 7);
  EXPECT_EQ(vec.front(), 1);
  EXPECT_EQ(vec.back(), 7);
}

TEST(MYVectorTest, ConstructorTest4) {
  Vector<int> v1 = {1, 2, 3, 4, 5, 6, 7};
  Vector<int> v2(std::move(v1));
  EXPECT_EQ(v2.size(), 7);
  EXPECT_EQ(v2.front(), 1);
  EXPECT_EQ(v2.back(), 7);
  EXPECT_EQ(v1.size(), 0);
}

TEST(MYVectorTest, Operator_Equal) {
  Vector<int> v1 = {1, 2, 3, 4, 5, 6, 7};
  Vector<int> v2;

  v2 = std::move(v1);

  EXPECT_EQ(v2.size(), 7);
  EXPECT_EQ(v2.front(), 1);
  EXPECT_EQ(v2.back(), 7);
  EXPECT_EQ(v1.size(), 0);
}

TEST(MYVectorTest, Method_At) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  v.at(3) = 33;
  EXPECT_EQ(v.at(2), 3);
  EXPECT_EQ(v.at(3), 33);
}

TEST(MYVectorTest, Op_index) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  v[6] = 55;
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[6], 55);
}

TEST(MYVectorTest, Method_Front) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(v.front(), 1);
}

TEST(MYVectorTest, Method_Back) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(v.back(), 7);
}

TEST(MYVectorTest, Method_Data) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  auto* values = v.data();
  EXPECT_EQ(values[0], 1);
  EXPECT_EQ(values[1], 2);
}

TEST(MYVectorTest, Method_Empty) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(v.empty(), false);
}

TEST(MYVectorTest, Method_Size) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(v.size(), 7);
}

TEST(MYVectorTest, Method_Reserve) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  Vector<int> v2(v);
  v2.reserve(45);
  EXPECT_EQ(v.capacity(), 15);
  EXPECT_EQ(v2.capacity(), 45);
}

TEST(MYVectorTest, Method_Shrink_TO_FIT) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  Vector<int> v2(v);
  v2.reserve(45);
  v2.shrink_to_fit();
  EXPECT_EQ(v.capacity(), 15);
  EXPECT_EQ(v2.capacity(), 7);
}

TEST(MYVectorTest, Method_Clear) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  v.clear();
  EXPECT_EQ(v.empty(), true);
  EXPECT_EQ(v.capacity(), 15);
}

TEST(MYVectorTest, Method_Insert) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  auto it = v.begin();
  ++it;
  ++it;
  v.insert(it, 5);
  EXPECT_EQ(v[2], 5);
}

TEST(MYVectorTest, Method_Insert_Many) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};

  Vector<int>::const_iterator it = &v[2];
  v.insert_many(it, -1, -2, -3);
  EXPECT_EQ(v[2], -1);
  EXPECT_EQ(v[3], -2);
  EXPECT_EQ(v.size(), 10);
}

TEST(MYVectorTest, Method_Insert_Many_back) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};

  v.insert_many_back(-1, -2, -3);
  EXPECT_EQ(v[7], -1);
  EXPECT_EQ(v[8], -2);
  EXPECT_EQ(v.size(), 10);
}

TEST(MYVectorTest, Method_Erase_) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  v.erase(v.begin() + 1);
  EXPECT_EQ(v[1], 3);
  EXPECT_EQ(v.size(), 6);
}

TEST(MYVectorTest, Method_Push_back) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  v.push_back(8);
  EXPECT_EQ(v.size(), 8);
  EXPECT_EQ(v.back(), 8);
}

TEST(MYVectorTest, Method_Pop_back) {
  Vector<int> v = {1, 2, 3, 4, 5, 6, 7};
  v.pop_back();
  EXPECT_EQ(v.size(), 6);
  EXPECT_EQ(v.back(), 6);
}

TEST(MYVectorTest, Method_Swap) {
  Vector<int> v1 = {1, 2, 3, 4, 5, 6, 7};
  Vector<int> v2 = {7, 6, 5, 4, 3, 2, 1, 0};
  v1.swap(v2);
  EXPECT_EQ(v1.size(), 8);
  EXPECT_EQ(v2.size(), 7);
  EXPECT_EQ(v2.front(), 1);
  EXPECT_EQ(v1.front(), 7);
}

TEST(MYVectorTest, Other) {
  Vector<int> v(17);

  for (int i = 0; i < 100; i++) v.push_back(i);

  for (int i = 0; i < 100; i++) v.pop_back();

  EXPECT_THROW(v.at(1000), std::out_of_range);
}
