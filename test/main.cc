#include <cstring>
#include <iostream>
#include <time.h>

#include "BufferPoolManager.h"
#include "cmdline.h"

int main(int argc, char *argv[]) {
  clock_t start_time = clock();
  cmdline::parser cmd;
  cmd.add("lru", 'l', "use lru replacer");
  cmd.add("clock", 'c', "use clock replacer");
  cmd.parse_check(argc, argv);
  int policy = ReplacePolicy::Lru;
  if (cmd.exist("lru")) {
    policy = ReplacePolicy::Lru;
  } else {
    policy = ReplacePolicy::Clock;
  }
  char *filename = const_cast<char *>(cmd.rest()[0].c_str());
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
  BMgr *bmgr = new BMgr("test.dbf", policy);
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
