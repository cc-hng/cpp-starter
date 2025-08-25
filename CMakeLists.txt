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

CPMAddPackage(
  NAME cc
  GIT_TAG v0.1.3
  GITHUB_REPOSITORY cc-hng/cc
  OPTIONS
    "CC_ENABLE_IO_URING OFF"
    "CC_WITH_SQLITE3 ON"
    "CC_ENABLE_COROUTINE ON"
)

CPMAddPackage(
  GITHUB_REPOSITORY gabime/spdlog
  GIT_TAG v1.15.3
  OPTIONS
    "SPDLOG_INSTALL ON"
    "SPDLOG_FMT_EXTERNAL ON"
)

#####################################
# target
#####################################
file(GLOB_RECURSE cxx_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/**.h ${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp)

# precompile header
set(pch_header "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")
list(REMOVE_ITEM cxx_sources ${pch_header})

add_executable(${PROJECT_NAME} ${cxx_sources})
target_link_libraries(${PROJECT_NAME} PRIVATE strings cc spdlog)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${pch_header})
target_link_time_optimization(${PROJECT_NAME} REQUIRED)
