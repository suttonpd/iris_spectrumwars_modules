#ifndef SPECTRUMWARSTXGUI_H
#define SPECTRUMWARSTXGUI_H

#include <string>
#include "irisapi/Controller.h"

#include "SWTxGuiParams.h"

class SpectrumWarsTxGuiWrapper;

class SpectrumWarsTxGui
{
public:
  SpectrumWarsTxGui(SWTxGuiParams params, iris::Controller* master=NULL);
  ~SpectrumWarsTxGui();

private:
  SpectrumWarsTxGuiWrapper* plot_;
  SWTxGuiParams params_;
  iris::Controller* master_;
};

#endif // SPECTRUMWARSTXGUI_H
