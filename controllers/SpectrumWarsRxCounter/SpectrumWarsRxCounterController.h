/**
 * \file controllers/SpectrumWarsRxCounter/SpectrumWarsRxCounterController.h
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
 * A controller for the spectrum wars demo. Counts successfully received
 * frames and tells the central display node about them so the display can
 * be updated.
 */

#ifndef CONTROLLERS_SPECTRUMWARSRXCOUNTERCONTROLLER_H_
#define CONTROLLERS_SPECTRUMWARSRXCOUNTERCONTROLLER_H_

#include "irisapi/Controller.h"
#include "utility/UdpSocketTransmitter.h"
#include <boost/scoped_ptr.hpp>

namespace iris
{

class SpectrumWarsRxCounterController
  : public Controller
{
public:
  SpectrumWarsRxCounterController();
  virtual void subscribeToEvents();
  virtual void initialize();
  virtual void processEvent(Event &e);
	virtual void destroy();

private:
  std::string id_x;       ///< Identifier for this node (must be 5 chars)
  std::string address_x;  ///< UDP target IP address
  int port_x;             ///< UDP port number
  int triggerNum_x;       ///< Send a UDP packet every triggerNum_x packets

  uint32_t counter_;
  boost::scoped_ptr<UdpSocketTransmitter> tx_;
};

} // namespace iris

#endif // CONTROLLERS_SPECTRUMWARSRXCOUNTERCONTROLLER_H_
