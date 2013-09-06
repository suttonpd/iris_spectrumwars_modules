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

MESSAGE(STATUS "  Processing spectrumwarstxsweeper.")

########################################################################
# Add includes and dependencies
########################################################################
FIND_PACKAGE( Protobuf )

########################################################################
# Build the library from source files
########################################################################
SET(sources
	SpectrumWarsTxSweeperController.cpp
)

IF(PROTOBUF_FOUND)
    INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIRS})
    PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS packet.proto)
    INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR})

    # Static library to be used in tests
    ADD_LIBRARY(controller_spectrumwarstxsweeper_static STATIC ${sources} ${PROTO_SRCS} ${PROTO_HDRS})
    TARGET_LINK_LIBRARIES(controller_spectrumwarstxsweeper_static ${PROTOBUF_LIBRARIES})

    # Shared library to be used in radios
    ADD_LIBRARY(controller_spectrumwarstxsweeper SHARED ${sources} ${PROTO_SRCS} ${PROTO_HDRS})
    TARGET_LINK_LIBRARIES(controller_spectrumwarstxsweeper ${PROTOBUF_LIBRARIES})
    SET_TARGET_PROPERTIES(controller_spectrumwarstxsweeper PROPERTIES OUTPUT_NAME "spectrumwarstxsweeper")
    IRIS_INSTALL(controller_spectrumwarstxsweeper)
    IRIS_APPEND_INSTALL_LIST("spectrumwarstxsweeper")

    ADD_SUBDIRECTORY(test)
ELSE(PROTOBUF_FOUND)
    IRIS_APPEND_NOINSTALL_LIST("spectrumwarstxsweeper")
ENDIF(PROTOBUF_FOUND)
