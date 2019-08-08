# - Try to find ffmpeg libraries (libavcodec, libavformat and libavutil)
# Once done this will define
#
# FFMPEG_FOUND - system has ffmpeg or libav
# FFMPEG_INCLUDE_DIR - the ffmpeg include directory
# FFMPEG_LIBRARIES - Link these to use ffmpeg
# FFMPEG_LIBAVCODEC
# FFMPEG_LIBAVFORMAT
# FFMPEG_LIBAVUTIL

	find_path(
		FFMPEG_INCLUDE_DIR libavcodec/avcodec.h
		HINTS
		${FFMPEG_INCLUDE_SEARCH_PATHES}
		NO_DEFAULT_PATH
	)

	find_library(FFMPEG_LIBAVCODEC
		NAMES avcodec avcodec.58 avcodec-58
		HINTS
		${FFMPEG_LIB_SEARCH_PATHES}
		NO_DEFAULT_PATH
	)

	find_library(FFMPEG_LIBAVFORMAT
		NAMES avformat avformat.58 avformat-58
		HINTS
		${FFMPEG_LIB_SEARCH_PATHES}
		NO_DEFAULT_PATH
	)

	find_library(FFMPEG_LIBAVUTIL
		NAMES avutil avutil.56 avutil-56
		HINTS
		${FFMPEG_LIB_SEARCH_PATHES}
		NO_DEFAULT_PATH
	)

	find_library(FFMPEG_LIBSWSCALE
		NAMES swscale swscale.5 swscale-5
		HINTS
		${FFMPEG_LIB_SEARCH_PATHES}
		NO_DEFAULT_PATH
	)

	if(FFMPEG_INCLUDE_DIR AND FFMPEG_LIBAVCODEC AND  FFMPEG_LIBAVCODEC AND FFMPEG_LIBAVFORMAT AND FFMPEG_LIBSWSCALE)
		set(FFMPEG_FOUND TRUE)
	endif()

	if(FFMPEG_FOUND)
		set(FFMPEG_LIBRARIES
			${FFMPEG_LIBAVCODEC}
			${FFMPEG_LIBAVFORMAT}
			${FFMPEG_LIBAVUTIL}
			${FFMPEG_LIBSWSCALE})
		if(NOT FFMPEG_FIND_QUIETLY)
			message(STATUS "Found FFMPEG package: ${FFMPEG_LIBRARIES}, ${FFMPEG_INCLUDE_DIR}")
		endif()
	else()
		if(FFMPEG_FIND_REQUIRED)
			message(FATAL_ERROR "Could not find FFMPEG package!")
		endif()
	endif()