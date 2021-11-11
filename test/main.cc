#include <cstring>
#include <iostream>

#include "BufferPoolManager.h"

void PrintHelp(char *program_name) {
  printf("ustc adbs 2021 lab\n"
         "Usage:\n"
         "\t%s [options] [filename]\n"
         "Options\n"
         "\t-l, --lru\t\tuse lru replacer\n"
         "\t-c, --clock\t\tuse clock replacer\n",
         program_name);
}

int ParseOpt(char *opt) {
  size_t len = strlen(opt);
  if (len == 2 && opt[0] == '_') {
    return opt[1];
  }
  if (len == 5 && strcmp(opt + 2, "lru") == 0)
    return 'l';
  if (len == 7 && strcmp(opt + 2, "clock") == 0)
    return 'c';
  return -1;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    PrintHelp(argv[0]);
    exit(1);
  }
  int opt = ParseOpt(argv[1]);
  int policy;
  char *filename;
  if (opt == 'l') {
    policy = ReplacePolicy::LruReplacer;
  } else if (opt == 'c') {
    policy = ReplacePolicy::ClockReplacer;
  } else {
    fprintf(stderr, "Unknown option: %s.\n", argv[1]);
    exit(1);
  }
  filename = argv[2];
  FILE *db_file = fopen("test.dbf", "r");
  /* create db with certain size */
  if (db_file == NULL) {
    fclose(db_file);
    db_file = fopen("test.dbf", "w");
    void *buffer = malloc(50000 * PAGE_SIZE);
    fwrite(buffer, PAGE_SIZE, 50000, db_file);
    free(buffer);
  }
  fclose(db_file);
  /* create buffer pool manager */
  BMgr *bmgr = new BMgr("test.db");
  /* read data file */ 
  FILE *data_file = fopen(filename, "r");
  if (data_file == NULL) {
    fprintf(stderr, "Error: file %s doesn't exist\n", filename);
    exit(1);
  }
  int act, page_id;
  while (fscanf(data_file, "%d,%d", &act, &page_id)) {
  }
  fclose(data_file);
  return 0;
}
