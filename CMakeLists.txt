# Name of the project
project(thunar-torrent-property)
set(CMAKE_SHARED_LIBRARY_PREFIX "")

# Minimum version of cmake required to build this project
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# Set the default build type to release
if(NOT CMAKE_BUILD_TYPE )
   set( CMAKE_BUILD_TYPE Release ... FORCE )
endif()

# Find required library headers & files
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBTORRENT_RB REQUIRED libtorrent-rasterbar>=1.1)
pkg_check_modules(GLIB REQUIRED glib-2.0>=2.44)

# Find the target thunarx version
if(NOT THUNARX_TARGET_VERSION)
    pkg_check_modules(THUNARX2 QUIET thunarx-2)
    pkg_check_modules(THUNARX3 QUIET thunarx-3)
    if (${THUNARX3_FOUND})
        set(THUNARX_TARGET_VERSION 3)
    elseif (${THUNARX2_FOUND})
        set(THUNARX_TARGET_VERSION 2)
    endif()
endif()
if(${THUNARX_TARGET_VERSION} EQUAL 2)
    pkg_check_modules(THUNARX REQUIRED thunarx-2)
    pkg_check_modules(GTK REQUIRED gtk+-2.0)
else()
    pkg_check_modules(THUNARX REQUIRED thunarx-3)
    pkg_check_modules(GTK REQUIRED gtk+-3.0)
endif()

# Let the C compiler know where headers are
include_directories(${GTK_INCLUDE_DIRS})
include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${THUNARX_INCLUDE_DIRS})
include_directories(${LIBTORRENT_RB_INCLUDE_DIRS})

# Let the linker know where libraries are
link_directories(${GTK_LIBRARY_DIRS})
link_directories(${GLIB_LIBRARY_DIRS})
link_directories(${THUNARX_LIBRARY_DIRS})
link_directories(${LIBTORRENT_RB_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTK_CFLAGS_OTHER})
add_definitions(${GLIB_CFLAGS_OTHER})
add_definitions(${THUNARX_CFLAGS_OTHER})
add_definitions(-DLIBTORRENT_RB_VERSION=${LIBTORRENT_RB_VERSION})
add_definitions(${LIBTORRENT_RB_CFLAGS_OTHER})

# Link against Boost if not using libtorrent 1.2
if(LIBTORRENT_RB_VERSION VERSION_LESS "1.2")
    find_package(Boost 1.49 REQUIRED COMPONENTS system filesystem)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
    add_definitions(${Boost_CFLAGS_OTHER})
endif()

# Find source files
file(GLOB SOURCES src/*.c src/utils/*.c)
file(GLOB CXX_SOURCES src/wrappers/*.cpp)

# Add a shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES} ${CXX_SOURCES})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${LIBTORRENT_RB_LIBRARIES})

# Add C/C++ features constraints
target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_auto_type)

# Find the thunarx extension dir
execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=extensionsdir thunarx-${THUNARX_TARGET_VERSION}
                OUTPUT_VARIABLE THUNARX_EXTENSIONS_DIR
                OUTPUT_STRIP_TRAILING_WHITESPACE)

# Install the library to the extensions directory
install(TARGETS ${PROJECT_NAME} DESTINATION ${THUNARX_EXTENSIONS_DIR})
