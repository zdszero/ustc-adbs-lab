#pragma once

#include "Common.h"

#define PAGE_SIZE 4096

class Page {
public:
  explicit Page() = default;
  explicit Page(page_id_t page_id): page_id_(page_id) {}
  virtual ~Page() = default;

  page_id_t GetPageId() { return page_id_; }
  void SetPageId(page_id_t page_id) { page_id_ = page_id; }
  char *GetData() { return data_; }
  bool IsDirty() { return is_dirty_; }
  void SetDirty(bool is_dirty) { is_dirty_ = is_dirty; }
  int GetPinCount() { return pin_count_; }
  void IncPinCount() { pin_count_++; }
  void DecPinCount() { pin_count_--; }

private:
  bool is_dirty_;
  char data_[PAGE_SIZE];
  page_id_t page_id_ = -1;
  int pin_count_;
};
