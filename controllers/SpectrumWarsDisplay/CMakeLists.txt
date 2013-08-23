#
# Copyright 2012-2013 The Iris Project Developers. See the
# COPYRIGHT file at the top-level directory of this distribution
# and at http://www.softwareradiosystems.com/iris/copyright.html.
#
# This file is part of the Iris Project.
#
# Iris is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of
# the License, or (at your option) any later version.
#
# Iris is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# A copy of the GNU Lesser General Public License can be found in
# the LICENSE file in the top-level directory of this distribution
# and at http://www.gnu.org/licenses/.
#

MESSAGE(STATUS "  Processing spectrumwarsdisplay.")

########################################################################
# Add includes and dependencies
########################################################################
FIND_PACKAGE( Protobuf )

########################################################################
# Build the library from source files
########################################################################
SET(sources
    SpectrumWarsDisplayController.cpp
    packet.pb.cc
)

ADD_SUBDIRECTORY(SpectrumWarsDisplayPlot)

IF(IRIS_HAVE_SPECTRUMWARSDISPLAYPLOT AND PROTOBUF_FOUND)
  PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS packet.proto)

  # Static library to be used in tests
  ADD_LIBRARY(controller_spectrumwarsdisplay_static STATIC ${sources})
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
  TARGET_LINK_LIBRARIES(controller_spectrumwarsdisplay_static spectrumwarsdisplayplot ${PROTOBUF_LIBRARIES})

  # Shared library to be used in radios
  ADD_LIBRARY(controller_spectrumwarsdisplay SHARED ${sources})
  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
  TARGET_LINK_LIBRARIES(controller_spectrumwarsdisplay spectrumwarsdisplayplot ${PROTOBUF_LIBRARIES})

  SET_TARGET_PROPERTIES(controller_spectrumwarsdisplay PROPERTIES OUTPUT_NAME "spectrumwarsdisplay")
  SET_TARGET_PROPERTIES(controller_spectrumwarsdisplay PROPERTIES DEFINE_SYMBOL "IRIS_CORE_DLL_EXPORTS")
  IRIS_INSTALL(controller_spectrumwarsdisplay)
  IRIS_APPEND_INSTALL_LIST("spectrumwarsdisplay")

  ADD_SUBDIRECTORY(test)
ELSE(IRIS_HAVE_SPECTRUMWARSDISPLAYPLOT AND PROTOBUF_FOUND)
  IRIS_APPEND_NOINSTALL_LIST("spectrumwarsdisplay")
ENDIF(IRIS_HAVE_SPECTRUMWARSDISPLAYPLOT AND PROTOBUF_FOUND)


