cmake_minimum_required(VERSION 2.6)

project(pipeline_examples)

#### Check --------------------------------

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
#if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
#else()
#  message(SEND_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
#endif()

#### Configure ----------------------------


file(GLOB APP_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp")

foreach(FILENAME ${APP_SOURCES})
  string(REPLACE ".cpp" "" EXEC_NAME ${FILENAME})

  add_executable(${EXEC_NAME} ${FILENAME})
endforeach(FILENAME ${APP_SOURCES})
