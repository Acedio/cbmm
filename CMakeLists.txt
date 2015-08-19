cmake_minimum_required (VERSION 2.6)
project (cbmm_sim)
set (cbmm_sim_VERSION_MAJOR 0)
set (cbmm_sim_VERSION_MINOR 1)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/libs/tmxparser/cmake)

add_subdirectory (libs/tinyxml2)
set (USE_MINIZ ON CACHE BOOL "special")
set (TINYXML2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyxml2 CACHE STRING "lala")
set (TINYXML2_LIBRARIES tinyxml2 CACHE STRING "haha")

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
#add_dependencies (cbmm_sim tmxparser)
set_property (TARGET cbmm_sim PROPERTY CXX_STANDARD 11)