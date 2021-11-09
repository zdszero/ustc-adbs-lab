#include <gtest/gtest.h>
#include <iostream>
using namespace std;

#include "LRUReplacer.h"

TEST(LRUReplacerTest1, SampleTest) {
  LRUReplacer *replacer = new LRUReplacer(5);
  for (int i = 1; i <= 5; i++) {
    replacer->Insert(i);
    EXPECT_EQ(replacer->Size(), i);
  }
  replacer->Insert(1);
  // 2 3 4 5 1
  EXPECT_EQ(replacer->Size(), 5);
  // 3 4 5 1
  EXPECT_EQ(replacer->Victim(), 2);
  EXPECT_EQ(replacer->Size(), 4);
  // 3 5 1
  replacer->Remove(4);
  EXPECT_EQ(replacer->Size(), 3);
  for (int i = 7; i <= 9; i++) {
    replacer->Insert(i);
  }
  // 5 1 7 8 9
  replacer->Insert(5);
  replacer->Insert(1);
  // 7 8 9 5 1
  EXPECT_EQ(replacer->Victim(), 7);
  EXPECT_EQ(replacer->Victim(), 8);
  EXPECT_EQ(replacer->Victim(), 9);
  EXPECT_EQ(replacer->Victim(), 5);
  EXPECT_EQ(replacer->Victim(), 1);
  EXPECT_EQ(replacer->Size(), 0);
  for (int i = 10; i <= 14; i++) {
    replacer->Insert(i);
  }
  EXPECT_EQ(replacer->Size(), 5);
  delete replacer;
}
