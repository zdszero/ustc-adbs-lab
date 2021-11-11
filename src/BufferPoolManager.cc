#include "BufferPoolManager.h"

#include <cstdio>
#include <cassert>

BMgr::BMgr(std::string filename, int frame_num): frame_num_(frame_num) {
  pages_ = new Page*[frame_num];
  for (int i = 0; i < frame_num; i++) {
    free_list_.push_back(i);
    pages_[i] = new Page();
  }
  replacer_ = new LRUReplacer(frame_num);
  disk_manager_ = new DSMgr(filename);
}

BMgr::~BMgr() {
  delete disk_manager_;
  delete replacer_;
  for (int i = 0; i < frame_num_; i++) {
    if (pages_[i]->IsDirty()) {
      disk_manager_->WritePage(pages_[i]);
    }
    delete pages_[i];
  }
  delete pages_;
}

frame_id_t BMgr::FixPage(int page_id) {
  auto iter = page_table_.find(page_id);
  if (iter != page_table_.end()) {
    frame_id_t frame_id = iter->second;
    if (pages_[frame_id]->GetPinCount() == 0) {
      replacer_->Remove(frame_id);
    }
    pages_[frame_id]->IncPinCount();
    return frame_id;
  }
  return FixNewPage(page_id);
}

frame_id_t BMgr::FixNewPage(page_id_t &page_id) {
  frame_id_t frame_id = SelectVictim();
  page_id = disk_manager_->NewPage();
  Page *p = pages_[frame_id];
  page_table_[page_id] = frame_id;
  assert(p->GetPinCount() == 0);
  p->IncPinCount();
  p->SetPageId(page_id);
  SetDirty(frame_id);
  return frame_id;
}

frame_id_t BMgr::SelectVictim() {
  frame_id_t frame_id = -1;
  if (!free_list_.empty()) {
    frame_id = free_list_.front();
    free_list_.pop_front();
    return frame_id;
  }
  return replacer_->Victim();
}

frame_id_t BMgr::UnfixPage(int page_id) {
  auto iter = page_table_.find(page_id);
  frame_id_t frame_id = -1;
  if (iter != page_table_.end()) {
    frame_id = iter->second;
    Page *p = pages_[frame_id];
    assert(p->GetPinCount() > 0);
    p->DecPinCount();
    if (p->GetPinCount() == 0) {
      if (p->IsDirty()) {
        disk_manager_->WritePage(p);
        UnsetDirty(frame_id);
      }
      replacer_->Insert(page_id);
    }
  }
  return frame_id;
}

void BMgr::SetDirty(int frame_id) {
  pages_[frame_id]->SetDirty(true);
}

void BMgr::UnsetDirty(int frame_id) {
  pages_[frame_id]->SetDirty(false);
}

void BMgr::PrintFrame(int frame_id) {
  printf("frame %d:\n", frame_id);
  Page *p = pages_[frame_id];
  printf("page id: %d\n", p->GetPageId());
  printf("dirty: %d\n", p->IsDirty());
  printf("pin count: %d\n", p->GetPinCount());
  printf("\n");
}
