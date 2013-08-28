/**
 * \file components/gpp/phy/DataSetCounter/DataSetCounterComponent.cpp
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
 * Implementation of an example PhyComponent to be used as a template
 * for new PhyComponents.
 */

#include "DataSetCounterComponent.h"

#include "irisapi/LibraryDefs.h"
#include "irisapi/Version.h"
#include "irisapi/TypeVectors.h"

using namespace std;

namespace iris
{
namespace phy
{

// export library symbols
IRIS_COMPONENT_EXPORTS(PhyComponent, DataSetCounterComponent)

DataSetCounterComponent::DataSetCounterComponent(std::string name)
  : TemplatePhyComponent<DataSetCounterComponent>(name,
                "datasetcounter",
                "A simple dataset counter - triggers an event for every dataset",
                "Paul Sutton",
                "0.1")
{
  registerParameter("issink", "Act as a sink (do not provide output)", "false",
      false, isSink_x);

  registerEvent(
      "havedataset",
      "Triggered when a dataset passes through",
      TypeInfo< uint32_t >::identifier);
}

void DataSetCounterComponent::registerPorts()
{
  vector<int> validTypes = convertToTypeIdVector<IrisDataTypes>();
  registerInputPort("input1", validTypes);
  registerOutputPort("output1", validTypes);
}

void DataSetCounterComponent::calculateOutputTypes(
    std::map<std::string,int>& inputTypes,
    std::map<std::string,int>& outputTypes)
{
  outputTypes["output1"] = inputTypes["input1"];
}

template <class Tin, class Tout>
void DataSetCounterComponentImpl<Tin,Tout>::initialize()
{}

template <class Tin, class Tout>
void DataSetCounterComponentImpl<Tin,Tout>::process()
{
  DataSet<Tinput1>* readDataSet = NULL;
  getInputDataSet("input1", readDataSet);
  std::size_t size = readDataSet->data.size();

  uint32_t x=0;
  activateEvent("havedataset", x);

  if(!isSink_x)
  {
    DataSet<Tinput1>* writeDataSet = NULL;
    getOutputDataSet("output1", writeDataSet, size);
    writeDataSet->data = readDataSet->data;
    writeDataSet->sampleRate = readDataSet->sampleRate;
    writeDataSet->timeStamp = readDataSet->timeStamp;
    releaseOutputDataSet("output1", writeDataSet);
  }

  releaseInputDataSet("input1", readDataSet);
}

template <class Tin, class Tout>
DataSetCounterComponentImpl<Tin,Tout>::~DataSetCounterComponentImpl()
{}


} // namesapce phy
} // namespace iris
