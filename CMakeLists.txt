cmake_minimum_required(VERSION 2.6)

project(pipeline)

#### Check --------------------------------

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
else()
 message(SEND_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

#### Configure ----------------------------

set(EXEC_TESTS "tests")

set(SOURCES_TESTS
  tests.cpp
  )

include_directories("include/")

add_subdirectory("examples/")

add_executable(${EXEC_TESTS} ${SOURCES_TESTS})

target_link_libraries(${EXEC_TESTS}
  ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

enable_testing()
add_test(${EXEC_TESTS} ${EXEC_TESTS})
