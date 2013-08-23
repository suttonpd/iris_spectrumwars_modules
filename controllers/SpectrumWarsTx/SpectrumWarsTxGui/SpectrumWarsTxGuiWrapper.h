#ifndef SPECTRUMWARSTXGUIWRAPPER_H
#define SPECTRUMWARSTXGUIWRAPPER_H

#include <qapplication.h>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829
#include "irisapi/Controller.h"
#endif

#include "SWTxGuiParams.h"
#include "SWTxGuiCallback.h"

class SpectrumWarsTxGuiWidget;

class SpectrumWarsTxGuiWrapper
    : QObject, public SWTxGuiCallback
{
  Q_OBJECT

public:
  SpectrumWarsTxGuiWrapper(SWTxGuiParams params, iris::Controller* master=NULL);
  ~SpectrumWarsTxGuiWrapper();

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
  SpectrumWarsTxGuiWidget* widget_;
  SWTxGuiParams params_;
  iris::Controller* master_;
  bool destroyed_;
};

#endif // SPECTRUMWARSTXGUIWRAPPER_H
