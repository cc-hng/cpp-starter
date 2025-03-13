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
CPMAddPackage(
  NAME cc
  GIT_TAG v0.1.1
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
set(stdafx_header "${CMAKE_CURRENT_SOURCE_DIR}/src/stdafx.h")
file(GLOB_RECURSE headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/**.h")
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM headers ${stdafx_header})

add_executable(${PROJECT_NAME} ${headers} ${sources})
target_link_libraries(${PROJECT_NAME} PRIVATE cc)
target_precompile_headers(${PROJECT_NAME} PRIVATE ${stdafx_header})
