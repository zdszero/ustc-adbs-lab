#include <gtest/gtest.h>
#include <iostream>

#include "BufferPoolManager.h"

TEST(BufferPoolManagerTest, SampleTest) {
  BMgr *bmgr = new BMgr("test.db", 5);
  page_id_t pid;
  for (int i = 0; i < 5; i++) {
    frame_id_t fid = bmgr->FixNewPage(pid);
    EXPECT_EQ(pid, i);
    bmgr->UnfixPage(pid);
  }
  // 0 1 2 3 4
  for (int i = 5; i < 10; i++) {
    EXPECT_EQ(bmgr->FixNewPage(pid), i - 5);
    EXPECT_EQ(pid, i);
    bmgr->UnfixPage(pid);
  }
  bmgr->PrintPageTable();
  bmgr->PrintReplacer();
  // 0 1 2 3 4
  bmgr->FixPage(5);
  bmgr->UnfixPage(5);
  bmgr->FixPage(8);
  bmgr->UnfixPage(8);
  bmgr->FixPage(6);
  bmgr->UnfixPage(6);
  bmgr->PrintReplacer();
  // 2 4 0 3 1
  int expect2[5] = {2, 4, 0, 3, 1};
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(expect2[i], bmgr->SelectVictim());
  }
  delete bmgr;
  remove("test.db");
}

TEST(BufferPoolManager, SampleTest2) {
  // demo.db has size 120K == 30 pages
  BMgr *bmgr = new BMgr("demo.db", 5);
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(bmgr->FixPage(i), i);
    bmgr->UnfixPage(i);
  }
  EXPECT_EQ(bmgr->GetIONum(), 5);
  EXPECT_EQ(bmgr->GetHitNum(), 0);
  // 0 1 2 3 4
  for (int i = 4; i >= 0; i--) {
    EXPECT_EQ(bmgr->FixPage(i), i);
    bmgr->UnfixPage(i);
  }
  // 4 3 2 1 0
  EXPECT_EQ(bmgr->GetIONum(), 5);
  EXPECT_EQ(bmgr->GetHitNum(), 5);
  bmgr->SetDirty(4);
  bmgr->SetDirty(3);
  EXPECT_EQ(bmgr->FixPage(5), 4);
  bmgr->UnfixPage(5);
  EXPECT_EQ(bmgr->FixPage(6), 3);
  bmgr->UnfixPage(6);
  EXPECT_EQ(bmgr->GetIONum(), 9);
  // 2 1 0 4 3
  int expect[5] = {2, 1, 0, 4, 3};
  for (int i = 10; i < 15; i++) {
    EXPECT_EQ(bmgr->FixPage(i), expect[i - 10]);
    bmgr->UnfixPage(i);
  }
  bmgr->PrintPageTable();
  EXPECT_EQ(bmgr->GetIONum(), 14);
  bmgr->SetDirty(1);
  bmgr->SetDirty(3);
  bmgr->FixPage(10);
  bmgr->UnfixPage(10);
  bmgr->FixPage(12);
  bmgr->UnfixPage(12);
  // 1 4 3 2 0
  EXPECT_EQ(bmgr->FixPage(20), 1);
  EXPECT_EQ(bmgr->FixPage(21), 4);
  EXPECT_EQ(bmgr->FixPage(22), 3);
  // 2 0
  bmgr->PrintPageTable();
  bmgr->PrintReplacer();
  EXPECT_EQ(bmgr->FixPage(25), 2);
  EXPECT_EQ(bmgr->FixPage(26), 0);
  EXPECT_EQ(bmgr->FixPage(27), -1);
  EXPECT_EQ(bmgr->UnfixPage(27), -1);
  bmgr->PrintReplacer();
  delete bmgr;
}

TEST(BufferPoolManagerTest, SampleTest3) {
  BMgr *bmgr = new BMgr("demo.db", 5);
  EXPECT_EQ(bmgr->GetHitNum(), 0);
  for (int i = 10; i < 15; i++) {
    bmgr->FixPage(i, i % 2);
  }
  EXPECT_EQ(bmgr->GetHitNum(), 0);
  for (int i = 10; i < 15; i++) {
    bmgr->FixPage(i);
    bmgr->UnfixPage(i);
    bmgr->UnfixPage(i);
  }
  EXPECT_EQ(bmgr->GetHitNum(), 5);
  for (int i = 20; i < 25; i++) {
    bmgr->FixPage(i);
  }
  EXPECT_EQ(bmgr->GetIONum(), 12);
  delete bmgr;
}
