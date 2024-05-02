#include <gtest/gtest.h>

#include "../headers/my_queue.h"

using namespace my;
using namespace std;

TEST(MYQueueTestConst, ConstructorTest) {
  Queue<int> queue;
  EXPECT_EQ(queue.size(), 0);
}

TEST(MYQueueTestConst, ConstructorTest1) {
  Queue<int> queue{1, 2, 3, 4, 5};
  EXPECT_EQ(queue.size(), 5);
  EXPECT_EQ(queue.front(), 1);
  EXPECT_EQ(queue.back(), 5);
}

TEST(MYQueueTestConst, ConstructorTest3) {
  const Queue<int> queue1 = {1, 2, 3, 4, 5, 6, 7};
  Queue<int> queue(queue1);
  EXPECT_EQ(queue.size(), 7);
  EXPECT_EQ(queue.front(), 1);
  EXPECT_EQ(queue.back(), 7);
}

TEST(MYQueueTestConst, ConstructorTest4) {
  Queue<int> queue1 = {1, 2, 3, 4, 5, 6, 7};
  Queue<int> queue2(std::move(queue1));
  EXPECT_EQ(queue2.size(), 7);
  EXPECT_EQ(queue2.front(), 1);
  EXPECT_EQ(queue2.back(), 7);
  EXPECT_EQ(queue1.size(), 0);
}

TEST(MYQueueTestOperator, Operator_Equal) {
  Queue<int> queue1 = {1, 2, 3, 4, 5, 6, 7};
  Queue<int> queue2;

  queue2 = std::move(queue1);
  EXPECT_EQ(queue2.size(), 7);
  EXPECT_EQ(queue2.front(), 1);
  EXPECT_EQ(queue2.back(), 7);
  EXPECT_EQ(queue1.size(), 0);
}

TEST(MYQueueTest, Method_Front) {
  Queue<int> queue1 = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(queue1.front(), 1);
}

TEST(MYQueueTest, Method_Back) {
  Queue<int> queue1 = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(queue1.back(), 7);
}

TEST(MYQueueTest, Method_Empty) {
  Queue<int> list1 = {1, 2, 3, 4, 5, 6, 7};
  EXPECT_EQ(list1.empty(), false);
}

TEST(MYQueueTest, Method_Push) {
  Queue<int> queue1 = {1, 2, 3, 4, 5, 6, 7};
  queue1.push(8);
  EXPECT_EQ(queue1.size(), 8);
  EXPECT_EQ(queue1.back(), 8);
}

TEST(MYQueueTest, Method_Insert_many_back) {
  Queue<int> queue1 = {1, 2, 3, 4, 5, 6, 7};
  queue1.insert_many_back(-1, -2, -3);
  EXPECT_EQ(queue1.size(), 10);
  EXPECT_EQ(queue1.back(), -3);
}

TEST(MYQueueTest, Method_Push2) {
  Queue<int> queue1;
  queue1.push(1);
  queue1.push(2);
  queue1.push(3);
  queue1.push(4);
  EXPECT_EQ(queue1.size(), 4);
  EXPECT_EQ(queue1.back(), 4);
}

TEST(MYQueueTest, Method_Pop) {
  Queue<int> queue = {1, 2, 3, 4, 5, 6, 7};
  queue.pop();
  EXPECT_EQ(queue.front(), 2);
  EXPECT_EQ(queue.size(), 6);
}

TEST(MYQueueTest, Method_Swap) {
  Queue<int> queue1 = {1, 2, 3, 4, 5, 6, 7};
  Queue<int> queue2 = {7, 6, 5, 4, 3, 2, 1, 0};
  queue1.swap(queue2);
  EXPECT_EQ(queue1.size(), 8);
  EXPECT_EQ(queue2.size(), 7);
  EXPECT_EQ(queue2.front(), 1);
  EXPECT_EQ(queue1.front(), 7);
}
