cmake_minimum_required(VERSION 3.1)

set(PROJ StartposSwitcher)
set(LANG CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

project(${PROJ} ${LANG})

file(GLOB_RECURSE STARTPOS_SWITCHER_SOURCES src/*.cpp src/*.hpp)

add_library(${PROJ} SHARED ${STARTPOS_SWITCHER_SOURCES})

target_include_directories(${PROJ} PRIVATE 
  "libs/cocos-headers/cocos2dx"
  "libs/cocos-headers/cocos2dx/include"
  "libs/cocos-headers/cocos2dx/kazmath/include"
  "libs/cocos-headers/cocos2dx/platform/"
  "libs/cocos-headers/cocos2dx/platform/win32"
  "libs/cocos-headers/cocos2dx/platform/third_party/win32"
  "libs/cocos-headers/cocos2dx/platform/third_party/win32/OGLES"
  "libs/cocos-headers/extensions"
  "libs/minhook/include/"
  "libs/mat-dash/include"
  "libs/GDHM-API/includes/"
  "libs/gd.h/include/"
  "libs/gd.h/"
)

add_subdirectory(libs/minhook)
add_subdirectory(libs/cocos-headers)
add_subdirectory(libs/mat-dash)

target_link_libraries(${PROJ} cocos2d minhook)
target_link_directories(${PROJ} PRIVATE "libs/GDHM-API/lib")