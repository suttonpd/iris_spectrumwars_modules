/**
 * \file controllers/SpectrumWarsDisplay/SpectrumWarsDisplayPlot/test/SpectrumWarsDisplayPlot_test.cpp
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
 * Main test file for SpectrumWarsDisplayPlot class.
 */

#define BOOST_TEST_MODULE SpectrumWarsDisplayPlot_Test

#include "SpectrumWarsDisplayPlot.h"

#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/progress.hpp>
#include <boost/bind.hpp>
#include <qapplication.h>
#include <cstdlib>
#include <vector>
#include <boost/test/unit_test.hpp>

#define PI     3.14159265358979323846

using namespace std;

void threadMain1()
{
  int n=2048;
  SpectrumWarsDisplayPlot plot(n, n);
  plot.setTitle("Float");

  float step = 2.0*PI/n;
  float* data = new float[n*2];
  for(int i=0;i<n*2;i++)
    data[i] = sinf(step*i);

  plot.plotNewData(data, n);

  for(int i=0;i<n;i++)
  {
    plot.setLine1(i, 100);
    plot.setLine2(n-i,100);
    plot.plotNewData(data+i, n);
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));
  }
}

BOOST_AUTO_TEST_SUITE (SpectrumWarsDisplayPlot_Test)

BOOST_AUTO_TEST_CASE(SpectrumWarsDisplayPlot_Init_Test)
{
  int argc = 1;
  char* argv[] = { const_cast<char *>("SpectrumWarsDisplayPlot_Init_Test"), NULL };
  QApplication a(argc, argv);

  boost::scoped_ptr< boost::thread > thread1_;

  thread1_.reset( new boost::thread( &threadMain1 ) );

  qApp->exec();
  thread1_->join();
}

BOOST_AUTO_TEST_SUITE_END()
