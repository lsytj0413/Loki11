CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT (test_loki11)

set (CMAKE_CXX_FLAGS "-std=c++14")

SET (TEST_SRC
  main.cpp
  )

include_directories(../)

# locate gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

SET( CMAKE_BUILD_TYPE Debug )

# boost
# set(BOOST_ROOT "~/workspace/3rd/boost_1_61_0")
# include_directories(${Boost_INCLUDE_DIRECTORY})

add_executable(main ${TEST_SRC})
target_link_libraries(main ${GTEST_LIBRARIES} pthread)
