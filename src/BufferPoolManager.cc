#include "BufferPoolManager.h"

BMgr::BMgr() {
  for (int i = 0; i < BUF_SIZE; i++) {
    free_list_.push_back(i);
    pages_[i] = new Page();
  }
  lru_replacer_ = new LRUReplacer(BUF_SIZE);
  disk_manager_ = new DSMgr();
}

frame_id_t BMgr::FixPage(int page_id) {
}

frame_id_t BMgr::FixNewPage(page_id_t &page_id) {
  int flag = 0;
  for (int i = 0; i < BUF_SIZE; i++) {
    if (pages_[i]->GetPinCount() == 0) {
      flag = 1;
      break;
    }
  }
  if (!flag)
    return -1;
  page_id_t new_page_id;
  if (!free_list_.empty()) {
    new_page_id = free_list_.front();
    free_list_.pop_front();
  }
}
