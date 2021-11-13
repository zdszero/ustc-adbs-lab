#pragma once

#include <list>
#include <unordered_map>

#include "DataStorageManager.h"
#include "Replacer.h"

#define FRAME_NUM 1024

enum ReplacePolicy { Invalid = 0, Lru, Clock };

class BMgr {
public:
  BMgr(std::string filename, int policy = ReplacePolicy::Lru,
       int frame_num = FRAME_NUM);
  ~BMgr();
  // Interface functions
  frame_id_t FixPage(int page_id);
  frame_id_t FixPage(int page_id, bool is_dirty);
  frame_id_t FixNewPage(page_id_t &page_id);
  frame_id_t UnfixPage(int page_id);
  frame_id_t SelectVictim();
  int GetIONum();
  int GetHitNum();
  void PrintPageTable();
  void PrintReplacer();
  // int NumFreeFrames();
  // Internal Functions
  /* int Hash(int page_id); */
  /* void RemoveBCB(BCB *ptr, int page_id); */
  /* void RemoveLRUEle(int frid); */
  void SetDirty(int frame_id);
  void UnsetDirty(int frame_id);
  void WriteDirtys();
  void PrintFrame(int frame_id);

private:
  DSMgr *disk_manager_;
  int frame_num_;
  Page **pages_;
  std::list<int> free_list_;
  std::unordered_map<page_id_t, frame_id_t> page_table_;
  int num_io_;
  int num_hits_;
  Replacer *replacer_;
};
