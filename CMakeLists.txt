cmake_minimum_required(VERSION 3.14)

# -------------------------------------------------
# Project
# -------------------------------------------------
get_filename_component(PROJECT_NAME "${CMAKE_CURRENT_SOURCE_DIR}" NAME)
project(${PROJECT_NAME} LANGUAGES C)

# -------------------------------------------------
# Global options
# -------------------------------------------------
option(BUILD_SHARED_LIBS "Build shared libs" OFF)
# -------------------------------------------------
# Project definitions
# -------------------------------------------------
add_compile_definitions(FUNCTION="${PROJECT_NAME}")

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
option(ENABLE_TESTS "Build test binaries" ON)
if(UNIX OR MSYS OR MINGW)
  set(CMAKE_INSTALL_PREFIX "/usr/local/SignalRegistry")
elseif(WIN32)
  file(TO_CMAKE_PATH "$ENV{LOCALAPPDATA}" LOCALAPPDATA_CMAKE)
  set(CMAKE_INSTALL_PREFIX "${LOCALAPPDATA_CMAKE}/SignalRegistry")
endif()
message(STATUS "[${PROJECT_NAME}] CMake prefix path: ${CMAKE_INSTALL_PREFIX}")



# -------------------------------------------------
# Version from CHANGELOG
# -------------------------------------------------
file(STRINGS "${CMAKE_SOURCE_DIR}/CHANGELOG.md"
     CHANGELOG_LINES
     REGEX "^## \\[")

list(GET CHANGELOG_LINES 0 FIRST_LINE)

string(REGEX REPLACE "^## \\[(.*)\\].*" "\\1"
       PROJECT_VERSION "${FIRST_LINE}")

string(REGEX REPLACE "^## \\[.*\\] - (.*)" "\\1"
       PROJECT_RELEASE_DATE "${FIRST_LINE}")

message(STATUS "[${PROJECT_NAME}] Version      : ${PROJECT_VERSION}")
message(STATUS "[${PROJECT_NAME}] Release Date : ${PROJECT_RELEASE_DATE}")

# -------------------------------------------------
# Sources
# -------------------------------------------------
file(GLOB SOURCES "*.c")
list(REMOVE_ITEM SOURCES
     "${PROJECT_SOURCE_DIR}/main.c"
     "${PROJECT_SOURCE_DIR}/test.c")

file(GLOB INCLUDES "*.h")

# -------------------------------------------------
# Static library (project library)
# -------------------------------------------------
add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION lib)

# -------------------------------------------------
# Dependencies (STATIC ONLY)
# -------------------------------------------------
# FetchContent ile gelen projelerin install() kurallarını KAPAT
install(CODE "set(CMAKE_INSTALL_LOCAL_ONLY TRUE)" ALL_COMPONENTS) 

include(FetchContent)

# ---- Jansson
set(JANSSON_VERSION 2.14.1)
set(JANSSON_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(JANSSON_BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(JANSSON_WITHOUT_TESTS ON CACHE BOOL "" FORCE)

FetchContent_Declare(
  jansson
  URL https://github.com/akheron/jansson/releases/download/v${JANSSON_VERSION}/jansson-${JANSSON_VERSION}.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  TLS_VERIFY FALSE
)
FetchContent_MakeAvailable(jansson)

if(TARGET jansson::jansson)
  set(JANSSON_TARGET jansson::jansson)
else()
  set(JANSSON_TARGET jansson)
endif()

# ---- Argtable3
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ARGTABLE3_ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)
set(ARGTABLE3_ENABLE_TESTS OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  argtable3
  URL https://github.com/argtable/argtable3/releases/download/v3.3.0.116da6c/argtable-v3.3.0.116da6c.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  TLS_VERIFY FALSE
)
FetchContent_MakeAvailable(argtable3)

if(TARGET argtable3::argtable3)
  set(ARGTABLE3_TARGET argtable3::argtable3)
else()
  set(ARGTABLE3_TARGET argtable3)
endif()

# -------------------------------------------------
# Executable
# -------------------------------------------------
include_directories("build/_deps/jansson-build/include")
add_executable(main main.c)

target_link_libraries(main
PRIVATE
${PROJECT_NAME}
${JANSSON_TARGET}
${ARGTABLE3_TARGET}
)

set_target_properties(main PROPERTIES OUTPUT_NAME ${PROJECT_NAME})
install(TARGETS main
        RUNTIME DESTINATION bin)

# -------------------------------------------------
# Build type default
# -------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()
