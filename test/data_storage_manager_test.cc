#include <gtest/gtest.h>
#include <string.h>

#include "DataStorageManager.h"

TEST(DataStorageManagerTest, SampleTest) {
  DSMgr *ds_mgr = new DSMgr("test.db");
  char buf[PAGE_SIZE];
  char buf2[PAGE_SIZE];
  for (int i = 0; i < 30; i++) {
    Page *p = new Page();
    sprintf(p->GetData(), "hello world, this is page %d", i);
    ds_mgr->WritePage(p);
  }
  for (int i = 0; i < 30; i++) {
    ds_mgr->ReadPage(i, buf);
    sprintf(buf2, "hello world, this is page %d", i);
    EXPECT_EQ(strcmp(buf, buf2), 0);
  }
  remove("test.db");
  delete ds_mgr;
}
