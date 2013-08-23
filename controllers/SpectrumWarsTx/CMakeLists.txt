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

MESSAGE(STATUS "  Processing spectrumwarstx.")

########################################################################
# Add includes and dependencies
########################################################################

########################################################################
# Build the library from source files
########################################################################
SET(sources
	SpectrumWarsTxController.cpp
)

ADD_SUBDIRECTORY(SpectrumWarsTxGui)

IF(IRIS_HAVE_SPECTRUMWARSTXGUI)
  # Static library to be used in tests
  ADD_LIBRARY(controller_spectrumwarstx_static STATIC ${sources})
  TARGET_LINK_LIBRARIES(controller_spectrumwarstx_static spectrumwarstxgui)

  # Shared library to be used in radios
  ADD_LIBRARY(controller_spectrumwarstx SHARED ${sources})
  TARGET_LINK_LIBRARIES(controller_spectrumwarstx spectrumwarstxgui)
  SET_TARGET_PROPERTIES(controller_spectrumwarstx PROPERTIES OUTPUT_NAME "spectrumwarstx")
  SET_TARGET_PROPERTIES(controller_spectrumwarstx PROPERTIES DEFINE_SYMBOL "IRIS_CORE_DLL_EXPORTS")
  IRIS_INSTALL(controller_spectrumwarstx)
  IRIS_APPEND_INSTALL_LIST("spectrumwarstx")

  ADD_SUBDIRECTORY(test)
ELSE(IRIS_HAVE_SPECTRUMWARSTXGUI)
  IRIS_APPEND_NOINSTALL_LIST("spectrumwarstx")
ENDIF(IRIS_HAVE_SPECTRUMWARSTXGUI)
