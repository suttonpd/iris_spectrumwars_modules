/**
 * \file controllers/SpectrumWarsAutoRx/SpectrumWarsAutoRxController.cpp
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
 * The controller for the Spectrum Wars automatic receiver.
 */

#include "SpectrumWarsAutoRxController.h"

#include <sstream>
#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "packet.pb.h"

using namespace std;

namespace iris
{

//! Export library functions
IRIS_CONTROLLER_EXPORTS(SpectrumWarsAutoRxController);

SpectrumWarsAutoRxController::SpectrumWarsAutoRxController()
  : Controller("SpectrumWarsAutoRx", "The SpectrumWars Rx controller",
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
}

void SpectrumWarsAutoRxController::subscribeToEvents()
{
  subscribeToEvent("havedataset", "datasetcounter1");
  subscribeToEvent("havedataset", "datasetcounter2");
  subscribeToEvent("havedataset", "datasetcounter3");
  subscribeToEvent("havedataset", "datasetcounter4");
}

void SpectrumWarsAutoRxController::initialize()
{
  tx_.reset(new UdpSocketTransmitter(address_x, port_x));
}

void SpectrumWarsAutoRxController::processEvent(Event &e)
{
  if(e.eventName == "havedataset")
    processHaveData();
}

void SpectrumWarsAutoRxController::destroy()
{}

void SpectrumWarsAutoRxController::processHaveData()
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

void SpectrumWarsAutoRxController::notifyDisplay(double f, double r, double g)
{
  std::vector<uint8_t> buffer;
  Packet p;
  p.set_type(Packet_Type_RECONFIG);
  p.set_teamid(id_x);
  p.set_frequency(f);
  p.set_rate(r);
  p.set_gain(g);
  buffer.resize(p.ByteSize());
  p.SerializeWithCachedSizesToArray(&buffer.front());
  tx_->write(buffer.begin(), buffer.end());
}


} // namespace iris
