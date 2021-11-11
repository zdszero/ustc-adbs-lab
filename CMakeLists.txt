cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(adbs-lab)

FILE(GLOB SOURCES src/*.cc)

add_executable(
  main
  ${SOURCES}
  test/main.cc
)

include_directories(include)

# using google test

enable_testing()
find_package(GTest)

FILE(GLOB TESTS test/*.cc)

# foreach(test_source ${TESTS})
#   get_filename_component(test_source_filename ${test_source} NAME)
#   string(REPLACE ".cc" "" test_name ${test_source_filename})
#   add_executable(${test_name} EXCLUDE_FROM_ALL ${test_source})
#   target_include_directories(${test_name} PUBLIC include)
#   gtest_add_tests(TARGET ${test_name})
# endforeach(test_source ${TESTS})

add_executable(lru_replacer_test EXCLUDE_FROM_ALL test/lru_replacer_test.cc src/LRUReplacer.cc)
target_link_libraries(lru_replacer_test PUBLIC GTest::gtest_main)

add_executable(data_storage_manager_test EXCLUDE_FROM_ALL test/data_storage_manager_test.cc src/DataStorageManager.cc)
target_link_libraries(data_storage_manager_test PUBLIC GTest::gtest_main)

add_executable(buffer_pool_manager_test EXCLUDE_FROM_ALL test/buffer_pool_manager_test.cc ${SOURCES})
target_link_libraries(buffer_pool_manager_test PUBLIC GTest::gtest_main)

include(GoogleTest)
