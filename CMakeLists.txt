cmake_minimum_required(VERSION 3.12)

set(PROJ StartposSwitcher)

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(${PROJ} VERSION 0.52 DESCRIPTION "Startpos Switcher" LANGUAGES CXX)

add_subdirectory("libs/minhook")
add_subdirectory("libs/GDHM-API")
add_subdirectory("libs/cocos-headers")
add_subdirectory("libs/gd.h")
add_subdirectory("libs/mat-dash")
add_subdirectory("libs/json")

file(GLOB_RECURSE STARTPOS_SWITCHER_SOURCES "src/*.cpp")

add_library(${PROJ} SHARED ${STARTPOS_SWITCHER_SOURCES})

target_link_libraries(${PROJ} cocos2d minhook gdhm mat-dash gd_h nlohmann_json::nlohmann_json)