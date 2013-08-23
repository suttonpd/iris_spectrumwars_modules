#ifndef SPECTRUMWARSRXGUI_H
#define SPECTRUMWARSRXGUI_H

#include <string>
#include "irisapi/Controller.h"

#include "SWRxGuiParams.h"

class SpectrumWarsRxGuiWrapper;

class SpectrumWarsRxGui
{
public:
  SpectrumWarsRxGui(SWRxGuiParams params, iris::Controller* master=NULL);
  ~SpectrumWarsRxGui();

private:
  SpectrumWarsRxGuiWrapper* plot_;
  SWRxGuiParams params_;
  iris::Controller* master_;
};

#endif // SPECTRUMWARSRXGUI_H
