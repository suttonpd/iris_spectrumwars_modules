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
      SpectrumWarsTxGuiWidget.cpp
      SpectrumWarsTxGuiWidget.h
      SpectrumWarsTxGuiWrapper.cpp
      SpectrumWarsTxGuiWrapper.h
      SpectrumWarsTxGui.cpp
      SWTxGuiParams.h
      SWTxGuiCallback.h
    )
    QT4_WRAP_CPP(MOCSrcs KnobSpin.h SpectrumWarsTxGuiWidget.h SpectrumWarsTxGuiWrapper.h)
    ADD_LIBRARY(spectrumwarstxgui ${sources} ${MOCSrcs})
    TARGET_LINK_LIBRARIES(spectrumwarstxgui ${QT_LIBRARIES} ${QWT_LIBRARIES})
    IRIS_SET_PIC(spectrumwarstxgui)
    SET(IRIS_HAVE_SPECTRUMWARSTXGUI ON CACHE INTERNAL "Has SpectrumWarsTxGui been built?")
    ADD_SUBDIRECTORY(test)
ELSE(QT4_FOUND AND QWT_FOUND)
    SET(IRIS_HAVE_SPECTRUMWARSTXGUI OFF CACHE INTERNAL "Has SpectrumWarsTxGui been built?")
ENDIF(QT4_FOUND AND QWT_FOUND)
