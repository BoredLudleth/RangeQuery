cmake_minimum_required(VERSION 3.0)

project(cache)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

set(CMAKE_CXX_STANDARD 11)
option(BUILD_TESTING "Build tests" ON)


include_directories(${INCLUDE_DIR})

add_executable (tree src/main.cpp)

add_executable (set src/set.cpp)

add_executable(tree_gen src/tree_gen.cpp)

if(BUILD_TESTING)
  include(CTest)
  enable_testing()
  add_subdirectory(test/)
endif()