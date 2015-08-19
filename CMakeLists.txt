cmake_minimum_required (VERSION 2.6)
project (cbmm_sim)
set (cbmm_sim_VERSION_MAJOR 0)
set (cbmm_sim_VERSION_MINOR 1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/libs/tmxparser/cmake)

set (BUILD_SHARED_LIBS OFF CACHE BOOL "Build a shared version of tinyxml2 (rather than static).")
add_subdirectory (libs/tinyxml2)

set (USE_MINIZ ON CACHE BOOL "Use the repo copy of miniz rather than zlib.")
set (TINYXML2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyxml2 CACHE STRING "The location of the tinyxml2 include directory.")
set (TINYXML2_LIBRARIES tinyxml2 CACHE STRING "The location of the tinyxml2.{dll,so,etc} file.")
add_subdirectory (libs/tmxparser)
add_dependencies (tmxparser_static tinyxml2)

find_package (SDL2 REQUIRED)
if (SDL2_FOUND)
  include_directories (${SDL2_INCLUDE_DIR})
  link_libraries (${SDL2_LIBRARY})
endif ()

find_package (SDL2_IMAGE REQUIRED)
if (SDL2_IMAGE_FOUND)
  include_directories (${SDL2_IMAGE_INCLUDE_DIR})
  link_libraries (${SDL2_IMAGE_LIBRARY})
endif ()

find_package (OPENGL REQUIRED)
if (OPENGL_FOUND)
  include_directories (${OPENGL_INCLUDE_DIR})
  link_libraries (${OPENGL_LIBRARIES})
endif ()

find_package (GLEW REQUIRED)
if (GLEW_FOUND)
  include_directories (${GLEW_INCLUDE_DIRS})
  link_libraries (${GLEW_LIBRARIES})
endif ()
MESSAGE( STATUS "GLEW_LIBRARIES: " ${GLEW_LIBRARIES} )

file(GLOB cbmm_sim_SRC
  "*.h"
  "*.cc"
)

add_executable (cbmm_sim ${cbmm_sim_SRC})
target_link_libraries (cbmm_sim tmxparser_static tinyxml2)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/libs/tmxparser/src ${CMAKE_CURRENT_BINARY_DIR}/libs)
set_property (TARGET cbmm_sim PROPERTY CXX_STANDARD 11)

add_custom_target (copy_resources)
add_custom_command(TARGET copy_resources PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E
                   copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:cbmm_sim>/resources)
