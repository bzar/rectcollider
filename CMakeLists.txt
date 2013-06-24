CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(rectcollider)

add_subdirectory(lib)

include_directories(
  ${kazmath_SOURCE_DIR}/src
  ${glhck_SOURCE_DIR}/include
  ${GLFW_SOURCE_DIR}/include
  ${engineworks_SOURCE_DIR}/include
)

file(GLOB SOURCES src/*.cpp src/**/*.cpp )
add_definitions(-DGLHCK_KAZMATH_FLOAT -DUSE_SINGLE_PRECISION)
list(APPEND CMAKE_CXX_FLAGS -std=c++11)
add_executable(rectcollider ${SOURCES})

target_link_libraries(rectcollider engineworks glfw glhck ${GLFW_LIBRARIES})
