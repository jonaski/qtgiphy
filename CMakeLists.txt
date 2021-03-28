project(qtgiphy)

cmake_minimum_required(VERSION 3.0)

include(CheckCXXCompilerFlag)
include(CheckIncludeFiles)
include(FindPkgConfig)
include(cmake/Version.cmake)
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  include(cmake/Rpm.cmake)
  include(cmake/Deb.cmake)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
  list(APPEND COMPILE_OPTIONS /std:c++17)
else()
  list(APPEND COMPILE_OPTIONS
    $<$<COMPILE_LANGUAGE:C>:-std=c99>
    $<$<COMPILE_LANGUAGE:CXX>:-std=c++17>
    -Wall
    -Wextra
    -Wpedantic
    -Wunused
    -Wuninitialized
    -Wredundant-decls
    -Wcast-align
    -Winit-self
    -Wmissing-include-dirs
    -Wmissing-declarations
    -Wstrict-overflow=2
    -Wunused-parameter
    -Wformat=2
    -Wdisabled-optimization
    -Wno-sign-conversion
    $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>
    $<$<COMPILE_LANGUAGE:CXX>:-Wno-old-style-cast>
    $<$<COMPILE_LANGUAGE:CXX>:-fpermissive>
  )
endif()

option(BUILD_WERROR "Build with -Werror" ON)
if(BUILD_WERROR)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif(BUILD_WERROR)

if(COMPILE_OPTIONS)
  add_compile_options(${COMPILE_OPTIONS})
endif()

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
  add_definitions(-DNDEBUG)
  add_definitions(-DQT_NO_DEBUG_OUTPUT)
endif()

find_program(CCACHE_EXECUTABLE NAMES ccache)
if (CCACHE_EXECUTABLE)
  message(STATUS "ccache found: will be used for compilation and linkage")
  SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_EXECUTABLE})
  SET_PROPERTY(GLOBAL PROPERTY RULE_LAUNCH_LINK ${CCACHE_EXECUTABLE})
endif ()
find_program(QT_LCONVERT_EXECUTABLE NAMES lconvert lconvert-qt5 PATHS ${QT_BINARY_DIR} NO_DEFAULT_PATH)
find_program(QT_LCONVERT_EXECUTABLE NAMES lconvert lconvert-qt5)

find_package(PkgConfig REQUIRED)

if(NOT QT_DEFAULT_MAJOR_VERSION)
  set(QT_DEFAULT_MAJOR_VERSION 5)
endif()
set(QT_MAJOR_VERSION ${QT_DEFAULT_MAJOR_VERSION} CACHE STRING "Qt version to use (5 or 6), defaults to ${QT_DEFAULT_MAJOR_VERSION}")

option(BUILD_WITH_QT5 "Use Qt 5" OFF)
option(BUILD_WITH_QT6 "Use Qt 6" OFF)

if(BUILD_WITH_QT5)
  set(QT_MAJOR_VERSION 5)
elseif(BUILD_WITH_QT6)
  set(QT_MAJOR_VERSION 6)
else()
  if(QT_MAJOR_VERSION EQUAL 5)
    set(BUILD_WITH_QT5 ON)
  elseif(QT_MAJOR_VERSION EQUAL 6)
    set(BUILD_WITH_QT6 ON)
  else()
    set(BUILD_WITH_QT5 ON)
    set(QT_MAJOR_VERSION 5)
  endif()
endif()

set(QT_COMPONENTS Core Network Gui Widgets)
find_package(Qt${QT_MAJOR_VERSION} REQUIRED COMPONENTS ${QT_COMPONENTS})
set(QtCore_LIBRARIES Qt${QT_MAJOR_VERSION}::Core)
set(QtCore_LIBRARIES Qt${QT_MAJOR_VERSION}::Network)
set(QtGui_LIBRARIES Qt${QT_MAJOR_VERSION}::Gui)
set(QtWidgets_LIBRARIES Qt${QT_MAJOR_VERSION}::Widgets)
set(QT_LIBRARIES Qt${QT_MAJOR_VERSION}::Core Qt${QT_MAJOR_VERSION}::Network Qt${QT_MAJOR_VERSION}::Gui Qt${QT_MAJOR_VERSION}::Widgets)

# Subdirectories
add_subdirectory(src)
add_subdirectory(dist)
