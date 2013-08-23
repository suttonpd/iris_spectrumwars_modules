#ifndef SPECTRUMWARSRXGUIWIDGET_H
#define SPECTRUMWARSRXGUIWIDGET_H

#include <qapplication.h>
#include <qwidget.h>

#include "SWRxGuiParams.h"
#include "SWRxGuiCallback.h"

class KnobSpin;

class SpectrumWarsRxGuiWidget
  : public QWidget
{
  Q_OBJECT

public:
  SpectrumWarsRxGuiWidget(SWRxGuiParams params,
                          SWRxGuiCallback* callback = NULL,
                          QWidget* parent = NULL);
  virtual ~SpectrumWarsRxGuiWidget();

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
  SWRxGuiParams params_;
  SWRxGuiCallback* callback_;
  double frequency_;
  double bandwidth_;
  double gain_;
};

#endif // SPECTRUMWARSRXGUIWIDGET_H
