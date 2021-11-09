#include "DataStorageManager.h"

#include <iostream>

DSMgr::DSMgr() {
  num_pages_ = 0;
}

DSMgr::DSMgr(std::string filename) {
  num_pages_ = 0;
  OpenFile(filename);
}

DSMgr::~DSMgr() {
  CloseFile();
}

int DSMgr::OpenFile(std::string filename) {
  curr_file_ = fopen(filename.c_str(), "a+");
  fseek(curr_file_, 0L, SEEK_END);
  long sz = ftell(curr_file_);
  num_pages_ = sz / PAGE_SIZE;
  return curr_file_ != nullptr;
}

int DSMgr::CloseFile() {
  return fclose(curr_file_);
}

FILE *DSMgr::GetFile() {
  return curr_file_;
}

page_id_t DSMgr::NewPage() {
  int ret = num_pages_;
  IncNumPages();
  return ret;
}

void DSMgr::ReadPage(int page_id, char *data) {
  if (Seek(page_id * PAGE_SIZE, SEEK_SET != 0)) {
    fprintf(stderr, "Error: cannot find page: %d\n", page_id);
    exit(1);
  }
  fread(data, 1, PAGE_SIZE, curr_file_);
}

void DSMgr::WritePage(Page *page) {
  page_id_t page_id = page->GetPageId();
  if (Seek(page_id * PAGE_SIZE, SEEK_SET) != 0) {
    fprintf(stderr, "Error: cannot find page: %d\n", page_id);
    exit(1);
  }
  fwrite(page->GetData(), 1, PAGE_SIZE, curr_file_);
}

int DSMgr::Seek(int offset, int pos) {
  return fseek(curr_file_, (long) offset, pos);
}

void DSMgr::IncNumPages() {
  num_pages_++;
}

int DSMgr::GetNumPages() {
  return num_pages_;
}
