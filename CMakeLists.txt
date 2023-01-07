cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/.cache/CPM)

set(PROJ StartposSwitcher)

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(cmake/CPM.cmake)

project(${PROJ} VERSION 0.5.2 DESCRIPTION "Startpos Switcher" LANGUAGES CXX)

CPMAddPackage(
  NAME Ccache.cmake
  GITHUB_REPOSITORY TheLartians/Ccache.cmake
  VERSION 1.2.4)
CPMAddPackage(
    NAME minhook
    GITHUB_REPOSITORY TsudaKageyu/minhook
    GIT_TAG 49d03ad)
CPMAddPackage(
    NAME mat-dash
    GITHUB_REPOSITORY zaphir-code/mat-dash
    GIT_TAG c382ee5)
CPMAddPackage(
    NAME gdhm
    GITHUB_REPOSITORY zaphir-code/GDHM-API
    GIT_TAG 428be32)
CPMAddPackage(
    NAME cocos2d
    GITHUB_REPOSITORY HJfod/cocos-headers
    GIT_TAG 231afab)
CPMAddPackage(
    NAME gd_h
    GITHUB_REPOSITORY zaphir-code/gd.h
    GIT_TAG e1d702d)
CPMAddPackage(
    NAME nlohmann_json
    GITHUB_REPOSITORY nlohmann/json
    VERSION 3.11.2)

file(GLOB_RECURSE StartposSwitcher_SOURCES "src/*.cpp")

add_library(${PROJ} SHARED ${StartposSwitcher_SOURCES})

target_link_libraries(${PROJ} cocos2d minhook gdhm mat-dash gd_h nlohmann_json::nlohmann_json)