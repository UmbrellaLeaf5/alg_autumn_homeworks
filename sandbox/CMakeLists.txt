cmake_minimum_required(VERSION 3.20)

project(sandbox LANGUAGES CXX)

file(GLOB_RECURSE SUBFOLDERS LIST_DIRECTORIES true ".")

foreach(FOLDER ${SUBFOLDERS})
  if(IS_DIRECTORY ${FOLDER})
    if(NOT ${FOLDER} MATCHES ".*src.*")
      add_subdirectory(${FOLDER})
    endif()
  endif()
endforeach()
