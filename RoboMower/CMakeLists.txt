project(robomow)
cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
SET(robomow_STATIC_SFML FALSE CACHE BOOL "Choose whether SFML is linked statically or not.")
SET(robomow_STATIC_RUNTIME FALSE CACHE BOOL "Use statically linked standard/runtime libraries? This option must match the one used for SFML.")

#use if you prefer clang, comment out if using g++ (4.9+)
#SET(CMAKE_C_COMPILER    "/usr/bin/clang")
#SET(CMAKE_CXX_COMPILER  "/usr/bin/clang++")
#SET(CMAKE_AR            "/usr/bin/llvm-ar")
#SET(CMAKE_LINKER        "/usr/bin/llvm-ld")
#SET(CMAKE_NM            "/usr/bin/llvm-nm")
#SET(CMAKE_OBJDUMP       "/usr/bin/llvm-objdump")
#SET(CMAKE_RANLIB        "/usr/bin/llvm-ranlib")

if(CMAKE_COMPILER_IS_GNUCXX)
  if(robomow_STATIC_RUNTIME)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y -static")
  else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")
  endif()
endif()

if(robomow_STATIC_SFML)
  SET(SFML_STATIC_LIBRARIES TRUE)
endif()

if(WIN32)
  find_package(SFML 2 REQUIRED graphics window audio system network main)
else()
  find_package(SFML 2 REQUIRED graphics window audio system network)
endif()

if(UNIX)
  find_package(X11 REQUIRED)
endif()

#optional package manager on arch (PKGBUILD script currently missing)
#include (FindPkgConfig)
#PKG_SEARCH_MODULE(SFML 2 REQUIRED system window graphics audio)

include_directories(
  ${SFML_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR}/include
  ${CMAKE_SOURCE_DIR}/../xygine/include)

if(X11_FOUND)
  include_directories(${X11_INCLUDE_DIRS})
endif()

#if using mingw on windows compile the icon / resource files
#set(RES_FILES "")
#if(MINGW)
#  set(RES_FILES "icon.rc")
#  set(CMAKE_RC_COMPILER_INIT windres)
#  enable_language(RC)
#  set(CMAKE_RC_COMPILE_OBJECT
#  "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
#endif()

SET(xygine_DIR ../xygine/src)
include(${xygine_DIR}/CMakeLists.txt)

set(SOURCE
  src/ButtonLogic.cpp
  src/Game.cpp
  src/GameState.cpp
  src/InstructionBlockLogic.cpp
  src/main.cpp
  src/MenuBackgroundState.cpp
  src/MenuJoinState.cpp
  src/MenuLobbyState.cpp
  src/MenuMainState.cpp
  src/MenuOptionState.cpp
  src/MenuPauseState.cpp
  src/RoundedRectangle.cpp
  src/StatsOverlayState.cpp)
#  ${RES_FILES})
  
if(WIN32)
  add_executable(robomow WIN32 ${SOURCE} ${xygine_SRC})
else()
  add_executable(robomow ${SOURCE} ${xygine_SRC})
endif()

target_link_libraries(robomow
  ${SFML_LIBRARIES}
  ${SFML_DEPENDENCIES})

if(UNIX)
  target_link_libraries(robomow
    ${X11_LIBRARIES})
endif()

#install executable
install(TARGETS robomow
  RUNTIME DESTINATION .)

#install game data
install(DIRECTORY assets
  DESTINATION .)

#install the OpenAL DLL
if(WIN32)
  install(FILES ${SFML_INCLUDE_DIR}/../bin/openal32.dll
    DESTINATION .)
endif()
