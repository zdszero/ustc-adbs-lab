#include <gtest/gtest.h>
#include <string.h>

#include "DataStorageManager.h"

TEST(DataStorageManagerTest, SampleTest) {
  DSMgr *ds_mgr = new DSMgr("test.db");
  char buf[PAGE_SIZE];
  char buf2[PAGE_SIZE];
  for (int i = 0; i < 30; i++) {
    Page *p = new Page(i);
    sprintf(p->GetData(), "hello world, this is page %d", i);
    ds_mgr->WritePage(p);
  }
  for (int i = 0; i < 30; i++) {
    ds_mgr->ReadPage(i, buf);
    sprintf(buf2, "hello world, this is page %d", i);
    EXPECT_EQ(strcmp(buf, buf2), 0);
  }
  delete ds_mgr;
  remove("test.db");
}

TEST(DataStorageManagerTest, SampleTest2) {
  DSMgr *ds_mgr = new DSMgr("test.db");
  Page **pages;
  pages = new Page*[10];
  for (int i = 0; i < 10; i++) {
    pages[i] = new Page();
    pages[i]->SetPageId(ds_mgr->NewPage());
  }
  for (int i = 0; i < 10; i++) {
    ds_mgr->WritePage(pages[i]);
  }
  delete ds_mgr;
  remove("test.db");
}
