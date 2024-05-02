#include <gtest/gtest.h>

#include "../headers/my_stack.h"

TEST(Stack, Test_1) {
  my::stack<int> s;
  int j = 9;
  EXPECT_EQ(s.size(), 0);
  for (int i = 0; i < 10; i++) s.push(i);
  EXPECT_EQ(s.size(), 10);
  while (!s.empty()) {
    EXPECT_EQ(j, s.top());
    s.pop();
    j--;
  }
  EXPECT_EQ(s.size(), 0);
}

TEST(Stack, Test_2) {
  my::stack<int> s;
  int j = 9999;
  for (int i = 0; i < 10000; i++) s.push(i);
  EXPECT_EQ(s.size(), 10000);
  while (!s.empty()) {
    EXPECT_EQ(s.top(), j);
    s.pop();
    j--;
  }

  EXPECT_EQ(s.size(), 0);
}

TEST(Stack, Test_3) {
  my::stack<int> s{5, 6, 7, 8, 9, 10, 11, 12};

  for (int i = 12; !s.empty(); i--) {
    EXPECT_EQ(i, s.top());
    s.pop();
  }
}

TEST(Stack, Test_4) {
  my::stack<int> s{5, 6, 7, 8, 9, 10, 11, 12};
  my::stack<int> s1(s);

  EXPECT_EQ(s.size(), 8);
  EXPECT_EQ(s1.size(), 8);

  for (int i = 12; !s.empty(); i--) {
    EXPECT_EQ(i, s.top());
    EXPECT_EQ(i, s1.top());

    s.pop();
    s1.pop();
  }

  EXPECT_EQ(s.size(), 0);
  EXPECT_EQ(s1.size(), 0);
}

TEST(Stack, Test_5) {
  my::stack<int> s{5, 6, 7, 8, 9, 10, 11, 12};
  my::stack<int> s1{5, 6, 7, 8, 9, 10, 11, 12};
  my::stack<int> s2 = std::move(s);
  my::stack<int> s3(std::move(s1));

  EXPECT_EQ(s2.size(), 8);
  EXPECT_EQ(s3.size(), 8);

  for (int i = 12; !s2.empty(); i--) {
    EXPECT_EQ(i, s2.top());
    EXPECT_EQ(i, s3.top());
    s2.pop();
    s3.pop();
  }

  EXPECT_EQ(s2.size(), 0);
  EXPECT_EQ(s3.size(), 0);
}

TEST(Stack, Test_6) {
  my::stack<int> s;

  EXPECT_EQ(s.size(), 0);
  EXPECT_THROW(s.top(), std::runtime_error);
}

TEST(Stack, Test_7) {
  my::stack<int> s{1, 2, 3, 4, 5};
  my::stack<int> s1{6, 7, 8, 9, 10};
  s.swap(s1);

  for (int i = 1; i <= 5; i++) {
    EXPECT_EQ(s.top() - 5, s1.top());
    s.pop();
    s1.pop();
  }
}

TEST(Stack, Test_8) {
  my::stack<int> s0;
  my::stack<int> s{1};
  my::stack<int> s1{1, 2, 3, 4, 5, 6, 7, 8};
}

TEST(Stack, Test_9) {
  my::stack<int> s{1, 2};

  s.insert_many_front(3, 4);

  for (int i = 4; i >= 1; i--) {
    EXPECT_EQ(i, s.top());
    s.pop();
  }
}