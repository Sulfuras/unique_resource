# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#

# ------------------------------------------------------------------------------
# CMake basic options
# ------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.22)

# Only set the cxx_standard if it is not set by someone else
if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/prevent_in_source_builds.cmake)

# ------------------------------------------------------------------------------
# MSVC
# ------------------------------------------------------------------------------

# set the startup project for the "play" button in MSVC
# set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT WinXD)

#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Enable Hot Reload for MSVC compilers if supported.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT
      "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>"
  )
endif()

# ------------------------------------------------------------------------------
# Project description and (meta) information
# ------------------------------------------------------------------------------

# Get git revision
#include(GetGitRevisionDescription)
#get_git_head_revision(GIT_REFSPEC GIT_SHA1)
#string(
#  SUBSTRING "${GIT_SHA1}"
#            0
#            12
#            GIT_REV)
#if(NOT GIT_SHA1)
#  set(GIT_REV "0")
#endif()

# Meta information about the project
set(META_PROJECT_NAME "unique_resource")
set(META_PROJECT_DESCRIPTION "std::unique_resource implementation")
set(META_AUTHOR_ORGANIZATION "Sulfuras")
set(META_GITHUB_REPO "https://github.com/Sulfuras/unique_resource")
set(META_AUTHOR_DOMAIN "https://github.com/Sulfuras/unique_resource")
set(META_AUTHOR_MAINTAINER "Sulfuras")
set(META_VERSION_MAJOR "1")
set(META_VERSION_MINOR "0")
set(META_VERSION_PATCH "0")
set(META_VERSION_REVISION "${GIT_REV}")
set(META_VERSION "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_NAME_VERSION "${META_PROJECT_NAME} v${META_VERSION} (${META_VERSION_REVISION})")

string(MAKE_C_IDENTIFIER ${META_PROJECT_NAME} META_PROJECT_ID)
string(TOUPPER ${META_PROJECT_ID} META_PROJECT_ID)
string(TOLOWER ${META_PROJECT_ID} META_PROJECT_ID_LOWER)

# Project Target information

# ------------------------------------------------------------------------------
# Project configuration options
# ------------------------------------------------------------------------------

# Project Options
#option(BUILD_SHARED_LIBS "Build shared instead of static libraries." ON)
option(UNIQUE_RESOURCE_BUILD_TESTS "Setup target to build and run tests." OFF)
option(USE_CCACHE "Enable caching of compiled artifacts using ccache" OFF)

# ------------------------------------------------------------------------------
# Project Declaration
# ------------------------------------------------------------------------------

# Project Declaration
project(
  ${META_PROJECT_NAME}
  VERSION ${META_VERSION}
  DESCRIPTION ${META_PROJECT_DESCRIPTION}
  HOMEPAGE_URL ${META_GITHUB_REPO}
  LANGUAGES CXX)

# Set output directories
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_definitions(-DUNICODE -D_UNICODE)
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/EHsc>")

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------

include(cmake/CPM.cmake)

cpmaddpackage(
  NAME
  googletest
  GITHUB_REPOSITORY
  google/googletest
  VERSION
  1.15.0
)

# ------------------------------------------------------------------------------
# Top level code generation
# ------------------------------------------------------------------------------

configure_file(${PROJECT_SOURCE_DIR}/include/version.h.in ${PROJECT_SOURCE_DIR}/include/unique_resource/version.h)

# ------------------------------------------------------------------------------
# Project modules
# ------------------------------------------------------------------------------

add_subdirectory(src)

## ---- Testing ----

enable_testing()

add_subdirectory(tests)

# ------------------------------------------------------------------------------
# Testing
# ------------------------------------------------------------------------------

#include(CTest)

#if(ENDPOINT_SECURITY_BUILD_TESTS)
#include(CTest)

#include(GoogleSanitizers)

#include(CodeCoverage)
# Enable this target when the project has test cases
#asap_add_code_coverage_all_targets()
#include(Valgrind)

#include(GoogleTest)
#endif()
