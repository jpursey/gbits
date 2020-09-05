## Copyright (c) 2020 John Pursey
##
## Use of this source code is governed by an MIT-style License that can be found
## in the LICENSE file or at https://opensource.org/licenses/MIT.

cmake_minimum_required (VERSION 3.8)

###############################################################################
## Set project configuration
###############################################################################

project(BlockWorld CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

###############################################################################
# Game Bits configuration
###############################################################################

STRING(REGEX REPLACE "\\\\" "/" CURRENT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(GB_DIR "${CURRENT_DIR}/../..")

# Optionally set your own build and bin directories. The examples just use the
# same directory as game-bits.
# set(GB_BUILD_DIR "${CURRENT_DIR}/build")
# set(GB_BIN_DIR "${CURRENT_DIR}/bin")

add_subdirectory("${GB_DIR}" gb)
gb_config()
gb_external_libraries()

###############################################################################
## Game Source
###############################################################################

set(block_world_SOURCE
  base_state.cc base_state.h
  block_world.cc block_world.h
  main.cc
  play_state.cc play_state.h
  states.cc states.h
  title_state.cc title_state.h
)

set(block_world_DEPS
  absl::strings
  gb_file
  gb_game
  gb_imgui
  gb_message
  gb_render
  gb_render_vulkan
  gb_render_vulkan_sdl
  gb_resource
)

set(block_world_LIBS
  glog
  sdl2
  sdl2_main
)

gb_add_win_executable(block_world)