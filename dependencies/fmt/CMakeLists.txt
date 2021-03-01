FetchContent_GetProperties(fmt)
if(NOT fmt_POPULATED)
  if (USE_SYSTEM_DEPS)
    find_package(fmt ${FMT_VERSION} QUIET)
  endif()

  if(NOT fmt_FOUND)
    message(STATUS "Fetching fmt v${FMT_VERSION}")

    FetchContent_Populate(fmt)
    add_external_dependency(fmt)
  endif()
endif()

