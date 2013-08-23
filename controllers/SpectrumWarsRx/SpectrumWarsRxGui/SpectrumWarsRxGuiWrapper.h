#ifndef SPECTRUMWARSRXGUIWRAPPER_H
#define SPECTRUMWARSRXGUIWRAPPER_H

#include <qapplication.h>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829
#include "irisapi/Controller.h"
#endif

#include "SWRxGuiParams.h"
#include "SWRxGuiCallback.h"

class SpectrumWarsRxGuiWidget;

class SpectrumWarsRxGuiWrapper
    : QObject, public SWRxGuiCallback
{
  Q_OBJECT

public:
  SpectrumWarsRxGuiWrapper(SWRxGuiParams params, iris::Controller* master=NULL);
  ~SpectrumWarsRxGuiWrapper();

  virtual void setFrequency(double frequency);
  virtual void setBandwidth(double bandwidth);
  virtual void setGain(double gain);

public slots:
  void createWidgetSlot();
  void destroyWidgetSlot();
  void widgetDestroyed();

signals:
  void createWidgetSignal();
  void destroyWidgetSignal();
  void destroyWidgetSignalBlocking();

private:
  SpectrumWarsRxGuiWidget* widget_;
  SWRxGuiParams params_;
  iris::Controller* master_;
  bool destroyed_;
};

#endif // SPECTRUMWARSRXGUIWRAPPER_H
