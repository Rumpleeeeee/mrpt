# ----------------------------------------------------------------------------
#  Update the library version header file
#    FILE_TO_PARSE="SRC/include/mrpt/MRPT_version.h.in"
#    TARGET_FILE  ="MRPT_version.h"
# ----------------------------------------------------------------------------
SET(CMAKE_MRPT_COMPLETE_NAME "MRPT ${CMAKE_MRPT_VERSION_NUMBER_MAJOR}.${CMAKE_MRPT_VERSION_NUMBER_MINOR}.${CMAKE_MRPT_VERSION_NUMBER_PATCH}")
# Build a three digits version code, eg. 0.5.1 -> 051,  1.2.0 -> 120
SET(CMAKE_MRPT_VERSION_CODE "0x${CMAKE_MRPT_VERSION_NUMBER_MAJOR}${CMAKE_MRPT_VERSION_NUMBER_MINOR}${CMAKE_MRPT_VERSION_NUMBER_PATCH}")

# SOURCE_DATE_EPOCH: See Specs in https://reproducible-builds.org/specs/source-date-epoch/
# Take its value from: 
# 1) The file SRC/SOURCE_DATE_EPOCH  (generated by scripts creating Windows/Debian packages)
# 2) The env var "SOURCE_DATE_EPOCH"
# 3) The output from "git log -1 --pretty=%ct"
# 4) As a last resort, leave it blank, so the C++ source code will use current time.

SET(CMAKE_SOURCE_DATE_EPOCH "")
# 1: SRC/SOURCE_DATE_EPOCH
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/SOURCE_DATE_EPOCH")
  FILE(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/SOURCE_DATE_EPOCH" CMAKE_SOURCE_DATE_EPOCH LIMIT_COUNT 1) # Read only the first line
endif()
if ("" STREQUAL "${CMAKE_SOURCE_DATE_EPOCH}")
  # If empty: 
  # 2) The env var "SOURCE_DATE_EPOCH"
  if (DEFINED ENV{SOURCE_DATE_EPOCH})
    set(CMAKE_SOURCE_DATE_EPOCH "$ENV{SOURCE_DATE_EPOCH}")
  else()
    # 3) The output from "git log -1 --pretty=%ct"
    find_package(Git QUIET)
    if(GIT_FOUND)
      execute_process(
        COMMAND "${GIT_EXECUTABLE}" "log" "-1" "--pretty=%ct"
        OUTPUT_VARIABLE CMAKE_SOURCE_DATE_EPOCH
        OUTPUT_STRIP_TRAILING_WHITESPACE)
        IF ($ENV{VERBOSE})
         MESSAGE(STATUS "SOURCE_DATE_EPOCH from git log: ${CMAKE_SOURCE_DATE_EPOCH}")
        ENDIF()
    endif()
  endif()
endif()


CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/parse-files/version.h.in" "${MRPT_CONFIG_FILE_INCLUDE_DIR}/mrpt/version.h")


# Prepare version.rc for Windows apps: 
IF (WIN32)
	configure_file(
		${MRPT_SOURCE_DIR}/parse-files/version.rc.in
		${MRPT_BINARY_DIR}/version.rc
		@ONLY)
	SET(MRPT_VERSION_RC_FILE "${MRPT_BINARY_DIR}/version.rc")
ELSE(WIN32)
	SET(MRPT_VERSION_RC_FILE "")
ENDIF (WIN32)
