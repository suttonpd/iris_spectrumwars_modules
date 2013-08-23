#include "SpectrumWarsTxGui.h"
#include "SpectrumWarsTxGuiWrapper.h"

using namespace std;

SpectrumWarsTxGui::SpectrumWarsTxGui(SWTxGuiParams params, iris::Controller* master)
{
  params_ = params;
  master_ = master;
  plot_ = new SpectrumWarsTxGuiWrapper(params_, master_);
}

SpectrumWarsTxGui::~SpectrumWarsTxGui()
{
  if(plot_)
    delete plot_;
}
