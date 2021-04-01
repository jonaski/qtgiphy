set(QTGIPHY_VERSION_MAJOR 0)
set(QTGIPHY_VERSION_MINOR 1)
set(QTGIPHY_VERSION_PATCH 1)
#set(QTGIPHY_VERSION_PRERELEASE rc1)

set(INCLUDE_GIT_REVISION ON)

set(majorminorpatch "${QTGIPHY_VERSION_MAJOR}.${QTGIPHY_VERSION_MINOR}.${QTGIPHY_VERSION_PATCH}")

set(QTGIPHY_VERSION_DISPLAY "${majorminorpatch}")
set(QTGIPHY_VERSION_RPM_V   "${majorminorpatch}")
set(QTGIPHY_VERSION_RPM_R   "1")
set(QTGIPHY_VERSION_PACKAGE "${majorminorpatch}")

if(${QTGIPHY_VERSION_PATCH} EQUAL "0")
  set(QTGIPHY_VERSION_DISPLAY "${QTGIPHY_VERSION_MAJOR}.${QTGIPHY_VERSION_MINOR}")
endif(${QTGIPHY_VERSION_PATCH} EQUAL "0")

if(QTGIPHY_VERSION_PRERELEASE)
  set(QTGIPHY_VERSION_DISPLAY "${QTGIPHY_VERSION_DISPLAY} ${QTGIPHY_VERSION_PRERELEASE}")
  set(QTGIPHY_VERSION_RPM_R   "0.${QTGIPHY_VERSION_PRERELEASE}")
  set(QTGIPHY_VERSION_PACKAGE "${QTGIPHY_VERSION_PACKAGE}${QTGIPHY_VERSION_PRERELEASE}")
endif(QTGIPHY_VERSION_PRERELEASE)


if(INCLUDE_GIT_REVISION AND EXISTS "${CMAKE_SOURCE_DIR}/.git")

  find_program(GIT_EXECUTABLE git)
  if(NOT GIT_EXECUTABLE OR GIT_EXECUTABLE-NOTFOUND)
    message(FATAL_ERROR "Missing GIT executable." )
  endif()

  # Get the current working branch
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE GIT_CMD_RESULT_BRANCH
    OUTPUT_VARIABLE GIT_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )
  # Get the latest abbreviated commit hash of the working branch
  execute_process(
    COMMAND ${GIT_EXECUTABLE} describe --long --tags --always
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    RESULT_VARIABLE GIT_CMD_RESULT_REVISION
    OUTPUT_VARIABLE GIT_REVISION
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
  )

  if(NOT ${GIT_CMD_RESULT_REVISION} EQUAL 0)
    message(FATAL_ERROR "GIT command failed to get revision string '${GIT_REVISION}'")
  endif()

endif()

if(FORCE_GIT_REVISION)
  set(GIT_REVISION ${FORCE_GIT_REVISION})
endif()

if(GIT_REVISION)

  string(REGEX REPLACE "^(.+)-([0-9]+)-(g[a-f0-9]+)$" "\\1;\\2;\\3" GIT_PARTS ${GIT_REVISION})

  if(NOT GIT_PARTS)
    message(FATAL_ERROR "Failed to parse git revision string '${GIT_REVISION}'")
  endif()

  list(LENGTH GIT_PARTS GIT_PARTS_LENGTH)
  if(NOT GIT_PARTS_LENGTH EQUAL 3)
    message(FATAL_ERROR "Failed to parse git revision string '${GIT_REVISION}'")
  endif()

  list(GET GIT_PARTS 0 GIT_TAGNAME)
  list(GET GIT_PARTS 1 GIT_COMMITCOUNT)
  list(GET GIT_PARTS 2 GIT_SHA1)

  set(HAS_GIT_REVISION ON)

  set(QTGIPHY_VERSION_DISPLAY "${GIT_REVISION}")
  set(QTGIPHY_VERSION_PACKAGE "${GIT_TAGNAME}.${GIT_COMMITCOUNT}.${GIT_SHA1}")
  set(QTGIPHY_VERSION_RPM_V   "${GIT_TAGNAME}")
  set(QTGIPHY_VERSION_RPM_R   "2.${GIT_COMMITCOUNT}.${GIT_SHA1}")

endif()

message(STATUS "QtGiphy Version:")
message(STATUS "Display:  ${QTGIPHY_VERSION_DISPLAY}")
message(STATUS "Package:  ${QTGIPHY_VERSION_PACKAGE}")
message(STATUS "Rpm:      ${QTGIPHY_VERSION_RPM_V}-${QTGIPHY_VERSION_RPM_R}")
