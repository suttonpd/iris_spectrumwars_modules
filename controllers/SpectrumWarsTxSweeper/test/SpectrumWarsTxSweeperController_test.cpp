/**
 * \file controllers/SpectrumWarsTx/test/SpectrumWarsTxSweeperController_test.cpp
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
 * Main test file for SpectrumWarsTxSweeper controller.
 */

#define BOOST_TEST_MODULE SpectrumWarsTxSweeperController_Test

#include <boost/test/unit_test.hpp>
#include "../SpectrumWarsTxSweeperController.h"

using namespace std;
using namespace iris;

BOOST_AUTO_TEST_SUITE (SpectrumWarsTxSweeperController_Test)

BOOST_AUTO_TEST_CASE(SpectrumWarsTxSweeperController_Basic_Test)
{
  BOOST_REQUIRE_NO_THROW(SpectrumWarsTxSweeperController c);
}

BOOST_AUTO_TEST_CASE(SpectrumWarsTxSweeperController_Parm_Test)
{
  SpectrumWarsTxSweeperController c;
  BOOST_CHECK(c.getParameterDefaultValue("intervalms") == "5000");
  BOOST_CHECK(c.getParameterDefaultValue("minfrequency") == "2400000000");
  BOOST_CHECK(c.getParameterDefaultValue("maxfrequency") == "2405000000");
  BOOST_CHECK(c.getParameterDefaultValue("bandwidth") == "250000");
  BOOST_CHECK(c.getParameterDefaultValue("gain") == "10");
  BOOST_CHECK(c.getParameterDefaultValue("frontendtx") == "usrptx1");
  BOOST_CHECK(c.getParameterDefaultValue("frontendtxengine") == "phyengine1");
}

BOOST_AUTO_TEST_CASE(SpectrumWarsTxSweeperController_Init_Test)
{
  SpectrumWarsTxSweeperController c;
  c.initialize();
  c.destroy();
}

void threadMain1()
{
  // Load and start - controller runs own thread
  SpectrumWarsTxSweeperController c;
  c.load();
  c.start();
  boost::this_thread::sleep(boost::posix_time::seconds(20));
  c.stop();
  c.unload();
}

BOOST_AUTO_TEST_CASE(SpectrumWarsTxSweeperController_Process_Test)
{
  boost::scoped_ptr< boost::thread > thread1_;
  thread1_.reset( new boost::thread( &threadMain1 ) );
  thread1_->join();
}

BOOST_AUTO_TEST_SUITE_END()
