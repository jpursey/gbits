## Copyright (c) 2020 John Pursey
##
## Use of this source code is governed by an MIT-style License that can be found
## in the LICENSE file or at https://opensource.org/licenses/MIT.

set(gb_container_SOURCE
  buffer_view.h
  buffer_view_2d.h
  buffer_view_3d.h
  buffer_view_operations.h
  container_stub.cc
)

set(gb_container_TEST_SOURCE
  buffer_view_test.cc
  buffer_view_2d_test.cc
  buffer_view_3d_test.cc
)

set(gb_container_DEPS
  gb_base
)

set(gb_container_LIBS
  glog
  glm
)

gb_add_library(gb_container)
