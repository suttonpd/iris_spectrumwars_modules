#include "SpectrumWarsRxGui.h"
#include "SpectrumWarsRxGuiWrapper.h"

using namespace std;

SpectrumWarsRxGui::SpectrumWarsRxGui(SWRxGuiParams params, iris::Controller* master)
{
  params_ = params;
  master_ = master;
  plot_ = new SpectrumWarsRxGuiWrapper(params_, master_);
}

SpectrumWarsRxGui::~SpectrumWarsRxGui()
{
  if(plot_)
    delete plot_;
}
