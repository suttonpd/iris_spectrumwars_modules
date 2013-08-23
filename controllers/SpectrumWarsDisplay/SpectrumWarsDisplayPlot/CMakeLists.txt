########################################################################
# Setup Qt and Qwt
########################################################################
FIND_PACKAGE(Qt4 REQUIRED)
IF(QT4_FOUND)
    INCLUDE(${QT_USE_FILE})
ENDIF(QT4_FOUND) 

FIND_PACKAGE(Qwt REQUIRED)
IF(QT4_FOUND AND QWT_FOUND)
    INCLUDE_DIRECTORIES(${QWT_INCLUDE_DIRS})
ENDIF(QT4_FOUND AND QWT_FOUND)
 
########################################################################
# Build our library
########################################################################
IF(QT4_FOUND AND QWT_FOUND)
  QT4_WRAP_CPP(MOCSrcs SpectrumWarsDisplayPlotWidget.h SpectrumWarsDisplayPlotWrapper.h)
  ADD_LIBRARY(spectrumwarsdisplayplot SpectrumWarsDisplayPlotWidget.cpp SpectrumWarsDisplayPlotWrapper.cpp SpectrumWarsDisplayPlot.cpp ${MOCSrcs})
  TARGET_LINK_LIBRARIES(spectrumwarsdisplayplot ${QT_LIBRARIES} ${QWT_LIBRARIES} lineplot spectrogramplot)
  IRIS_SET_PIC(spectrumwarsdisplayplot)
  SET(IRIS_HAVE_SPECTRUMWARSDISPLAYPLOT ON CACHE INTERNAL "Has SpectrumWarsDisplayPlot been built?")
  #ADD_SUBDIRECTORY(test)
ELSE(QT4_FOUND AND QWT_FOUND)
  SET(IRIS_HAVE_SPECTRUMWARSDISPLAYPLOT OFF CACHE INTERNAL "Has SpectrumWarsDisplayPlot been built?")
ENDIF(QT4_FOUND AND QWT_FOUND)
