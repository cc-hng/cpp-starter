cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(
  demo
  VERSION "0.1.0"
  LANGUAGES CXX C)

include(cmake/cpm.cmake)
CPMAddPackage("gh:cc-hng/cmake-scripts#v0.1.0")
include(${cmake-scripts_SOURCE_DIR}/ProjectGuard.cmake)
include(${cmake-scripts_SOURCE_DIR}/core.cmake)

#####################################
# compile & link options
#####################################
cxx_20()

#####################################
# third-party packages
#####################################
CPMAddPackage("gh:cc-hng/strings#v0.1.0")
# CPMAddPackage(
#   NAME cc
#   GIT_TAG v0.1.1
#   GITHUB_REPOSITORY cc-hng/cc
#   OPTIONS
#     "CC_ENABLE_IO_URING OFF"
#     "CC_WITH_SQLITE3 ON"
#     "CC_ENABLE_COROUTINE ON"
# )

#####################################
# target
#####################################
file(GLOB_RECURSE cxx_sources ${CMAKE_CURRENT_SOURCE_DIR}/include/**.h ${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp)

# stdafx
set(stdafx_header "${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.h")
list(REMOVE_ITEM cxx_sources ${stdafx_header})

add_executable(${PROJECT_NAME} ${cxx_sources})
target_link_libraries(${PROJECT_NAME} PRIVATE strings)
target_link_time_optimization(${PROJECT_NAME} REQUIRED)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${stdafx_header})
