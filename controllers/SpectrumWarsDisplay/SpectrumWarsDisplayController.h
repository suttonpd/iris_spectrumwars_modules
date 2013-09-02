/**
 * \file controllers/SpectrumWarsDisplay/SpectrumWarsDisplayController.h
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
 * A controller for the spectrumwars main display radio.
 */

#ifndef CONTROLLERS_SPECTRUMWARSDISPLAYCONTROLLER_H_
#define CONTROLLERS_SPECTRUMWARSDISPLAYCONTROLLER_H_

#include "irisapi/Controller.h"
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include "SpectrumWarsDisplayPlot/SpectrumWarsDisplayPlot.h"
#include "utility/UdpSocketReceiver.h"
#include "packet.pb.h"

namespace iris
{

class SpectrumWarsDisplayController
  : public Controller
{
public:
  SpectrumWarsDisplayController();
  virtual void subscribeToEvents();
  virtual void initialize();
  virtual void processEvent(Event &e);
	virtual void destroy();

private:
  void socketLoop();
  void processCount(Packet& p);
  void processReconfig(Packet& p);

  int nRows_x;
  std::string spgrmCompName_x;
  int port_x;
  double winCount_x;

  uint32_t countA_;
  uint32_t countB_;
  bool exit_;
  boost::scoped_ptr<SpectrumWarsDisplayPlot> plot_;
  boost::scoped_ptr<boost::thread> socketThread_;
  boost::scoped_ptr<UdpSocketReceiver> rx_;
};

} // namespace iris

#endif // CONTROLLERS_SPECTRUMWARSDISPLAYCONTROLLER_H_
