# Name and language of the project
project(thunar-property-torrent)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

# Minimum version of cmake required to build this project
cmake_minimum_required(VERSION 2.8)

# Use packageconfig to detect headers & library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
pkg_check_modules(THUNARX REQUIRED thunarx-2)
pkg_check_modules(LIBTORRENT_RB REQUIRED libtorrent-rasterbar)
find_package(Boost 1.49 REQUIRED COMPONENTS system filesystem)

# Let the C compiler know where headers are
include_directories(${GTK2_INCLUDE_DIRS})
include_directories(${THUNARX_INCLUDE_DIRS})
include_directories(${LIBTORRENT_RB_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

# Let the linker know where libraries are
link_directories(${GTK2_LIBRARY_DIRS})
link_directories(${THUNARX_LIBRARY_DIRS})
link_directories(${LIBTORRENT_RB_LIBRARY_DIRS})
link_directories(${Boost_LIBRARY_DIRS})


# Add other flags to the compiler
add_definitions(${GTK2_CFLAGS_OTHER})
add_definitions(${THUNARX_CFLAGS_OTHER})



# Find source files
file(GLOB SOURCES src/*.c)
file(GLOB CXX_SOURCES src/wrappers/*.cpp)

# Add a shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${CXX_SOURCES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${LIBTORRENT_RB_LIBRARIES})


# Install the library to the extensions directory
install(TARGETS ${PROJECT_NAME} DESTINATION ${THUNARX_LIBDIR}/thunarx-2)
