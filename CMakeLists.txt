cmake_minimum_required(VERSION 3.13)

project(bomberman)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmakeModules)
set(ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE STRING "Root dir")

set(CP_CMAKE_DIR ${PROJECT_SOURCE_DIR}/cmakeModules)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib) # Setup linker libs

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

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Werror -Wextra -std=c++14")
find_package(OpenGL REQUIRED)

set(BOMBERMAN_SOURCES
    src/main.cpp
    src/nibblerRenderer/Renderer.cpp
    src/nibblerRenderer/CustomException.cpp
    src/nibblerRenderer/ResourceManager.cpp
    src/nibblerRenderer/Shader.cpp
    src/nibblerRenderer/SpriteRenderer.cpp
    src/nibblerRenderer/Texture.cpp
    inc/Core.h
    src/game_logic/MapGeneration.cpp
    inc/game_logic/MapGeneration.h
    inc/game_logic/Serialization.h
    src/game_logic/Serialization.cpp)


set(BOMBERMAN_LIBS
    ${OPENGL_LIBRARY}
    ${SDL2_LIBRARY}
    ${ASSIMP_LIBRARY}
    ${GLEW_LIBRARIES}
    ${IMGUI_LIBRARY}
)

set(BOMBERMAN_INCLUDES
  inc/
  inc/nibblerRenderer/
  ${GLM_INCLUDE_DIRS}
  ${STB_INCLUDE_DIRS}
  ${SDL2_INCLUDE_DIR}
  ${ASSIMP_INCLUDE_DIR}
  ${GLEW_INCLUDE_DIRS}
  ${IMGUI_INCLUDE_DIRS}
)

include_directories(${BOMBERMAN_INCLUDES})

add_executable(bomberman  ${BOMBERMAN_SOURCES})

target_link_libraries (bomberman ${BOMBERMAN_LIBS})

if (${CMAKE_SIZEOF_VOID_P} EQUAL "8")
    set (PROJECTNAME_ARCHITECTURE "x64")
else ()
    set (PROJECTNAME_ARCHITECTURE "x86")
endif ()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT bomberman)
