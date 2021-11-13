#include "BufferPoolManager.h"
#include "ClockReplacer.h"
#include "LRUReplacer.h"

#include <cassert>
#include <cstdio>

BMgr::BMgr(std::string filename, int policy, int frame_num)
    : frame_num_(frame_num), num_io_(0), num_hits_(0) {
  pages_ = new Page *[frame_num];
  for (int i = 0; i < frame_num; i++) {
    free_list_.push_back(i);
    pages_[i] = new Page();
  }
  if (policy == ReplacePolicy::Lru)
    replacer_ = new LRUReplacer(frame_num);
  else if (policy == ReplacePolicy::Clock)
    replacer_ = new ClockReplacer(frame_num);
  else
    exit(1);
  disk_manager_ = new DSMgr(filename);
}

BMgr::~BMgr() {
  delete replacer_;
  for (int i = 0; i < frame_num_; i++) {
    if (pages_[i]->IsDirty()) {
      disk_manager_->WritePage(pages_[i]);
    }
    delete pages_[i];
  }
  delete pages_;
  delete disk_manager_;
}

frame_id_t BMgr::FixPage(int page_id) {
  auto iter = page_table_.find(page_id);
  frame_id_t frame_id;
  if (iter != page_table_.end()) {
    num_hits_++;
    frame_id = iter->second;
    if (pages_[frame_id]->GetPinCount() == 0) {
      replacer_->Remove(frame_id);
      /* replacer_->Print(); */
    }
    pages_[frame_id]->IncPinCount();
    return frame_id;
  }
  frame_id = SelectVictim();
  if (frame_id == -1)
    return -1;
  Page *p = pages_[frame_id];
  disk_manager_->ReadPage(page_id, p->GetData());
  num_io_++;
  page_table_[page_id] = frame_id;
  p->IncPinCount();
  p->SetPageId(page_id);
  return frame_id;
}

frame_id_t BMgr::FixPage(int page_id, bool is_dirty) {
  frame_id_t fid = FixPage(page_id);
  pages_[fid]->SetDirty(pages_[fid]->IsDirty() | is_dirty);
  return fid;
}

frame_id_t BMgr::FixNewPage(page_id_t &page_id) {
  frame_id_t frame_id = SelectVictim();
  if (frame_id == -1)
    return frame_id;
  page_id = disk_manager_->NewPage();
  Page *p = pages_[frame_id];
  page_table_[page_id] = frame_id;
  p->IncPinCount();
  p->SetPageId(page_id);
  p->SetDirty(true);
  return frame_id;
}

frame_id_t BMgr::SelectVictim() {
  frame_id_t frame_id = -1;
  if (!free_list_.empty()) {
    frame_id = free_list_.front();
    free_list_.pop_front();
    return frame_id;
  }
  frame_id_t victim_fid = replacer_->Victim();
  if (victim_fid == -1)
    return -1;
  Page *p = pages_[victim_fid];
  assert(p->GetPinCount() == 0);
  if (p->IsDirty()) {
    disk_manager_->WritePage(p);
    num_io_++;
    p->SetDirty(false);
  }
  page_table_.erase(p->GetPageId());
  return victim_fid;
}

int BMgr::GetIONum() { return num_io_; }

int BMgr::GetHitNum() { return num_hits_; }

frame_id_t BMgr::UnfixPage(int page_id) {
  auto iter = page_table_.find(page_id);
  frame_id_t frame_id = -1;
  if (iter != page_table_.end()) {
    frame_id = iter->second;
    Page *p = pages_[frame_id];
    assert(p->GetPinCount() > 0);
    p->DecPinCount();
    if (p->GetPinCount() == 0) {
      replacer_->Insert(frame_id);
      /* replacer_->Print(); */
    }
  }
  return frame_id;
}

void BMgr::SetDirty(int frame_id) { pages_[frame_id]->SetDirty(true); }

void BMgr::UnsetDirty(int frame_id) { pages_[frame_id]->SetDirty(false); }

void BMgr::PrintFrame(int frame_id) {
  printf("frame %d:\n", frame_id);
  Page *p = pages_[frame_id];
  printf("page id: %d\n", p->GetPageId());
  printf("dirty: %d\n", p->IsDirty());
  printf("pin count: %d\n", p->GetPinCount());
  printf("\n");
}

void BMgr::PrintPageTable() {
  int i = 0;
  printf("page table:\n");
  for (const auto &iter : page_table_) {
    printf("%d -> %d  ", iter.first, iter.second);
    if (++i % 3 == 0)
      printf("\n");
  }
  printf("\n");
}

void BMgr::PrintReplacer() { replacer_->Print(); }
