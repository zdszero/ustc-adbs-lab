#pragma once

#include <list>
#include <unordered_map>

#include "DataStorageManager.h"
#include "LRUReplacer.h"
#include "Page.h"

#define BUF_SIZE 1024

class BMgr {
public:
  BMgr();
  ~BMgr();
  // Interface functions
  frame_id_t FixPage(int page_id);
  frame_id_t FixNewPage(page_id_t &page_id);
  int UnfixPage(int page_id);
  int NumFreeFrames();
  // Internal Functions
  int SelectVictim();
  int Hash(int page_id);
  /* void RemoveBCB(BCB *ptr, int page_id); */
  void RemoveLRUEle(int frid);
  void SetDirty(int frame_id);
  void UnsetDirty(int frame_id);
  void WriteDirtys();
  void PrintFrame(int frame_id);

private:
  DSMgr *disk_manager_;
  Page *pages_[BUF_SIZE];
  std::list<int> free_list_;
  std::unordered_map<page_id_t, frame_id_t> page_table_;
  LRUReplacer *lru_replacer_;
};
