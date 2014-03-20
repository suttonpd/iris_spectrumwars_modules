/**
 * \file components/gpp/phy/Gate/GateComponent.cpp
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
#include "GateComponent.h"

using namespace std;

namespace iris
{
// export library symbols
IRIS_COMPONENT_EXPORTS(PhyComponent, GateComponent);

GateComponent::GateComponent(std::string name)
  :PhyComponent(name,
                "gatecomponent",
                "Passes data through when open, buffers it when closed.",
                "Paul Sutton",
                "0.1")
  ,nData_(0)
{
  registerParameter("open", "Is the gate open?",
                    "false", true, open_x);
  registerParameter("buffer", "If enabled, the gate will buffer data when closed",
                    "false", true, buffer_x);
  registerParameter("datalimit", "Number of data samples to let through before closing",
                    "900000", true, dataLimit_x);
  registerParameter("maxbufferedsets",
                    "The maximum buffer size in Bytes (0 means unlimited, which is not recommended)",
                    "1048576", true, maxBufferedSets_x);
}

void GateComponent::registerPorts()
{
  registerInputPort("input1", TypeInfo< complex<float> >::identifier);
  registerOutputPort("output1", TypeInfo< complex<float> >::identifier);
}

void GateComponent::calculateOutputTypes(
      std::map<std::string, int>& inputTypes,
      std::map<std::string, int>& outputTypes)
{
  outputTypes["output1"] = TypeInfo< complex<float> >::identifier;
}

void GateComponent::initialize()
{
  gateBuffer_.set_capacity(maxBufferedSets_x);
}

void GateComponent::process()
{
  //Get a DataSet from the input DataBuffer
  DataSet< complex<float> >* readDataSet = NULL;
  getInputDataSet("input1", readDataSet);
  size_t inputSize = readDataSet->data.size();

  if(open_x)  //Gate is open
  {
    nData_ += inputSize;
    if(nData_ > dataLimit_x)
    {
      //We've exceeded the limit - close the gate
      open_x = false;
      if(buffer_x)
        gateBuffer_.push_back(*readDataSet);
      releaseInputDataSet("input1", readDataSet);
      LOG(LINFO) << "Closed gate";
      nData_ = 0;
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
  else // Gate is closed
  {
    if(buffer_x)
      gateBuffer_.push_back(*readDataSet);
  }

  releaseInputDataSet("input1", readDataSet);
}

void GateComponent::flushBuffer()
{
  int flushCount = 0;

  while(gateBuffer_.size() > 0)
  {
    SetBuffer::iterator it = gateBuffer_.begin();
    size_t inputSize = it->data.size();
    nData_ += inputSize;
    if(nData_ > dataLimit_x)
    {
      //We've exceeded the limit - close the gate
      open_x = false;
      LOG(LINFO) << "Closed gate";
      LOG(LINFO) << "Flushed " << flushCount << " sets";
      nData_ = 0;
      return;
    }

    DataSet< complex<float> >* set = NULL;
    getOutputDataSet("output1", set, it->data.size());
    *set = *it;
    releaseOutputDataSet("output1", set);
    gateBuffer_.pop_front();
    flushCount++;
  }

  LOG(LINFO) << "Flushed " << flushCount << " sets";
}

} /* namespace iris */
