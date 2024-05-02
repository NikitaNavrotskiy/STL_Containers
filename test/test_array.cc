#include <gtest/gtest.h>

#include "../headers/my_array.h"

TEST(Array, Test_1) {
  my::array<int, 4> arr;
  my::array<int, 3> arr1{1, 2, 4};
  EXPECT_EQ(arr[0], 0);
  EXPECT_EQ(arr[1], 0);
  EXPECT_EQ(arr[2], 0);
  EXPECT_EQ(arr[3], 0);

  EXPECT_EQ(arr1[0], 1);
  EXPECT_EQ(arr1[1], 2);
  EXPECT_EQ(arr1[2], 4);

  EXPECT_EQ(arr.size(), 4);
  EXPECT_EQ(arr1.size(), 3);

  arr[0] = 5;
  arr[1] = 6;
  arr[2] = 7;
  arr[3] = 8;

  arr1[0] = 9;
  arr1[1] = 10;
  arr1[2] = 11;

  EXPECT_EQ(arr[0], 5);
  EXPECT_EQ(arr[1], 6);
  EXPECT_EQ(arr[2], 7);
  EXPECT_EQ(arr[3], 8);

  EXPECT_EQ(arr1[0], 9);
  EXPECT_EQ(arr1[1], 10);
  EXPECT_EQ(arr1[2], 11);
}

TEST(Array, Test_2) {
  my::array<int, 4> arr{1, 2, 3, 4};
  my::array<int, 4> arr1(arr);

  EXPECT_EQ(arr1[0], 1);
  EXPECT_EQ(arr1[1], 2);
  EXPECT_EQ(arr1[2], 3);
  EXPECT_EQ(arr1[3], 4);
}

TEST(Array, Test_3) {
  my::array<int, 4> arr{1, 2, 3, 4};
  my::array<int, 4> arr1 = std::move(arr);

  EXPECT_EQ(arr[0], 0);
  EXPECT_EQ(arr[1], 0);
  EXPECT_EQ(arr[2], 0);
  EXPECT_EQ(arr[3], 0);

  EXPECT_EQ(arr1[0], 1);
  EXPECT_EQ(arr1[1], 2);
  EXPECT_EQ(arr1[2], 3);
  EXPECT_EQ(arr1[3], 4);
}

TEST(Array, Test_4) {
  my::array<int, 4> arr{2, 3, 4, 5};

  EXPECT_THROW(arr.at(4), std::runtime_error);
  EXPECT_THROW(arr.at(40), std::runtime_error);

  EXPECT_EQ(arr[0], 2);
  EXPECT_EQ(arr[1], 3);
  EXPECT_EQ(arr[2], 4);
  EXPECT_EQ(arr[3], 5);
}

TEST(Array, Test_5) {
  my::array<int, 4> arr1;
  my::array<int, 4> arr{2, 3, 4, 5};
  my::array<int, 4> arr2{1, 2, 3};

  EXPECT_EQ(arr1.front(), 0);
  EXPECT_EQ(arr1.back(), 0);
  EXPECT_EQ(arr.front(), 2);
  EXPECT_EQ(arr.back(), 5);
  EXPECT_EQ(arr2.front(), 1);
  EXPECT_EQ(arr2.back(), 0);
}

TEST(Array, Test_6) {
  my::array<int, 4> arr;
  my::array<int, 4> arr1{1, 2, 3, 4};
  my::array<int, 0> arr2;

  EXPECT_TRUE(arr2.empty());
  EXPECT_FALSE(arr.empty());
  EXPECT_FALSE(arr1.empty());
  EXPECT_EQ(arr.max_size(), 4);
  EXPECT_EQ(arr1.max_size(), 4);
}

TEST(Array, Test_7) {
  my::array<int, 4> arr{1, 2, 3, 4};
  int i = 1;
  for (auto it = arr.begin(); it != arr.end(); i++, it++) EXPECT_EQ(*it, i);

  EXPECT_EQ(arr.max_size(), 4);
}

TEST(Array, Test_8) {
  my::array<int, 4> arr{1, 2, 3, 4};

  int *ptr = arr.data();

  for (int i = 0; i < 4; i++) EXPECT_EQ(*(ptr + i), i + 1);
}

TEST(Array, Test_9) {
  my::array<int, 34> arr;

  arr.fill(-86);

  for (auto a : arr) EXPECT_EQ(a, -86);
}

TEST(Array, Test_10) {
  my::array<int, 10> arr{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  my::array<int, 10> arr1{11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
  arr1.swap(arr);

  for (int i = 0; i < 10; i++) {
    EXPECT_EQ(arr[i], i + 11);
    EXPECT_EQ(arr1[i], i + 1);
  }
}

TEST(Array, Test_11) {
  const my::array<int, 4> arr;
  const my::array<int, 3> arr1{1, 2, 4};
  EXPECT_EQ(arr[0], 0);
  EXPECT_EQ(arr[1], 0);
  EXPECT_EQ(arr[2], 0);
  EXPECT_EQ(arr[3], 0);

  EXPECT_EQ(arr1[0], 1);
  EXPECT_EQ(arr1[1], 2);
  EXPECT_EQ(arr1[2], 4);

  EXPECT_EQ(arr.size(), 4);
  EXPECT_EQ(arr1.size(), 3);
}

TEST(Array, Test_12) {
  const my::array<int, 4> arr{1, 2, 3, 4};
  const my::array<int, 4> arr1(arr);

  EXPECT_EQ(arr1[0], 1);
  EXPECT_EQ(arr1[1], 2);
  EXPECT_EQ(arr1[2], 3);
  EXPECT_EQ(arr1[3], 4);
}

TEST(Array, Test_13) {
  const my::array<int, 4> arr{2, 3, 4, 5};

  EXPECT_THROW(arr.at(4), std::runtime_error);
  EXPECT_THROW(arr.at(40), std::runtime_error);

  EXPECT_EQ(arr[0], 2);
  EXPECT_EQ(arr[1], 3);
  EXPECT_EQ(arr[2], 4);
  EXPECT_EQ(arr[3], 5);
}

TEST(Array, Test_14) {
  const my::array<int, 4> arr1;
  const my::array<int, 4> arr{2, 3, 4, 5};
  const my::array<int, 4> arr2{1, 2, 3};

  EXPECT_EQ(arr1.front(), 0);
  EXPECT_EQ(arr1.back(), 0);
  EXPECT_EQ(arr.front(), 2);
  EXPECT_EQ(arr.back(), 5);
  EXPECT_EQ(arr2.front(), 1);
  EXPECT_EQ(arr2.back(), 0);
}

TEST(Array, Test_15) {
  const my::array<int, 4> arr;
  const my::array<int, 4> arr1{1, 2, 3, 4};
  const my::array<int, 0> arr2;

  EXPECT_TRUE(arr2.empty());
  EXPECT_FALSE(arr.empty());
  EXPECT_FALSE(arr1.empty());
  EXPECT_EQ(arr.max_size(), 4);
  EXPECT_EQ(arr1.max_size(), 4);
}

TEST(Array, Test_16) {
  const my::array<int, 4> arr{1, 2, 3, 4};
  int i = 1;
  for (auto it = arr.begin(); it != arr.end(); i++, it++) EXPECT_EQ(*it, i);

  EXPECT_EQ(arr.max_size(), 4);
}

TEST(Array, Test_17) {
  const my::array<int, 4> arr{1, 2, 3, 4};

  const int *ptr = arr.data();

  for (int i = 0; i < 4; i++) EXPECT_EQ(*(ptr + i), i + 1);
}
