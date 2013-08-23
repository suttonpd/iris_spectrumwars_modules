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
    SET(sources
      KnobSpin.cpp
      KnobSpin.h
      SpectrumWarsRxGuiWidget.cpp
      SpectrumWarsRxGuiWidget.h
      SpectrumWarsRxGuiWrapper.cpp
      SpectrumWarsRxGuiWrapper.h
      SpectrumWarsRxGui.cpp
      SWRxGuiParams.h
      SWRxGuiCallback.h
    )
    QT4_WRAP_CPP(MOCSrcs KnobSpin.h SpectrumWarsRxGuiWidget.h SpectrumWarsRxGuiWrapper.h)
    ADD_LIBRARY(spectrumwarsrxgui ${sources} ${MOCSrcs})
    TARGET_LINK_LIBRARIES(spectrumwarsrxgui ${QT_LIBRARIES} ${QWT_LIBRARIES})
    IRIS_SET_PIC(spectrumwarsrxgui)
    SET(IRIS_HAVE_SPECTRUMWARSTXGUI ON CACHE INTERNAL "Has SpectrumWarsRxGui been built?")
    ADD_SUBDIRECTORY(test)
ELSE(QT4_FOUND AND QWT_FOUND)
    SET(IRIS_HAVE_SPECTRUMWARSTXGUI OFF CACHE INTERNAL "Has SpectrumWarsRxGui been built?")
ENDIF(QT4_FOUND AND QWT_FOUND)
