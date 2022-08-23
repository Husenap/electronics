cmake_minimum_required(VERSION 3.23)

project("projects"
  LANGUAGES C CXX)

include("cmake/prevent_in_source_builds.cmake")
include("cmake/compiler_features.cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_subdirectory(bcm2835)
add_subdirectory(ir-sensor)