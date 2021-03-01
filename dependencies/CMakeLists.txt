include(FetchContent)

#=====================================
# Helper functions and macros
#=====================================

# Adds external dependency into a project
function(add_external_dependency dependency)
  add_subdirectory(${${dependency}_SOURCE_DIR} ${${dependency}_BINARY_DIR})
endfunction()

#=====================================
# Options and initial settings
#=====================================

message(STATUS "Fetching third party libraries")
option(LINK_DEPS_STATIC CACHE ON)

set(FMT_VERSION 7.1.3)
set(GOOGLETEST_VERSION 1.10)

if (NOT LINK_DEPS_STATIC)
  set(BUILD_SHARED_LIBS ON)
else()
  set(BUILD_SHARED_LIBS OFF)
endif()

#=====================================
# FetchContent declarations
#=====================================

FetchContent_Declare(
  fmt
  GIT_REPOSITORY "https://github.com/fmtlib/fmt"
  GIT_TAG 7bdf0628b1276379886c7f6dda2cef2b3b374f0b # 7.1.3
)

if(BUILD_TESTING)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY "https://github.com/google/googletest"
    GIT_TAG 703bd9caab50b139428cea1aaff9974ebee5742e # 1.10
  )
endif()

#=====================================

add_subdirectory(fmt)
if(BUILD_TESTING)
  add_subdirectory(googletest)
endif()
