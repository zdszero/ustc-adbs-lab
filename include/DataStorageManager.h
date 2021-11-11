#pragma once

#include <iostream>

#include "Common.h"
#include "Page.h"

class DSMgr {
public:
  explicit DSMgr();
  explicit DSMgr(std::string filename);
  ~DSMgr();
  int OpenFile(std::string filename);
  int CloseFile();
  page_id_t NewPage();
  void ReadPage(int page_id, char *data);
  void WritePage(Page *page);
  int Seek(int offset, int pos);
  FILE *GetFile();
  void IncNumPages();
  int GetNumPages();
  int GetWriteNum();
  void SetUse(int index, int use_bit);
  int GetUse(int index);

private:
  FILE *curr_file_;
  int num_pages_;
};
