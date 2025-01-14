cmake_minimum_required(VERSION 3.20)

project(homeworks LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++23 -O2")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sandbox)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/additional_tasks)

file(GLOB_RECURSE SUBFOLDERS LIST_DIRECTORIES true ".")

foreach(FOLDER ${SUBFOLDERS})
  if(IS_DIRECTORY ${FOLDER})
    if(${FOLDER} MATCHES "task_0[0-9]$" AND NOT ${FOLDER} MATCHES "build")
      add_subdirectory(${FOLDER})
    endif()
  endif()
endforeach()
