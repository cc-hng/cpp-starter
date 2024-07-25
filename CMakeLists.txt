include(cmake/ProjectGuard.cmake)
cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(
  demo
  VERSION "0.1.0"
  LANGUAGES CXX C)

#####################################
# compile & link options
#####################################
include(cmake/common.cmake)

#####################################
# third-party packages
#####################################
set(MY_LIBS)
include(FetchContent)

include(cmake/FetchCC.cmake)

FetchContent_MakeAvailable(${MY_LIBS})

#####################################
# target
#####################################

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE cc)
