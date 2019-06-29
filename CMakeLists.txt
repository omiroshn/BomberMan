cmake_minimum_required(VERSION 3.13...3.14)

project(bomberman LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if (APPLE)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
endif()
add_subdirectory(3rdParty)

set ( SDL2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/sdl2/include )
set ( SDL2_LIBRARY SDL2 )

set ( GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glew/include )
set ( GLEW_LIBRARIES libglew_shared )

set ( ASSIMP_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/assimp/include )
set ( ASSIMP_LIBRARY assimp )

set ( GLM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/glm/ )

set ( STB_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/stb/ )

set ( IMGUI_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/imgui/ )
set ( IMGUI_LIBRARY imgui )

find_package(OpenGL 3.0 REQUIRED)
find_package(OpenCL 1.2 REQUIRED)

if (!${OpenCL_FOUND})
message("OpenCL not found")
return()
endif()

message("OPENCL LIB:")
message( ${OpenCL_LIBRARIES} )

message("OPENCL INCLUDE DIRS:")
message(${OpenCL_INCLUDE_DIRS})

add_subdirectory(src)
add_subdirectory(inc)

set(BOMBERMAN_SOURCES
  ${BOMBERMAN_LOCAL_SOURCES}
  ${BOMBERMAN_LOCAL_INCLUDES})

set(BOMBERMAN_INCLUDE_DIRS inc/)

set(BOMBERMAN_LIBS
  ${OPENGL_LIBRARY}
  ${SDL2_LIBRARY}
  ${ASSIMP_LIBRARY}
  ${GLEW_LIBRARIES}
  ${IMGUI_LIBRARY}
  ${OpenCL_LIBRARIES})

set(BOMBERMAN_INCLUDES
  ${BOMBERMAN_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIR}
  ${STB_INCLUDE_DIRS}
  ${ASSIMP_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIRS}
  ${IMGUI_INCLUDE_DIRS}
  ${OpenCL_INCLUDE_DIRS})

source_group("Source Files" FILES ${BOMBERMAN_LOCAL_SOURCES})
source_group("Header File" FILES ${BOMBERMAN_LOCAL_INCLUDES})

add_executable(bomberman ${BOMBERMAN_SOURCES})
set_target_properties(bomberman PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
	USE_FOLDERS ON)
target_include_directories(bomberman PRIVATE ${BOMBERMAN_INCLUDES})
target_link_libraries (bomberman PRIVATE ${BOMBERMAN_LIBS})

target_link_libraries(bomberman PUBLIC Boost::serialization)
target_link_libraries(bomberman PUBLIC Boost::boost)

# if ( CMAKE_COMPILER_IS_GNUCC )
#     target_compile_options(bomberman PRIVATE "-Wall -Wextra -Werror")
# endif()
# if ( MSVC )
#     target_compile_options(bomberman PRIVATE "/W4")
# endif()

if (${CMAKE_SIZEOF_VOID_P} EQUAL "8")
    set (PROJECTNAME_ARCHITECTURE "x64")
else ()
    set (PROJECTNAME_ARCHITECTURE "x86")
endif ()

function(LINK_ASSETS TARGET_ARG SRC_ARG DIST_ARG )
  add_custom_command(TARGET ${TARGET_ARG} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink "${SRC_ARG}/Assets" "${DIST_ARG}/Assets" DEPENDS "${DIST_ARG}")
endfunction()

if ( MSVC )
  link_assets(bomberman "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/$<CONFIG>")
else()
  link_assets(bomberman "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT bomberman)
