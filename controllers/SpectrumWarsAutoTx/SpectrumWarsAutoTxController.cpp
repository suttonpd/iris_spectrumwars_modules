/**
 * \file controllers/SpectrumWarsAutoTx/SpectrumWarsAutoTxController.cpp
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
 * The controller for the Spectrum Wars auto transmitter. This controller
 * manages the synchronization between tx and rx required for periodic
 * spectrum sensing.
 */

#include "SpectrumWarsAutoTxController.h"

#include <sstream>
#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

namespace iris
{

//! Export library functions
IRIS_CONTROLLER_EXPORTS(SpectrumWarsAutoTxController);

SpectrumWarsAutoTxController::SpectrumWarsAutoTxController()
  : Controller("SpectrumWarsAutoTx", "The SpectrumWars Tx controller",
               "Paul Sutton", "0.1")
  ,isRunning_(false)
  ,refTime_(date(2000,Jan,1))
  ,nChans_(4)
{
  registerParameter("timeopenms", "Time in ms to transmit for",
                    "900", true, timeOpenMs_x);
  registerParameter("delayms", "Delay in ms between tx and rx to allow for bus latency",
                    "10", true, delayMs_x);
  registerParameter("timeclosedms", "Time in ms to sense for",
                    "100", true, timeClosedMs_x);
  registerParameter("txgate", "Tx gate component name",
                    "txgate1", false, txGate_x);
  registerParameter("txgateengine", "Engine containing our tx gate",
                    "phyengine1", false, txGateEngine_x);
  registerParameter("rxgate", "Rx gate component name",
                    "rxgate1", false, rxGate_x);
  registerParameter("rxgateengine", "Engine containing our rx gate",
                    "phyengine2", false, rxGateEngine_x);
  registerParameter("frontendtx", "Name of front end tx component",
                    "usrptx1", false, frontEndTx_x);
  registerParameter("frontendtxengine", "Engine containing our tx front end",
                    "phyengine1", false, frontEndTxEngine_x);
  registerParameter("spectrogramcomponent", "Name of spectrogram component",
                    "spectrogram1", false, spgrmCompName_x);
}

void SpectrumWarsAutoTxController::subscribeToEvents()
{
  subscribeToEvent("psdevent", spgrmCompName_x);
}

void SpectrumWarsAutoTxController::initialize()
{
  channelFreqs_.resize(nChans_);
  channelFreqs_[0] = 2.491e9;
  channelFreqs_[1] = 2.492e9;
  channelFreqs_[2] = 2.493e9;
  channelFreqs_[3] = 2.494e9;
  isRunning_ = true;
  thread_.reset( new boost::thread(
                   boost::bind(&SpectrumWarsAutoTxController::threadLoop, this) ) );
}

void SpectrumWarsAutoTxController::processEvent(Event &e)
{
  //We've only subscribed to psdevent

}

void SpectrumWarsAutoTxController::destroy()
{
  thread_->interrupt();
  isRunning_ = false;
  thread_->join();
}

void SpectrumWarsAutoTxController::threadLoop()
{
  uint8_t count = 0;
  boost::this_thread::sleep(milliseconds(5000));
  try
  {
    while(isRunning_)
    {
      boost::this_thread::interruption_point();

      uint64_t timestamp = getTimestamp();
      openTxGate();
      boost::this_thread::sleep(milliseconds(timeOpenMs_x));

      count++;
      setChannel(count%nChans_);

      //TODO: Add a delay here to allow data to pass through the USRP
      boost::this_thread::sleep(milliseconds(delayMs_x));

      openRxGate();
      boost::this_thread::sleep(milliseconds(timeClosedMs_x));

      //TODO: Get sensing data and reconfig tx here
    }
  }
  catch(boost::thread_interrupted& e)
  {}
}

uint64_t SpectrumWarsAutoTxController::getTimestamp()
{
  return (microsec_clock::local_time() - refTime_).total_milliseconds();
}

void SpectrumWarsAutoTxController::openTxGate()
{
  //Open the gate
  ReconfigSet r;
  ParametricReconfig p;
  p.engineName = txGateEngine_x;
  p.componentName = txGate_x;
  p.parameterName = "open";
  p.parameterValue = "true";
  r.paramReconfigs.push_back(p);
  reconfigureRadio(r);
}

void SpectrumWarsAutoTxController::openRxGate()
{
  //Open the gate
  ReconfigSet r;
  ParametricReconfig p;
  p.engineName = rxGateEngine_x;
  p.componentName = rxGate_x;
  p.parameterName = "open";
  p.parameterValue = "true";
  r.paramReconfigs.push_back(p);
  reconfigureRadio(r);
}

void SpectrumWarsAutoTxController::setChannel(uint8_t ch)
{
  if(ch >= nChans_)
    return;
  ReconfigSet r;
  ParametricReconfig p;
  p.engineName = frontEndTxEngine_x;
  p.componentName = frontEndTx_x;
  p.parameterName = "frequency";
  stringstream ss;
  ss << channelFreqs_[ch];
  p.parameterValue = ss.str();
  r.paramReconfigs.push_back(p);
  reconfigureRadio(r);
}


} // namespace iris
