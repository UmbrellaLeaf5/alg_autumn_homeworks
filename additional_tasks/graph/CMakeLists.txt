cmake_minimum_required(VERSION 3.10)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE source_list "src/*.cpp" "src/*.hpp")
file(GLOB_RECURSE lib_source_list "../../lib/src/*.cpp" "../../lib/src/*.hpp")
file(GLOB_RECURSE main_source_list "src/main.cpp")
file(GLOB_RECURSE test_source_list "src/*.cpp")
file(GLOB_RECURSE test_list "src/*test.cpp")

list(REMOVE_ITEM test_source_list ${main_source_list})
list(REMOVE_ITEM source_list ${test_list})

include_directories(${PROJECT_NAME} PUBLIC src)

add_executable(${PROJECT_NAME} ${source_list} ${lib_source_list})

# Locate GTest
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(${PROJECT_NAME}_tests ${test_source_list})
target_link_libraries(
  ${PROJECT_NAME}_tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)
