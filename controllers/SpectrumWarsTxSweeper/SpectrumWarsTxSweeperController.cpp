/**
 * \file controllers/SpectrumWarsTxSweeper/SpectrumWarsTxSweeperController.cpp
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
 * The controller for the Spectrum Wars sweeping transmitter.
 * This controller creates an interferer which sweeps randomly, causing
 * interference for spectrum wars game players.
 */

#include "SpectrumWarsTxSweeperController.h"

#include <sstream>
#include <cstdlib>
#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"

using namespace std;

namespace iris
{

//! Export library functions
IRIS_CONTROLLER_EXPORTS(SpectrumWarsTxSweeperController);

SpectrumWarsTxSweeperController::SpectrumWarsTxSweeperController()
  : Controller("SpectrumWarsTxSweeper", "The SpectrumWars Tx controller",
               "Paul Sutton", "0.1")
  ,exit_(false)
{
  registerParameter("intervalms", "Interval between reconfigurations (ms)",
      "5000", false, intervalMs_x);
  registerParameter("minfrequency", "Minimum frequency", "2400000000",
      false, minF_x, Interval<double>(0,6000000000));
  registerParameter("maxfrequency", "Maximum frequency", "2405000000",
      false, maxF_x, Interval<double>(0,6000000000));
  registerParameter("bandwidth", "Bandwidth", "250000",
      false, bandwidth_x, Interval<double>(0,20000000));
  registerParameter("gain", "Gain", "10",
      false, gain_x, Interval<double>(0,20));
  registerParameter("frontendtx", "Name of front end tx component",
      "usrptx1", false, frontEndTx_x);
  registerParameter("frontendtxengine", "Engine containing our tx front end",
      "phyengine1", false, frontEndTxEngine_x);
}

void SpectrumWarsTxSweeperController::subscribeToEvents()
{
}

void SpectrumWarsTxSweeperController::initialize()
{
  thread_.reset( new boost::thread(boost::bind( &SpectrumWarsTxSweeperController::threadLoop, this )));
}

void SpectrumWarsTxSweeperController::processEvent(Event &e)
{
}

void SpectrumWarsTxSweeperController::destroy()
{
  thread_->interrupt();
  exit_ = true;
  thread_->join();
}

void SpectrumWarsTxSweeperController::threadLoop()
{
  double f = minF_x + (maxF_x-minF_x)/2.0;
  double step = 0.0;

  try
  {
    while(!exit_)
    {
      boost::this_thread::interruption_point();

      // Randomly choose a new frequency
      double newF = (maxF_x-minF_x)*((double)rand()/RAND_MAX);
      newF += minF_x;
      LOG(LDEBUG) << "Sweeping to " << newF << "MHz";

      // Sweep to it
      step = (newF-f)/10;
      for(int i=0;i<10;i++)
      {
        f += step;
        setFrequency(f);
        boost::this_thread::sleep(boost::posix_time::milliseconds(200));
      }
      boost::this_thread::sleep(boost::posix_time::milliseconds(intervalMs_x));
    }
  }
  catch(boost::thread_interrupted& e)
  {}
}

void SpectrumWarsTxSweeperController::setFrequency(double f)
{
  LOG(LDEBUG) << "Frequency: " << f << "MHz";
  ReconfigSet r;
  ParametricReconfig p;
  p.engineName = frontEndTxEngine_x;
  p.componentName = frontEndTx_x;
  p.parameterName = "frequency";
  stringstream str;
  str << f;
  p.parameterValue = str.str();

  r.paramReconfigs.push_back(p);
  reconfigureRadio(r);
}

} // namespace iris
