cmake_minimum_required (VERSION 2.8.3)
project (SEARCH)
#include include directory
include_directories(${PROJECT_SOURCE_DIR}/include)
#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# include GTK2
find_package(GTK2 REQUIRED gtk)
include_directories(${GTK2_INCLUDE_DIRS})
# Build usjAIL library
add_library(usjAIL src/Data.cpp 
			src/usjAIL.cpp
			src/usjAILSearches.cpp
			src/usjAILsrc.cpp
			src/usjMapProblem.cpp)
# Build mapper gui
add_executable(MapRoutePlanner src/MapRoutePlanner.cpp)
target_link_libraries(MapRoutePlanner usjAIL ${GTK2_LIBRARIES})
# Install
set(exec_prefix             "\${prefix}")
set(includedir              "\${prefix}/include/usjsearch")
set(libdir                  "${CMAKE_INSTALL_PREFIX}/lib")
set(prefix                  "${CMAKE_INSTALL_PREFIX}")
# Finally generate a pkg-config file matching this config
configure_file("${PROJECT_SOURCE_DIR}/usjsearch.pc.in"
               "${CMAKE_BINARY_DIR}/usjsearch.pc" @ONLY)
install(FILES "${CMAKE_BINARY_DIR}/usjsearch.pc"
        DESTINATION lib/pkgconfig)
# Install headers
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/" 
	DESTINATION include/usjsearch
	FILES_MATCHING PATTERN "*.h")
# Install Libraries
install(TARGETS usjAIL DESTINATION lib)
# Install Executable
install(TARGETS MapRoutePlanner DESTINATION bin)    
