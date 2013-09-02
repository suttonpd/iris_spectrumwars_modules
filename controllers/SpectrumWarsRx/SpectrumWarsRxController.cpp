/**
 * \file controllers/SpectrumWarsRx/SpectrumWarsRxController.cpp
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

#include "SpectrumWarsRxController.h"

#include <sstream>
#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "packet.pb.h"

using namespace std;

namespace iris
{

//! Export library functions
IRIS_CONTROLLER_EXPORTS(SpectrumWarsRxController);

SpectrumWarsRxController::SpectrumWarsRxController()
  : Controller("SpectrumWarsRx", "The SpectrumWars Rx controller",
               "Paul Sutton", "0.1")
  ,counter_(0)
{
  registerParameter("id", "Identifier for this node",
      "TeamA", false, id_x);
  registerParameter("address", "Address of display node",
      "127.0.0.1", false, address_x);
  registerParameter("port", "Port of display node",
      "12345", false, port_x);
  registerParameter("triggernum", "Send a udp packet every triggernum packets",
      "1000", false, triggerNum_x);
  registerParameter("minfrequency", "Minimum frequency", "2400000000",
      false, minFrequency_x, Interval<double>(0,6000000000));
  registerParameter("maxfrequency", "Maximum frequency", "2405000000",
      false, maxFrequency_x, Interval<double>(0,6000000000));
  registerParameter("minbandwidth", "Minimum bandwidth", "200000",
      false, minBandwidth_x, Interval<double>(0,20000000));
  registerParameter("maxbandwidth", "Maximum bandwidth", "5000000",
      false, maxBandwidth_x, Interval<double>(0,20000000));
  registerParameter("mingain", "Minimum gain", "0",
      false, minGain_x, Interval<double>(0,40));
  registerParameter("maxgain", "Maximum gain", "1",
      false, maxGain_x, Interval<double>(0,40));
  registerParameter("frontendrx", "Name of front end rx component",
                    "usrprx1", false, frontEndRx_x);
  registerParameter("frontendrxengine", "Engine containing our rx front end",
                    "phyengine1", false, frontEndRxEngine_x);
}

void SpectrumWarsRxController::subscribeToEvents()
{
  subscribeToEvent("havedataset", "datasetcounter1");
}

void SpectrumWarsRxController::initialize()
{
  tx_.reset(new UdpSocketTransmitter(address_x, port_x));

  double m = 1000000.0;
  currentGain_ = minGain_x;
  currentFreq_ = minFrequency_x/m;
  currentRate_ = minBandwidth_x/m;

  SWRxGuiParams p;
  p.minFreq = minFrequency_x/m;
  p.maxFreq = maxFrequency_x/m;
  p.minBW = minBandwidth_x/m;
  p.maxBW = maxBandwidth_x/m;
  p.minGain = minGain_x;
  p.maxGain = maxGain_x;

  gui_.reset(new SpectrumWarsRxGui(p,this));
}

void SpectrumWarsRxController::processEvent(Event &e)
{
  if(e.eventName == "guifrequency")
    processFrequency(boost::any_cast<double>(e.data.front()));
  if(e.eventName == "guibandwidth")
    processBandwidth(boost::any_cast<double>(e.data.front()));
  if(e.eventName == "guigain")
    processGain(boost::any_cast<double>(e.data.front()));
  if(e.eventName == "havedataset")
    processHaveData();
}

void SpectrumWarsRxController::destroy()
{}

void SpectrumWarsRxController::processFrequency(double f)
{
  LOG(LDEBUG) << "Frequency: " << f << "MHz";
  ReconfigSet r;
  ParametricReconfig p;
  p.engineName = frontEndRxEngine_x;
  p.componentName = frontEndRx_x;
  p.parameterName = "frequency";
  stringstream str;
  str << f*1000000;
  p.parameterValue = str.str();

  r.paramReconfigs.push_back(p);
  reconfigureRadio(r);

  currentFreq_ = f*1000000;
  notifyDisplay();
}

void SpectrumWarsRxController::processBandwidth(double b)
{
  LOG(LDEBUG) << "Bandwidth: " << b << "MHz";
  ReconfigSet r;
  ParametricReconfig p;
  p.engineName = frontEndRxEngine_x;
  p.componentName = frontEndRx_x;
  p.parameterName = "rate";
  stringstream str;
  str << b*1000000;
  p.parameterValue = str.str();

  r.paramReconfigs.push_back(p);
  reconfigureRadio(r);

  currentRate_ = b*1000000;
  notifyDisplay();
}

void SpectrumWarsRxController::processGain(double g)
{
  LOG(LDEBUG) << "Gain: " << g;

  ReconfigSet r;
  ParametricReconfig p;
  p.engineName = frontEndRxEngine_x;
  p.componentName = frontEndRx_x;
  p.parameterName = "gain";
  stringstream str;
  str << g;
  p.parameterValue = str.str();

  r.paramReconfigs.push_back(p);
  reconfigureRadio(r);

  currentGain_ = g;
  notifyDisplay();
}

void SpectrumWarsRxController::processHaveData()
{
  std::vector<uint8_t> buffer;
  if(++counter_ >= triggerNum_x)
  {
    Packet p;
    p.set_type(Packet_Type_COUNT);
    p.set_teamid(id_x);
    p.set_count(counter_);
    buffer.resize(p.ByteSize());
    p.SerializeWithCachedSizesToArray(&buffer.front());
    tx_->write(buffer.begin(), buffer.end());
    counter_ = 0;
  }
}

void SpectrumWarsRxController::notifyDisplay()
{
  std::vector<uint8_t> buffer;
  Packet p;
  p.set_type(Packet_Type_RECONFIG);
  p.set_teamid(id_x);
  p.set_frequency(currentFreq_);
  p.set_rate(currentRate_);
  p.set_gain(currentGain_);
  buffer.resize(p.ByteSize());
  p.SerializeWithCachedSizesToArray(&buffer.front());
  tx_->write(buffer.begin(), buffer.end());
}


} // namespace iris
