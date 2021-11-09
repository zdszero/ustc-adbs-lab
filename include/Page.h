#pragma once

#include "Common.h"

#define PAGE_SIZE 4096

class Page {
public:
  explicit Page();
  virtual ~Page();

  page_id_t GetPageId() { return page_id_; }
  void SetPageId(page_id_t page_id) { page_id_ = page_id; }
  char *GetData() { return data_; }
  bool IsDirty() { return is_dirty_; }
  void SetDirty(bool is_dirty) { is_dirty_ = is_dirty; }
  int GetPinCount() { return pin_count_; }
  void SetPinCount(int pin_count) { pin_count_ = pin_count; }

private:
  bool is_dirty_;
  char data_[PAGE_SIZE];
  page_id_t page_id_;
  int pin_count_;
};
