cmake_minimum_required(VERSION 3.20)

get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_LIST "src/*.cpp" "src/*.hpp")
file(GLOB_RECURSE MAIN_SOURCE_LIST "src/main.cpp")
set(TEST_SOURCE_LIST ${SOURCE_LIST})
file(GLOB_RECURSE test_list "src/*test.cpp")

list(REMOVE_ITEM TEST_SOURCE_LIST ${MAIN_SOURCE_LIST})
list(REMOVE_ITEM SOURCE_LIST ${test_list})

include_directories(${PROJECT_NAME} PUBLIC src)

find_library(Utils ../)

add_executable(${PROJECT_NAME} ${SOURCE_LIST})
target_link_libraries(${PROJECT_NAME} PUBLIC Utils)

# Locate GTest
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Link runTests with what we want to test and the GTest and pthread library
add_executable(${PROJECT_NAME}_tests ${TEST_SOURCE_LIST})
target_link_libraries(
  ${PROJECT_NAME}_tests
  GTest::gtest_main
  Utils
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}_tests)
