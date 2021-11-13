#include <cstring>
#include <iostream>
#include <time.h>

#include "BufferPoolManager.h"

void PrintHelp(char *program_name) {
  printf("ustc adbs 2021 lab\n"
         "Usage:\n"
         "\t%s [options] [filename]\n"
         "Options\n"
         "\t-h, --help\t\tdisplay help\n"
         "\t-l, --lru\t\tuse lru replacer\n"
         "\t-c, --clock\t\tuse clock replacer\n",
         program_name);
}

int ParseOpt(char *opt) {
  size_t len = strlen(opt);
  if (len == 2 && opt[0] == '-') {
    return opt[1];
  }
  if (len == 5 && strcmp(opt + 2, "lru") == 0)
    return 'l';
  if (len == 6 && strcmp(opt + 2, "help") == 0)
    return 'h';
  if (len == 7 && strcmp(opt + 2, "clock") == 0)
    return 'c';
  return -1;
}

int main(int argc, char *argv[]) {
  clock_t start_time = clock();
  if (argc != 3) {
    PrintHelp(argv[0]);
    exit(1);
  }
  int opt = ParseOpt(argv[1]);
  int policy = ReplacePolicy::Invalid;
  char *filename;
  if (opt == 'l') {
    policy = ReplacePolicy::Lru;
    printf("using lru replacement policy:\n");
  } else if (opt == 'c') {
    printf("using clock replacement policy:\n");
    policy = ReplacePolicy::Clock;
  } else if (opt == 'h') {
    PrintHelp(argv[0]);
    return 0;
  } else {
    fprintf(stderr, "Unknown option: %s.\n", argv[1]);
    exit(1);
  }
  filename = argv[2];
  FILE *db_file = fopen("test.dbf", "r");
  /* create db with certain size */
  if (db_file == NULL) {
    db_file = fopen("test.dbf", "w");
    void *buffer = malloc(50000 * PAGE_SIZE);
    fwrite(buffer, PAGE_SIZE, 50000, db_file);
    free(buffer);
  }
  fclose(db_file);
  /* create buffer pool manager */
  BMgr *bmgr = new BMgr("test.db", policy);
  /* read data file */
  FILE *data_file = fopen(filename, "r");
  if (data_file == NULL) {
    fprintf(stderr, "Error: file %s doesn't exist\n", filename);
    exit(1);
  }
  int is_dirty, page_id;
  while (fscanf(data_file, "%d,%d", &is_dirty, &page_id) != EOF) {
    bmgr->FixPage(page_id, is_dirty);
    bmgr->UnfixPage(page_id);
  }
  printf("hit number: %d\n", bmgr->GetHitNum());
  printf("hit rate: %.2lf%%\n", bmgr->GetHitNum() * 100.0 / 500000);
  printf("io number: %d\n", bmgr->GetIONum());
  printf("time taken: %.2fs\n",
         (double)(clock() - start_time) / CLOCKS_PER_SEC);
  fclose(data_file);
  return 0;
}
