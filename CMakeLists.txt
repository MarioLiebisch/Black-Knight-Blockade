#
# Black Knight Blockade, a simple game entry for Ludum Dare 25: You are the Villain
# 
# Copyright (C) 2012 Mario Liebisch <mario.liebisch@gmail.com>
# 
# This file is part of Black Knight Blockade.
# 
# Black Knight Blockade is free software: you can redistribute it and/or modify it under the terms
# of the GNU General Public License as published by the Free Software Foundation, either version 3
# of the License, or (at your option) any later version.
# 
# Black Knight Blockade is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# 
# See the GNU General Public License for more details. You should have received a copy of the GNU
# General Public License along with Black Knight Blockade.
# If not, see <http://www.gnu.org/licenses/>.
# 

cmake_minimum_required(VERSION 2.6)

project(ld25bkb)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(SFML COMPONENTS system audio graphics window REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${SFML_INCLUDE_DIR})

file(GLOB SOURCES "source/*.cpp")
if(MSVC)
  file(GLOB HEADERS "source/*.h")
  list(APPEND SOURCES ${HEADERS})
endif()

if(WIN32 AND NOT MSVC AND (${CMAKE_BUILD_TYPE} MATCHES "Release"))
  add_executable(ld25bkb WIN32 ${SOURCES})
else()
  add_executable(ld25bkb ${SOURCES})
endif()
target_link_libraries(ld25bkb ${SFML_AUDIO_LIBRARY} ${SFML_GRAPHICS_LIBRARY} ${SFML_WINDOW_LIBRARY} ${SFML_SYSTEM_LIBRARY})

install(FILES readme.txt license.txt DESTINATION .)

file(GLOB DATA "data/*")
install(FILES ${DATA} DESTINATION data)
install(TARGETS ld25bkb DESTINATION .)
