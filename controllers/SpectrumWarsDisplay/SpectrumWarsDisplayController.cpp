/**
 * \file controllers/SpectrumWarsDisplay/SpectrumWarsDisplayController.cpp
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
 * Implementation of the spectrum wars main display controller.
 */

#include "SpectrumWarsDisplayController.h"
#include <sstream>
#include <algorithm>
#include <string>
#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "packet.pb.h"


using namespace std;

namespace iris
{

//! Export library functions
IRIS_CONTROLLER_EXPORTS(SpectrumWarsDisplayController);

SpectrumWarsDisplayController::SpectrumWarsDisplayController()
  : Controller("SpectrumWarsDisplay", "Spectrum Wars main display controller",
               "Paul Sutton", "0.1")
  ,countA_(0)
  ,countB_(0)
  ,exit_(false)
{
  registerParameter("spectrogramcomponent", "Name of spectrogram component",
      "spectrogram1", false, spgrmCompName_x);
  registerParameter("numrows", "Number of waterfall rows", "512",
      false, nRows_x, Interval<int>(1,65536));
  registerParameter("port", "Port number to listen to", "12345",
      false, port_x);
  registerParameter("wincount", "Number of successful packets to win", "100000",
      false, winCount_x);
}

void SpectrumWarsDisplayController::subscribeToEvents()
{
  subscribeToEvent("psdevent", spgrmCompName_x);
}

void SpectrumWarsDisplayController::initialize()
{
  plot_.reset(new SpectrumWarsDisplayPlot(512, nRows_x));
  plot_->setTitle("Spectrum Wars");
  plot_->setSpectrogramZAxisScale(0, 1e-6);
  double frequency, rate;
  string f = getParameterValue("frequency", "usrprx1");
  string r = getParameterValue("rate", "usrprx1");
  LOG(LDEBUG) << "Frequency: " << f << " Rate: " << r;
  if(!f.empty() && !r.empty())
  {
    stringstream s1(f);
    s1 >> frequency;
    stringstream s2(r);
    s2 >> rate;
    plot_->setPlotXAxisRange(frequency-(rate/2), frequency+(rate/2));
    plot_->setSpectrogramXAxisRange(frequency-(rate/2), frequency+(rate/2));
  }

  plot_->setLevelLeft(45);
  plot_->setLevelRight(75);

  socketThread_.reset( new boost::thread(boost::bind( &SpectrumWarsDisplayController::socketLoop, this )));
}

void SpectrumWarsDisplayController::processEvent(Event &e)
{
  //We've only subscribed to psdevent
  vector<float> data;
  for(int i=0;i<e.data.size();i++)
    data.push_back(boost::any_cast<float>(e.data[i]));

  plot_->plotNewData(data.begin(), data.end());
}

void SpectrumWarsDisplayController::destroy()
{
  socketThread_->interrupt();
  exit_ = true;
  socketThread_->join();
}

void SpectrumWarsDisplayController::socketLoop()
{
  std::vector<uint8_t> buffer(64);
  int n;
  Packet p;
  rx_.reset(new UdpSocketReceiver(port_x));

  try
  {
    while(!exit_)
    {
      boost::this_thread::interruption_point();
      if(rx_->available() > 0)
      {
        n = rx_->read(buffer.begin(), buffer.end());
        p.ParseFromArray(&buffer.front(), n);
        string s = p.teamid();
        uint32_t c = p.count();
        transform(s.begin(), s.end(), s.begin(), ::tolower);
        LOG(LDEBUG) << "Got scoring packet: " << s << " = " << c;
        if(s == "teama")
        {
          countA_ += c;
          plot_->setLevelLeft(countA_*100/winCount_x);
        }
        if(s == "teamb")
        {
          countB_ += c;
          plot_->setLevelRight(countB_*100/winCount_x);
        }
      }
      else
      {
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
      }
    }
  }
  catch(boost::thread_interrupted& e)
  {}
}

} // namespace iris
