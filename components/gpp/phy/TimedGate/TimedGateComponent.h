/**
 * \file components/gpp/phy/TimedGate/TimedGateComponent.h
 * \version 1.0
 *
 * \section COPYRIGHT
 *
 * Copyright 2012 The Iris Project Developers. See the
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
 * A simple gate component that can buffer data when closed.
 * To open the gate, a controller sets the "openedtime"
 * parameter with a timestamp. The gate will then stay open
 * until openedtime + timeopen.
 */

#ifndef GATECOMPONENT_H_
#define GATECOMPONENT_H_

#include "irisapi/PhyComponent.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include <boost/circular_buffer.hpp>

namespace iris
{

/** A simple gate component that can buffer data when closed.
 *
 * The component then flushes any buffered data when opened. If the buffer
 * fills when closed, data will be lost.
 */
class TimedGateComponent
  : public PhyComponent
{
public:
  typedef boost::circular_buffer< DataSet< std::complex<float> > > SetBuffer;

  TimedGateComponent(std::string name);
  virtual void calculateOutputTypes(
      std::map<std::string, int>& inputTypes,
      std::map<std::string, int>& outputTypes);
  virtual void registerPorts();
  virtual void initialize();
  virtual void process();
  virtual void parameterHasChanged(std::string name);

private:
  void flushBuffer();
  bool hasClockExpired();

  uint64_t openedTime_x;          ///< Time this gate was opened (ms since Jan 1 2000)
  bool buffer_x;                  ///< Are we buffering data?
  uint32_t timeOpenMs_x;          ///< Time in ms to leave gate open
  uint32_t maxBufferedSets_x;     ///< Max number of sets to buffer

	// Buffer of DataSets
	SetBuffer gateBuffer_;

	bool isOpen_;
	boost::posix_time::ptime timeClosed_;
	boost::posix_time::ptime refTime_;

};

} /* namespace iris */

#endif /* GATECOMPONENT_H_ */
