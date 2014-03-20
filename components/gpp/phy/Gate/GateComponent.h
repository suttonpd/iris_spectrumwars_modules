/**
 * \file components/gpp/phy/Gate/GateComponent.h
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
#include <boost/circular_buffer.hpp>

namespace iris
{

/** A simple gate component that can buffer data when closed.
 *
 * The component then flushes any buffered data when opened. If the buffer
 * fills when closed, data will be lost.
 */
class GateComponent
  : public PhyComponent
{
public:
  typedef boost::circular_buffer< DataSet< std::complex<float> > > SetBuffer;

  GateComponent(std::string name);
  virtual void calculateOutputTypes(
      std::map<std::string, int>& inputTypes,
      std::map<std::string, int>& outputTypes);
  virtual void registerPorts();
  virtual void initialize();
  virtual void process();

private:
  void flushBuffer();
  bool dataLimitExceeded();

  bool open_x;                    ///< Is the gate open?
  bool buffer_x;                  ///< Are we buffering data?
  uint64_t dataLimit_x;           ///< Number of data samples to let through before closing
  uint32_t maxBufferedSets_x;     ///< Max number of sets to buffer

	// Buffer of DataSets
	SetBuffer gateBuffer_;
  uint64_t nData_;

};

} /* namespace iris */

#endif /* GATECOMPONENT_H_ */
