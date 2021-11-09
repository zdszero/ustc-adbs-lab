#include "DataStorageManager.h"

#include <iostream>

int DSMgr::OpenFile(std::string filename) {
  currFile = fopen(filename.c_str(), "w");
  return currFile != nullptr;
}

int DSMgr::CloseFile() {
  return fclose(currFile);
}

FILE *DSMgr::GetFile() {
  return currFile;
}

bFrame DSMgr::ReadPage(int page_id) {
  bFrame buf;
  Seek(page_id * FRAMESIZE, SEEK_SET);
  fread(buf.field, 1, FRAMESIZE, currFile);
  return buf;
}

int DSMgr::Seek(int offset, int pos) {
  return fseek(currFile, (long) offset, pos);
}
