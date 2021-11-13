#include <gtest/gtest.h>
#include <vector>

#include "ClockReplacer.h"

TEST(ClockReplacerTest, DISABLED_SampleTest) {
  ClockReplacer *replacer = new ClockReplacer(4);
  std::vector<int> v = {2,  5, 5, 10, 1, 2, 2, 6, 9, 1, 2,
                        10, 2, 6, 1,  2, 1, 6, 9, 5, 1, 3};
  for (int x : v) {
    printf("insert %d\n", x);
    replacer->Insert(x);
    replacer->Print();
  }
  delete replacer;
}

TEST(ClockReplacerTest, DISABLED_SampleTest2) {
  ClockReplacer *replacer = new ClockReplacer(4);
  replacer->Insert(2);
  replacer->Insert(5);
  replacer->Insert(10);
  replacer->Insert(1);
  EXPECT_EQ(replacer->Victim(), 2);
  EXPECT_EQ(replacer->Size(), 3);
  replacer->Insert(5);
  replacer->Print();
  EXPECT_EQ(replacer->Victim(), 10);
  replacer->Print();
  replacer->Remove(5);
  EXPECT_EQ(replacer->Size(), 1);
  delete replacer;
}

TEST(ClockReplacerTest, SampleTest3) {
  ClockReplacer *replacer = new ClockReplacer(4);
  replacer->Insert(1);
  replacer->Insert(2);
  replacer->Insert(3);
  replacer->Insert(4);
  EXPECT_EQ(replacer->Victim(), 1);
  EXPECT_EQ(replacer->Victim(), 2);
  replacer->Remove(4);
  EXPECT_EQ(replacer->Size(), 1);
  replacer->Insert(5);
  replacer->Insert(6);
  EXPECT_EQ(replacer->Size(), 3);
  replacer->Print();
  replacer->Remove(6);
  replacer->Remove(5);
  replacer->Remove(3);
  EXPECT_EQ(replacer->Size(), 0);
  delete replacer;
}
