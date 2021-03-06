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

# config to get root directory
if(MSVC_IDE)
  string(REPLACE "/" "\\\\" SOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}\\\\")
else()
  set(SOURCE_DIRECTORY "${CMAKE_SOURCE_DIR}/")
endif()

configure_file(${CMAKE_SOURCE_DIR}/build/config/root_dir.h.in ${CMAKE_SOURCE_DIR}/include/root_dir.h)
include_directories(${CMAKE_SOURCE_DIR}/include)

# create executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# external libraries
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# openGl
find_package(OpenGL REQUIRED)

# glfw 
# find_package(GLFW3 REQUIRED) ... can't get it to work, easier to manually add lib
include(FindGLFW3)

# GLAD library
set(GLAD_SRC_DIR "C:/Libraries/glad/src")
file(GLOB_RECURSE GLAD_SRC ${GLAD_SRC_DIR}/*.c)

set(GLAD_INCLUDE_DIR "C:/Libraries/glad/include")
file(GLOB_RECURSE GLAD_INCLUDE ${GLAD_INCLUDE_DIR}/*.h)

add_library(GLAD ${GLAD_SRC})

# ImGui
set(IMGUI_DIR "C:/Libraries/ImGui")
file(GLOB_RECURSE IMGUI_SRC ${IMGUI_DIR}/*.cpp)
file(GLOB_RECURSE IMGUI_INCLUDE ${IMGUI_DIR}/*.h)

add_library(ImGui ${IMGUI_SRC})

# THOMATH
set(THOMATH_DIR "C:/Libraries/thomath")
file(GLOB_RECURSE THOMATH_SRC ${THOMATH_DIR}/*.cpp)
file(GLOB_RECURSE THOMATH_INCLUDE ${THOMATH_DIR}/*.h)

add_library(THOMATH ${THOMATH_SRC})

# stb_image library
set(STB_IMAGE_INCLUDE_SEARCH_DIR
  "C:/Libraries/stb")

set(STB_IMAGE_HEADER_FILES
  "stb_image.h")

find_path(STB_IMAGE_INCLUDE_DIR 
  NAMES ${STB_IMAGE_HEADER_FILES} 
  PATHS ${STB_IMAGE_INCLUDE_SEARCH_DIR})

if(STB_IMAGE_INCLUDE_DIR-NOTFOUND)
  message(FATAL_ERROR "Could not find ${STB_IMAGE_HEADER_FILES} in ${STB_IMAGE_INCLUDE_SEARCH_DIR}")
endif()

#

# libraries
set(LIBS ${GLFW3_LIB} opengl32 GLAD ImGui THOMATH)

# include directories
include_directories(
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/include"
  "${GLAD_INCLUDE_DIR}"
  "${GLFW3_INCLUDE_DIR}"
  "${STB_IMAGE_INCLUDE_DIR}"
  "${IMGUI_DIR}"
  "${THOMATH_DIR}"
)

# link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})

if(MSVC_IDE)
  # visual studio project filters
  macro(GROUP_SOURCES CUR_DIR FILE_EXT)
    file(GLOB CHILDREN RELATIVE ${PROJECT_SOURCE_DIR}/${CUR_DIR} ${PROJECT_SOURCE_DIR}/${CUR_DIR}/*)
      foreach(CHILD ${CHILDREN})
        if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${CUR_DIR}/${CHILD})
          GROUP_SOURCES(${CUR_DIR}/${CHILD} FILE_EXT)
        else()
          string(REPLACE "/" "\\" GROUP_NAME ${CUR_DIR})
          source_group(${GROUP_NAME} FILES ${PROJECT_SOURCE_DIR}/${CUR_DIR}/${CHILD} REGULAR_EXPRESSION ".${FILE_EXT}$")
        endif()
      endforeach()
  endmacro()

  # run macro for src
  GROUP_SOURCES("src" "cpp")

  # run macro for headers
  GROUP_SOURCES("include" "h")
  
  #if (${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.16.0")
  #  message("-- Valid CMake version, making precompiled headers...")
  #  target_precompile_headers(${PROJECT_NAME} PRIVATE <math/thomath.h>)
  #endif()

  # set start up project
  if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.8.0")
    set(VS_STARTUP_PROJECT ${PROJECT_NAME})
  endif()
endif()