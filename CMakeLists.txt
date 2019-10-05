cmake_minimum_required(VERSION 3.13...3.14)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/cpack)
project(bomberman LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")

#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
endif()

add_subdirectory(3rdParty)

if ( WIN32 )
  set(FFMPEG_PATH_CUSTOM "${CMAKE_CURRENT_LIST_DIR}/3rdParty/ffmpeg/win64")
elseif( APPLE )
  set(FFMPEG_PATH_CUSTOM "${CMAKE_CURRENT_LIST_DIR}/3rdParty/ffmpeg/macOs")
endif()

find_package(FFmpeg REQUIRED)
find_package(OpenGL 3.0 REQUIRED)
find_package(OpenCL 1.2 REQUIRED)

add_subdirectory(src)
add_subdirectory(inc)

source_group("Source Files" FILES ${BOMBERMAN_LOCAL_SOURCES})
source_group("Header File" FILES ${BOMBERMAN_LOCAL_INCLUDES})

option(CREATE_PACK "Create *.app or *.exe" ON)

if (CREATE_PACK AND APPLE)
  add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${BOMBERMAN_SOURCES})
else()
  add_executable(${PROJECT_NAME} ${BOMBERMAN_SOURCES})
endif()

target_compile_options(bomberman PRIVATE -Wall -Wextra -Werror -Wno-deprecated -fvisibility=hidden)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
	USE_FOLDERS ON)

target_include_directories(${PROJECT_NAME} PRIVATE inc/)

target_link_libraries (${PROJECT_NAME} PRIVATE	OpenGL::GL
                                            OpenCL::OpenCL
                                            SDL2::SDL2
                                            SDL2::SDL_mixer
                                            glew::glew
                                            assimp::assimp
                                            imgui::imgui
                                            glm::glm
                                            stb::stb
                                            cereal::cereal
                                            ffmpeg::ffmpeg)


function(LINK_ASSETS TARGET_ARG SRC_ARG DIST_ARG )
  if (CREATE_PACK)
    add_custom_command(TARGET ${TARGET_ARG} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${SRC_ARG}/Assets" "${DIST_ARG}/Assets" DEPENDS "${DIST_ARG}")
  else()
    add_custom_command(TARGET ${TARGET_ARG} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_ARG}/Assets" "${DIST_ARG}/Assets" DEPENDS "${DIST_ARG}")
  endif()
endfunction()

if ( MSVC )
  link_assets(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
  add_custom_command(TARGET ${PROJECT_NAME} PRE_LINK
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${FFMPEG_PATH_CUSTOM}/libshared $<TARGET_FILE_DIR:${PROJECT_NAME}>)
else()
  link_assets(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}" $<TARGET_FILE_DIR:${PROJECT_NAME}>)
  add_custom_command(TARGET ${PROJECT_NAME} PRE_LINK
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${FFMPEG_PATH_CUSTOM}/lib $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )

if (CREATE_PACK)
  if(WIN32)
    set(APPS "\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.exe")
  elseif(APPLE)
    set(APPS "\${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.app")
    set(CPACK_GENERATOR "DRAGNDROP")
  endif()
  
  # Directories to look for dependencies
  set(DIRS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

  install(CODE "include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"\" \"${DIRS}\")")
  include(CPack)
endif()
