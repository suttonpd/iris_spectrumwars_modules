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

MESSAGE(STATUS "  Processing spectrumwarsrxcounter.")

########################################################################
# Add includes and dependencies
########################################################################
FIND_PACKAGE( Protobuf )

########################################################################
# Build the library from source files
########################################################################
SET(sources
    SpectrumWarsRxCounterController.cpp
    packet.pb.cc
)

IF(PROTOBUF_FOUND)
    PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS packet.proto)

    # Targets must be globally unique for cmake
    ADD_LIBRARY(controller_spectrumwarsrxcounter SHARED ${sources})
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})
    TARGET_LINK_LIBRARIES(controller_spectrumwarsrxcounter ${PROTOBUF_LIBRARIES})
    SET_TARGET_PROPERTIES(controller_spectrumwarsrxcounter PROPERTIES OUTPUT_NAME "spectrumwarsrxcounter")
    SET_TARGET_PROPERTIES(controller_spectrumwarsrxcounter PROPERTIES DEFINE_SYMBOL "IRIS_CORE_DLL_EXPORTS")
    IRIS_INSTALL(controller_spectrumwarsrxcounter)
    IRIS_APPEND_INSTALL_LIST("spectrumwarsrxcounter")
ELSE(PROTOBUF_FOUND)
  IRIS_APPEND_NOINSTALL_LIST("spectrumwarsrxcounter")
ENDIF(PROTOBUF_FOUND)
