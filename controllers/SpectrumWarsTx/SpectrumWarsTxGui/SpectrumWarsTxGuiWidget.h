#ifndef SPECTRUMWARSTXGUIWIDGET_H
#define SPECTRUMWARSTXGUIWIDGET_H

#include <qapplication.h>
#include <qwidget.h>

#include "SWTxGuiParams.h"
#include "SWTxGuiCallback.h"

class KnobSpin;

class SpectrumWarsTxGuiWidget
  : public QWidget
{
  Q_OBJECT

public:
  SpectrumWarsTxGuiWidget(SWTxGuiParams params,
                          SWTxGuiCallback* callback = NULL,
                          QWidget* parent = NULL);
  virtual ~SpectrumWarsTxGuiWidget();

public slots:
  void setFrequency(double frequency);
  void setBandwidth(double bandwidth);
  void setGain(double gain);

protected:

private:
  void updateGradient();
  KnobSpin* knob1_;
  KnobSpin* knob2_;
  KnobSpin* knob3_;
  SWTxGuiParams params_;
  SWTxGuiCallback* callback_;
  double frequency_;
  double bandwidth_;
  double gain_;
};

#endif // SPECTRUMWARSTXGUIWIDGET_H
