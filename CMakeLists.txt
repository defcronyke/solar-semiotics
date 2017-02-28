cmake_minimum_required(VERSION 3.7)
project(SolarSemiotics)
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${SolarSemiotics_SOURCE_DIR}/cmake-modules")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(SolarSemiotics
  include
  ${SDL2_INCLUDE_DIR}
  ${SDL2_IMAGE_INCLUDE_DIR}
  ${OPENGL_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIRS}
)
add_library(SolarSemiotics SHARED
  src/SolarSemiotics.cpp
  src/Ship.cpp
  src/Tile.cpp
  src/Window.cpp
)
add_executable(solar-semiotics src/main.cpp)
target_link_libraries(solar-semiotics
  SolarSemiotics
  ${SDL2_LIBRARY}
  ${SDL2_IMAGE_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
)
