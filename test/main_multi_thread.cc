#include <cstring>
#include <iostream>
#include <thread>
#include <time.h>
#include <vector>

#include "BufferPoolManager.h"
#include "cmdline.h"

static int ThreadNumber = 10;

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

template <typename... Args>
void LaunchParallelTest(int num_threads, Args &&...args) {
  std::vector<std::thread> thread_group;
  for (int i = 0; i < num_threads; i++) {
    thread_group.push_back(std::thread(args...));
  }
  for (int i = 0; i < num_threads; i++) {
    thread_group[i].join();
  }
}

void ExecuteFile(BMgr *bmgr, char *filename) {
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
  fclose(data_file);
}

int main(int argc, char *argv[]) {
  clock_t start_time = clock();
  cmdline::parser cmd;
  cmd.add("lru", 'l', "use lru replacer");
  cmd.add("clock", 'c', "use clock replacer");
  cmd.add<int>("thread", 't', "thread number", true, 10,
               cmdline::range(1, 100));
  cmd.parse_check(argc, argv);
  int policy = ReplacePolicy::Lru;
  if (cmd.exist("lru")) {
    policy = ReplacePolicy::Lru;
  } else {
    policy = ReplacePolicy::Clock;
  }
  char *filename = const_cast<char *>(cmd.rest()[0].c_str());
  ThreadNumber = cmd.get<int>("thread");
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
  /* run multiple threads */
  LaunchParallelTest(ThreadNumber, ExecuteFile, bmgr, filename);
  printf("hit number: %d\n", bmgr->GetHitNum());
  printf("hit rate: %.2lf%%\n",
         bmgr->GetHitNum() * 100.0 / (500000 * ThreadNumber));
  printf("io number: %d\n", bmgr->GetIONum());
  printf("time taken: %.2fs\n",
         (double)(clock() - start_time) / CLOCKS_PER_SEC);
  return 0;
}
