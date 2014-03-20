/**
 * \file controllers/SpectrumWarsAutoTx/SpectrumWarsAutoTxController.h
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

#ifndef CONTROLLERS_SPECTRUMWARSAUTOTXCONTROLLER_H_
#define CONTROLLERS_SPECTRUMWARSAUTOTXCONTROLLER_H_

#include "irisapi/Controller.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/scoped_ptr.hpp"

namespace iris
{

class SpectrumWarsAutoTxController
  : public Controller
{
public:
  SpectrumWarsAutoTxController();
	virtual void subscribeToEvents();
	virtual void initialize();
  virtual void processEvent(Event &e);
  virtual void destroy();

private:

  uint32_t timeOpenMs_x;          ///< Time to transmit for
  uint32_t delayMs_x;             ///< Delay between tx and rx to allow for bus latency
  uint32_t timeClosedMs_x;        ///< Time to sense for
  std::string txGate_x;           ///< Tx gate component name
  std::string txGateEngine_x;     ///< Tx gate component engine name
  std::string rxGate_x;           ///< Rx gate component name
  std::string rxGateEngine_x;     ///< Rx gate component engine name
  std::string frontEndTx_x;       ///< Tx front end component name
  std::string frontEndTxEngine_x; ///< Tx front end component engine name
  std::string spgrmCompName_x;    ///< Spectrogram component name (to subscribe to events)

  boost::scoped_ptr< boost::thread > thread_;
  bool isRunning_;
  boost::posix_time::ptime refTime_;
  uint8_t nChans_;
  std::vector<double> channelFreqs_;


  void threadLoop();
  uint64_t getTimestamp();
  void openTxGate();
  void openRxGate();
  void setChannel(uint8_t ch);
};

} // namespace iris

#endif // CONTROLLERS_SPECTRUMWARSAUTOTXCONTROLLER_H_
