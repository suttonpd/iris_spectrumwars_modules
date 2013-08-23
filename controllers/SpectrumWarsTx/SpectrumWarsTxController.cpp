/**
 * \file controllers/SpectrumWarsTx/SpectrumWarsTxController.cpp
 * \version 1.0
 *
 * \section COPYRIGHT
 *
 * Copyright 2012-2013 The Iris Project Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution
 * and at http://www.softwareradiosystems.com/iris/copyright.html.
 *
 * \section LICENSE
 *
 * This file is part of the Iris Project.
 *
 * Iris is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Iris is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * A copy of the GNU Lesser General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 * \section DESCRIPTION
 *
 * The controller for the Spectrum Wars transmitter. This controller
 * provides a gui to manually adjust parameters including frequency,
 * bandwidth and power.
 */

#include "SpectrumWarsTxController.h"

#include <sstream>
#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"

using namespace std;

namespace iris
{

//! Export library functions
IRIS_CONTROLLER_EXPORTS(SpectrumWarsTxController);

SpectrumWarsTxController::SpectrumWarsTxController()
  : Controller("SpectrumWarsTx", "The SpectrumWars Tx controller",
               "Paul Sutton", "0.1")
{
  registerParameter("minfrequency", "Minimum frequency", "2400000000",
      false, minFrequency_x, Interval<double>(0,6000000000));
  registerParameter("maxfrequency", "Maximum frequency", "2405000000",
      false, maxFrequency_x, Interval<double>(0,6000000000));
  registerParameter("minbandwidth", "Minimum bandwidth", "200000",
      false, minBandwidth_x, Interval<double>(0,20000000));
  registerParameter("maxbandwidth", "Maximum bandwidth", "5000000",
      false, maxBandwidth_x, Interval<double>(0,20000000));
  registerParameter("mingain", "Minimum gain", "0",
      false, minGain_x, Interval<double>(0,1));
  registerParameter("maxgain", "Maximum gain", "1",
      false, maxGain_x, Interval<double>(0,1));
  registerParameter("frontendtx", "Name of front end tx component",
                    "usrptx1", false, frontEndTx_x);
  registerParameter("frontendtxengine", "Engine containing our tx front end",
                    "phyengine1", false, frontEndTxEngine_x);
}

void SpectrumWarsTxController::subscribeToEvents()
{
}

void SpectrumWarsTxController::initialize()
{
  double m = 1000000.0;
  SWTxGuiParams p;
  p.minFreq = minFrequency_x/m;
  p.maxFreq = maxFrequency_x/m;
  p.minBW = minBandwidth_x/m;
  p.maxBW = maxBandwidth_x/m;
  p.minGain = minGain_x;
  p.maxGain = maxGain_x;

  gui_.reset(new SpectrumWarsTxGui(p,this));
}

void SpectrumWarsTxController::processEvent(Event &e)
{
  if(e.eventName == "guifrequency")
    processFrequency(boost::any_cast<double>(e.data.front()));
  if(e.eventName == "guibandwidth")
    processBandwidth(boost::any_cast<double>(e.data.front()));
  if(e.eventName == "guigain")
    processGain(boost::any_cast<double>(e.data.front()));
}

void SpectrumWarsTxController::destroy()
{}

void SpectrumWarsTxController::processFrequency(double f)
{
  LOG(LDEBUG) << "Frequency: " << f << "MHz";
  ReconfigSet r;
  ParametricReconfig p;
  p.engineName = frontEndTxEngine_x;
  p.componentName = frontEndTx_x;
  p.parameterName = "frequency";
  stringstream str;
  str << f*1000000;
  p.parameterValue = str.str();

  r.paramReconfigs.push_back(p);
  reconfigureRadio(r);
}

void SpectrumWarsTxController::processBandwidth(double b)
{
  LOG(LDEBUG) << "Bandwidth: " << b << "MHz";
  ReconfigSet r;
  ParametricReconfig p;
  p.engineName = frontEndTxEngine_x;
  p.componentName = frontEndTx_x;
  p.parameterName = "rate";
  stringstream str;
  str << b*1000000;
  p.parameterValue = str.str();

  r.paramReconfigs.push_back(p);
  reconfigureRadio(r);
}

void SpectrumWarsTxController::processGain(double g)
{
  LOG(LDEBUG) << "Gain: " << g;
  ReconfigSet r;
  ParametricReconfig p;
  p.engineName = frontEndTxEngine_x;
  p.componentName = frontEndTx_x;
  p.parameterName = "gain";
  stringstream str;
  str << g;
  p.parameterValue = str.str();

  r.paramReconfigs.push_back(p);
  //reconfigureRadio(r);
}


} // namespace iris
