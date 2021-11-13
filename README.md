## build

```
mkdir build
cd build
cmake ..
make
```

## directory structure

```

data
├── adbs-lab.pdf                # lab specification
└── data-5w-50w-zipf.txt        # data file
include
├── BufferPoolManager.h         # buffer manager
├── ClockReplacer.h             # clock relacer
├── cmdline.h                   # argument parser
├── Common.h
├── DataStorageManager.h        # data storeage manager
├── LRUReplacer.h               # lru replacer
├── Page.h                      # page class
└── Replacer.h                  # base class
src
├── BufferPoolManager.cc
├── ClockReplacer.cc
├── DataStorageManager.cc
└── LRUReplacer.cc
test
├── buffer_pool_manager_test.cc
├── clock_replacer_test.cc
├── data_storage_manager_test.cc
├── lru_replacer_test.cc
├── main.cc                     # test (single thread)
└── main_multi_thread.cc        # test (multi thread)
```

## running test

* single thread

running `./main --help` to display help message.

```sh
# use lru algorithm to run data file (in build directory)
./main -l ../data/data-5w-50w-zipf.txt
# use clock algorithm to run data file
./main -c ../data/data-5w-50w-zipf.txt
```

* multiple threads

running `./main_multi_thread --help` to display help messge

```sh
# use lru algorihtm and running 10 threads
./main_multi_thread -l -t 10 ../data/data-5w-50w-zipf.txt
```
