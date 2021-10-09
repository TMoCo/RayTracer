# set search directories
set(GLFW3_HEADER_SEARCH_DIR
"C:/Libraries/glfw-3.3.4.bin.WIN64/include")
set(GLFW3_LIB_SEARCH_DIR
"C:/Libraries/glfw-3.3.4.bin.WIN64/lib-vc2019")

# search header
find_path(GLFW3_INCLUDE_DIR "GLFW/glfw3.h" PATHS ${GLFW3_HEADER_SEARCH_DIR})

# search lib
find_library(GLFW3_LIBRARY NAMES GLFW3 GLFW PATHS ${GLFW3_LIB_SEARCH_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLFW3 DEFAULT_MSG GLFW3_LIBRARY GLFW3_INCLUDE_DIR)
