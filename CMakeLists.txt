cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(adbs-lab)

file(GLOB SOURCES src/*.cc)

add_executable(main ${SOURCES} test/main.cc)
add_executable(main_multi_thread ${SOURCES} test/main_multi_thread.cc)
target_link_libraries(main_multi_thread PUBLIC pthread)

include_directories(include)

# using google test

enable_testing()
find_package(GTest)

if(GTest_Found)

  file(GLOB TESTS test/*.cc)

  add_executable(
    lru_replacer_test EXCLUDE_FROM_ALL test/lru_replacer_test.cc
    src/LRUReplacer.cc
  )
  target_link_libraries(lru_replacer_test PUBLIC GTest::gtest_main)

  add_executable(
    data_storage_manager_test EXCLUDE_FROM_ALL
    test/data_storage_manager_test.cc src/DataStorageManager.cc
  )
  target_link_libraries(data_storage_manager_test PUBLIC GTest::gtest_main)

  add_executable(
    buffer_pool_manager_test EXCLUDE_FROM_ALL
    test/buffer_pool_manager_test.cc ${SOURCES}
  )
  target_link_libraries(buffer_pool_manager_test PUBLIC GTest::gtest_main)

  add_executable(
    clock_replacer_test EXCLUDE_FROM_ALL test/clock_replacer_test.cc
    src/ClockReplacer.cc
  )
  target_link_libraries(clock_replacer_test PUBLIC GTest::gtest_main)

  include(GoogleTest)

endif()
