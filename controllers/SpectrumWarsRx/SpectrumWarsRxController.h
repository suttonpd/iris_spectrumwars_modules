/**
 * \file controllers/SpectrumWarsRx/SpectrumWarsRxController.h
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

#ifndef CONTROLLERS_SPECTRUMWARSRXCONTROLLER_H_
#define CONTROLLERS_SPECTRUMWARSRXCONTROLLER_H_

#include "boost/scoped_ptr.hpp"
#include "irisapi/Controller.h"
#include "SpectrumWarsRxGui/SpectrumWarsRxGui.h"
#include "utility/UdpSocketTransmitter.h"

namespace iris
{

class SpectrumWarsRxController
  : public Controller
{
public:
  SpectrumWarsRxController();
	virtual void subscribeToEvents();
	virtual void initialize();
  virtual void processEvent(Event &e);
	virtual void destroy();

private:
  void processFrequency(double f);
  void processBandwidth(double b);
  void processGain(double g);
  void processHaveData();
  void notifyDisplay();   ///< Tell the display we've reconfigured

  std::string id_x;       ///< Identifier for this node (must be 5 chars)
  std::string address_x;  ///< UDP target IP address
  int port_x;             ///< UDP port number
  int triggerNum_x;       ///< Send a UDP packet every triggerNum_x packets
  double minFrequency_x;
  double maxFrequency_x;
  double minBandwidth_x;
  double maxBandwidth_x;
  double minGain_x;
  double maxGain_x;
  std::string frontEndRx_x;
  std::string frontEndRxEngine_x;

  boost::scoped_ptr<SpectrumWarsRxGui> gui_;
  uint32_t counter_;
  boost::scoped_ptr<UdpSocketTransmitter> tx_;
  double currentGain_;
  double currentFreq_;
  double currentRate_;
};

} // namespace iris

#endif // CONTROLLERS_SPECTRUMWARSRXCONTROLLER_H_
