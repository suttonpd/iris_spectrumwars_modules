/**
 * \file components/gpp/phy/TimedGate/TimedGateComponent.cpp
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
 * Implementation of a gate component which buffers data when closed.
 */

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "TimedGateComponent.h"

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

namespace iris
{
// export library symbols
IRIS_COMPONENT_EXPORTS(PhyComponent, TimedGateComponent);

TimedGateComponent::TimedGateComponent(std::string name)
  :PhyComponent(name,
                "timedgatecomponent",
                "Passes data through when open, optionally buffers it when closed.",
                "Paul Sutton",
                "0.1")
  ,isOpen_(false)
  ,refTime_(date(2000,Jan,1))
{
  registerParameter("openedtime", "Time this gate was opened (ms since Jan 1st 2000)",
                    "0", true, openedTime_x);
  registerParameter("buffer", "If enabled, the gate will buffer data when closed",
                    "false", true, buffer_x);
  registerParameter("timeopenms", "Time in ms to keep gate open",
                    "900", true, timeOpenMs_x);
  registerParameter("maxbufferedsets",
                    "The maximum buffer size in Bytes (0 means unlimited, which is not recommended)",
                    "1048576", true, maxBufferedSets_x);
}

void TimedGateComponent::registerPorts()
{
  registerInputPort("input1", TypeInfo< complex<float> >::identifier);
  registerOutputPort("output1", TypeInfo< complex<float> >::identifier);
}

void TimedGateComponent::calculateOutputTypes(
      std::map<std::string, int>& inputTypes,
      std::map<std::string, int>& outputTypes)
{
  outputTypes["output1"] = TypeInfo< complex<float> >::identifier;
}

void TimedGateComponent::initialize()
{
  gateBuffer_.set_capacity(maxBufferedSets_x);
}

void TimedGateComponent::parameterHasChanged(std::string name)
{
  if(name == "openedtime" && !hasClockExpired())
    isOpen_ = true;
}

void TimedGateComponent::process()
{
  //Get a DataSet from the input DataBuffer
  DataSet< complex<float> >* readDataSet = NULL;
  getInputDataSet("input1", readDataSet);
  size_t inputSize = readDataSet->data.size();

  if(isOpen_)  //TimedGate is open
  {
    if(hasClockExpired())
    {
      //We've timed out - buffer the data and close the gate
      timeClosed_ = microsec_clock::local_time();
      isOpen_ = false;
      if(buffer_x)
        gateBuffer_.push_back(*readDataSet);
      releaseInputDataSet("input1", readDataSet);
      LOG(LINFO) << "Closed gate";
      return;
    }

    if(gateBuffer_.size() > 0)
      flushBuffer();

    //Copy input to output
    DataSet< complex<float> >* writeDataSet = NULL;
    getOutputDataSet("output1", writeDataSet, inputSize);
    *writeDataSet = *readDataSet;
    releaseOutputDataSet("output1", writeDataSet);

  }
  else // TimedGate is closed
  {
    if(buffer_x)
      gateBuffer_.push_back(*readDataSet);
  }

  releaseInputDataSet("input1", readDataSet);
}

void TimedGateComponent::flushBuffer()
{
  int flushCount = 0;

  while(gateBuffer_.size() > 0)
  {
    if(hasClockExpired())
    {
      //We've timed out - close the gate
      timeClosed_ = microsec_clock::local_time();
      isOpen_ = false;
      LOG(LINFO) << "Closed gate";
      LOG(LINFO) << "Flushed " << flushCount << " sets";
      return;
    }
    SetBuffer::iterator it = gateBuffer_.begin();
    DataSet< complex<float> >* set = NULL;
    getOutputDataSet("output1", set, it->data.size());
    *set = *it;
    releaseOutputDataSet("output1", set);
    gateBuffer_.pop_front();
    flushCount++;
  }

  LOG(LINFO) << "Flushed " << flushCount << " sets";
}

bool TimedGateComponent::hasClockExpired()
{
  uint64_t nowStamp = (microsec_clock::local_time() - refTime_).total_milliseconds();
  uint64_t diff = nowStamp - openedTime_x;
  return (diff > timeOpenMs_x);
}

} /* namespace iris */
