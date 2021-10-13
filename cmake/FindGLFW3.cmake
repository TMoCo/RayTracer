# set search directories
set(GLFW3_HEADER_SEARCH_DIR
"C:/Libraries/glfw-3.3.4.bin.WIN64/include")
set(GLFW3_LIB_SEARCH_DIR
"C:/Libraries/glfw-3.3.4.bin.WIN64/lib-vc2019")

# search header
find_path(GLFW3_INCLUDE_DIR "GLFW/glfw3.h" PATHS ${GLFW3_HEADER_SEARCH_DIR})

# search lib
find_library(GLFW3_LIB NAMES GLFW3 PATHS ${GLFW3_LIB_SEARCH_DIR})

if (GLFW3_LIBRARY-NOTFOUND)
  message(FATAL_ERROR "Could not find ${GLFW3_LIBRARY} in ${GLFW3_LIB_SEARCH_DIR}")
endif()