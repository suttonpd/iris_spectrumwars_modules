/**
 * \file components/gpp/phy/DataSetCounter/DataSetCounterComponent.h
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
 * A simple DataSet counter - triggers an event for every dataset it passes through.
 */

#ifndef PHY_DATASETCOUNTERCOMPONENT_H_
#define PHY_DATASETCOUNTERCOMPONENT_H_

#include "irisapi/TemplatePhyComponent.h"

namespace iris
{
namespace phy
{

template <class Tin, class Tout>
class DataSetCounterComponentImpl;

class DataSetCounterComponent
  : public TemplatePhyComponent<DataSetCounterComponent>
{
 public:
  typedef IrisDataTypes supportedInputTypes;
  typedef IrisDataTypes supportedOutputTypes;

  DataSetCounterComponent(std::string name);
  virtual ~DataSetCounterComponent(){}
  virtual void calculateOutputTypes(
      std::map<std::string, int>& inputTypes,
      std::map<std::string, int>& outputTypes);
  virtual void registerPorts();

  template <typename Tin, typename Tout>
  static PhyComponent* createInstance(const PhyComponent* comp)
  {
    return new DataSetCounterComponentImpl<Tin,Tout>(*comp);
  }

protected:
  bool isSink_x;          ///< Act as a sink? (Has no output).
};

template <class Tin, class Tout>
class DataSetCounterComponentImpl
  : public DataSetCounterComponent
{
 public:
  DataSetCounterComponentImpl(const PhyComponent& other)
   : DataSetCounterComponent(other.getName())
  {
    // assign all values from other to this
    assign(other);
  }
  ~DataSetCounterComponentImpl();
  virtual void initialize();
  virtual void process();

 private:
  typedef typename boost::mpl::front<Tin>::type Tinput1;  ///< The input port type
  typedef typename boost::mpl::front<Tout>::type Toutput1;///< The output port type
};

} // namespace phy
} // namespace iris

#endif // PHY_DATASETCOUNTERCOMPONENT_H_
