CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(rectcollider)

find_package(Boost REQUIRED system filesystem)

add_subdirectory(lib)

include_directories(
  ${kazmath_SOURCE_DIR}/src
  ${qmlon_SOURCE_DIR}/include
  ${glhck_SOURCE_DIR}/include
  ${GLFW_SOURCE_DIR}/include
  ${engineworks_SOURCE_DIR}/include
  ${tmxparser_SOURCE_DIR}/include
  ${Boost_INCLUDE_DIRS}
)

file(GLOB SOURCES src/*.cpp src/**/*.cpp )
add_definitions(-DGLHCK_KAZMATH_FLOAT -DUSE_SINGLE_PRECISION)
list(APPEND CMAKE_CXX_FLAGS -std=c++11)
add_executable(rectcollider ${SOURCES})

target_link_libraries(rectcollider engineworks tmxparser glfw glhck ${GLFW_LIBRARIES} ${Boost_LIBRARIES})

file(COPY levels DESTINATION .)
