#include <gtest/gtest.h>

#include "BufferPoolManager.h"

TEST(BufferPoolManagerTest, SampleTest) {
  BMgr *bmgr = new BMgr("test.db", 5);
  page_id_t pid;
  for (int i = 0; i < 5; i++) {
    bmgr->FixNewPage(pid);
    EXPECT_EQ(pid, i);
    bmgr->UnfixPage(pid);
  }
  for (int i = 5; i < 10; i++) {
    EXPECT_EQ(bmgr->FixNewPage(pid), i-5);
    EXPECT_EQ(pid, i);
    bmgr->UnfixPage(pid);
  }
  // 5 6 7 8 9
  bmgr->FixPage(5);
  bmgr->UnfixPage(5);
  bmgr->FixPage(8);
  bmgr->UnfixPage(8);
  bmgr->FixPage(6);
  bmgr->UnfixPage(6);
  // 7 9 5 8 6
  int expect2[5] = {7, 9, 5, 8, 6};
  for (int i = 0; i < 5; i++) {
    EXPECT_EQ(expect2[i], bmgr->SelectVictim());
  }
  remove("test.db");
  delete bmgr;
}
