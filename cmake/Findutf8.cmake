# Include this file to your project's CMakeList.txt to initialize
# UTF8_INCLUDE_DIR, UTF8_LIBRARIES variables and add UTF8 subproject
#
# It is clear that this file does not try to search for the location 
# of the library, but knows it exactly because it is located in the 
# cmake subdirectory. We implemented this approach to connect all our 
# libraries in a uniform way. Both from the root files of the 
# repositories, and their use by external projects. In each case, it 
# is enough to add the path to the directory and start the search
#
# list(APPEND CMAKE_MODULE_PATH <location_of_lib_in_your_project>/cmake) 
# find_package(utf8 MODULE) 

get_filename_component(UTF8_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE) 

set(UTF8_INCLUDE_DIR ${UTF8_ROOT}/lib)
set(UTF8_LIBRARIES utf8)

set(UTF8_STATIC_WINDOWS_RUNTIME OFF CACHE BOOL "" FORCE)
add_subdirectory(${UTF8_ROOT}/lib)
