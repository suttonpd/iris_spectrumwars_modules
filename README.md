# Iris_SpectrumWars_Modules Project

This repositories contains Iris modules for the "Spectrum Wars" dynamic spectrum access game and demonstration. Iris is a software architecture for building highly reconfigurable radio networks using a component-based design. The architecture comprises separate core and modules repositories. Iris_Core contains the core part of the architecture such as parsers, managers and engines. Iris modules contains the PHY-layer components and radio controllers used in "Spectrum Wars".

## Description

The Iris architecture offers support for all layers of the network stack and provides a platform for the development of not only reconfigurable point-to-point radio links but complete networks of reconfigurable radios. Individual radios are described using an XML document. This lists the components which comprise the radio, gives the values to be used for their parameters and describes the connections between them.

Iris was originally developed by CTVR, The Telecommunications Research Centre, based at University of Dublin, Trinity College. In 2013, it was released under the LGPL v3 license and is currently managed by Software Radio Systems (http://www.softwareradiosystems.com).

## Getting Started

The installation guide can be found here: http://www.softwareradiosystems.com/redmine/projects/iris/wiki/Installing_Iris

## Requirements

Required:
* CMake 2.6 or later - http://www.cmake.org/
* Boost 1.46 or later - http://www.boost.org/
* Iris_Core
* Qt 4.8 - http://qt-project.org/ (For graphical widgets)
* Qwt 6 - http://qwt.sourceforge.net/ (For graphical widgets)
* UHD - http://code.ettus.com/redmine/ettus/projects/uhd/wiki (For using USRP hardware)
* FFTW - http://www.fftw.org/ (For FFT transforms)
* Liquid-DSP - https://github.com/jgaeddert/liquid-dsp (For some PHY components)

## Where To Get Help

* Redmine: http://www.softwareradiosystems.com/redmine/projects/iris
* Iris-discuss mailing list: http://www.softwareradiosystems.com/mailman/listinfo/iris-discuss
* Iris blog: http://irissoftwareradio.wordpress.com

## License

Iris is licensed under LGPL v3. For more information see LICENSE.

