CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET (CMAKE_CXX_FLAGS "-std=c++14")

SET (LOKI11_SRC
  static_check.hpp
  LokiTypeInfo.hpp
  TypeManip.hpp
  EmptyType.hpp
  NullType.hpp
  TypeTraits.hpp
  Typelist.hpp
  )

SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb ")
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
SET( CMAKE_BUILD_TYPE Debug )


# include_directories(../../3rd/liter)
# include_directories(../../3rd/spdlog/include)

ADD_EXECUTABLE(LOKI11 ${LOKI11_SRC})
set_target_properties(LOKI11 PROPERTIES LINKER_LANGUAGE CXX)
# target_link_libraries (LOKI11 pthread)
