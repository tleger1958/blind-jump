cmake_minimum_required(VERSION 3.9)

project(BlindJump)
set(EXECUTABLE_NAME ${PROJECT_NAME})
set(PROJECT_SOURCE_DIR "src/")
set(SFML_DIR "deps/SFML-2.4.1")
set(JSON_DIR "deps/JSON")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/*.cpp")
set(CMAKE_CXX_FLAGS "-fpermissive -pthread -std=c++0x")

set(CMAKE_BUILD_TYPE Release)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 3)
configure_file(
  "cmake-build-debug/config.h.in"
  "../src/config.h"
  )

if(WIN32)
  link_directories(${SFML_DIR}/lib/Release)
endif()

add_executable(${EXECUTABLE_NAME} WIN32 MACOSX_BUNDLE ${SOURCES})

if(APPLE)
  include_directories("${SFML_DIR}/include/" "${JSON_DIR}")
  add_definitions(-DBLINDJUMP_MAC)
  target_link_libraries(${EXECUTABLE_NAME} "-framework sfml-window -framework sfml-graphics -framework sfml-system -framework sfml-audio -Xlinker -rpath -Xlinker @loader_path/../Frameworks")
  set(MACOSX_BUNDLE_BUNDLE_NAME ${EXECUTABLE_NAME})
  set(MACOSX_BUNDLE_ICON_FILE icon.icns)
  add_custom_target(pkg ALL mkdir -p ${EXECUTABLE_NAME}.app/Contents/Resources
    COMMAND mkdir -p ${EXECUTABLE_NAME}.app/Contents/Frameworks
    COMMAND cp -r ../res/* ${EXECUTABLE_NAME}.app/Contents/Resources
    COMMAND cp macos/icon.icns ${EXECUTABLE_NAME}.app/Contents/Resources
    COMMAND cp -r ${SFML_DIR}/lib/* ${EXECUTABLE_NAME}.app/Contents/Frameworks
    COMMAND cp -r ${SFML_DIR}/extlibs/libs-osx/Frameworks/* ${EXECUTABLE_NAME}.app/Contents/Frameworks)
elseif(WIN32)
  include_directories("${SFML_DIR}/include/" "${JSON_DIR}")
  add_definitions(-DBLINDJUMP_WINDOWS)
  # FIXME: how to get path to show up in visual studio's "additional library directories"?
  target_link_libraries(${EXECUTABLE_NAME} sfml-graphics sfml-window sfml-system sfml-audio)
elseif(UNIX)
  # TODO: not enough to just assume non-mac will be linux, there's BSD, etc...
  set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake-build-release" ${CMAKE_MODULE_PATH})
  find_package(SFML 2 REQUIRED system window graphics network audio)
  include_directories(${SFML_INCLUDE_DIRS} ${JSON_DIR})
  add_definitions(-DBLINDJUMP_LINUX)
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()
