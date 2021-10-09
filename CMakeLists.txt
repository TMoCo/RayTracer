cmake_minimum_required (VERSION 3.2 FATAL_ERROR)

project(RAYTRACER
  VERSION 1.0
  LANGUAGES CXX C)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# source
file(GLOB_RECURSE SOURCE_FILES
  ${CMAKE_SOURCE_DIR}/src/*.c
  ${CMAKE_SOURCE_DIR}/src/*.cpp)

# include
file(GLOB_RECURSE HEADER_FILES
  ${CMAKE_SOURCE_DIR}/include/*.h
  ${CMAKE_SOURCE_DIR}/include/*.hpp)

# config
configure_file(include/root_dir.h.in include/root_dir.h)
include_directories(${CMAKE_BINARY_DIR}/include)

# create executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# external libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# openGl
find_package(OpenGL REQUIRED)

# glfw 
find_package(GLFW3 REQUIRED)

# header only GLAD library
set(GLAD_SRC_SEARCH_DIR
  "C:/Libraries/glad/src")

set(GLAD_SRC_FILES
  "glad.c")

find_path(GLAD_SRC_DIR NAMES ${GLAD_SRC_FILES} PATHS ${GLAD_SRC_SEARCH_DIR})
if(GLAD_SRC_DIR-NOTFOUND)
  message(FATAL_ERROR "Could not find ${GLAD_SRC_FILES} in ${GLAD_SRC_SEARCH_DIR}")
endif()

set(GLAD_INCLUDE_SEARCH_DIR
  "C:/Libraries/glad/include")

set(GLAD_HEADER_FILES
  "glad/glad.h"
  "KHR/khrplatform.h")

find_path(GLAD_INCLUDE_DIR NAMES ${GLAD_HEADER_FILES} PATHS ${GLAD_INCLUDE_SEARCH_DIR})
if(GLAD_INCLUDE_DIR-NOTFOUND)
  message(FATAL_ERROR "Could not find ${GLAD_HEADER_FILES} in ${GLAD_INCLUDE_SEARCH_DIR}")
endif()

add_library(GLAD "${GLAD_SRC_DIR}/${GLAD_SRC_FILES}")

# libraris
set(LIBS GLFW3 OPENGL32 GLAD)

# include directories
include_directories(
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/include"
  "${GLAD_INCLUDE_DIR}"
)

# link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})

# visual studio project filters
if(MSVC_IDE)
  macro(GROUP_SOURCES CUR_DIR)
    file(GLOB CHILDREN RELATIVE ${PROJECT_SOURCE_DIR}/${CUR_DIR} ${PROJECT_SOURCE_DIR}/${CUR_DIR}/*)
      foreach(CHILD ${CHILDREN})
        if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${CUR_DIR}/${CHILD})
          GROUP_SOURCES(${CUR_DIR}/${CHILD})
        else()
          string(REPLACE "/" "\\" GROUP_NAME ${CUR_DIR})
          string(REPLACE "src" "Sources" GROUP_NAME ${GROUP_NAME})
          source_group(${GROUP_NAME} FILES ${PROJECT_SOURCE_DIR}/${CUR_DIR}/${CHILD})
        endif()
      endforeach()
  endmacro()
  # run macro
  GROUP_SOURCES(SRC)
endif()