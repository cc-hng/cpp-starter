include(cmake/ProjectGuard.cmake)
cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(
  demo
  VERSION "0.1.0"
  LANGUAGES CXX C)

#####################################
# third-party packages
#####################################
include(cmake/cpm.cmake)
cpm_add_package(
  NAME cc
  GIT_TAG v0.1.0
  GITHUB_REPOSITORY cc-hng/cc
  OPTIONS
    "CC_ENABLE_IO_URING OFF"
    "CC_WITH_SQLITE3 ON"
    "CC_ENABLE_COROUTINE ON"
)

# compile & link options
include(${cc_SOURCE_DIR}/cmake/common.cmake)

#####################################
# target
#####################################
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE cc)
