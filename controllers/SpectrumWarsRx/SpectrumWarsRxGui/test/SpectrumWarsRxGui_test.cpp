/**
 * \file controllers/SpectrumWarsRx/SpectrumWarsRxGui/test/SpectrumWarsRxGui_test.cpp
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
 * Main test file for SpectrumWarsRxGui class.
 */

#define BOOST_TEST_MODULE SpectrumWarsRxGui_Test

#include "SpectrumWarsRxGui.h"

#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>
#include <qapplication.h>
#include <cstdlib>
#include <vector>
#include <boost/test/unit_test.hpp>

#include "SWRxGuiParams.h"

#define PI     3.14159265358979323846

using namespace std;

void threadMain1()
{
  SWRxGuiParams p;
  p.minFreq = 2400.00;
  p.maxFreq = 2405.00;
  p.minBW = 0.20;
  p.maxBW = 5.00;
  p.minGain = 0.00;
  p.maxGain = 1.00;
  SpectrumWarsRxGui gui(p);
  boost::this_thread::sleep(boost::posix_time::seconds(10));

}

BOOST_AUTO_TEST_SUITE (SpectrumWarsRxGui_Test)

BOOST_AUTO_TEST_CASE(SpectrumWarsRxGui_Init_Test)
{
  int argc = 1;
  char* argv[] = { const_cast<char *>("SpectrumWarsRxGui_Init_Test"), NULL };
  QApplication a(argc, argv);

  boost::scoped_ptr< boost::thread > thread1_;
  thread1_.reset( new boost::thread( &threadMain1 ) );
  qApp->exec();
  thread1_->join();
}

BOOST_AUTO_TEST_SUITE_END()
