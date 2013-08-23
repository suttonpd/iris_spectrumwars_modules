#include "SpectrumWarsTxGuiWrapper.h"

#include "SpectrumWarsTxGuiWidget.h"
#include "graphics/qt/common/Events.h"
#include <qapplication.h>
#include <QThread>

using namespace std;

SpectrumWarsTxGuiWrapper::SpectrumWarsTxGuiWrapper(SWTxGuiParams params, iris::Controller* master)
    :widget_(NULL)
    ,destroyed_(true)
{
  params_ = params;
  master_ = master;

  if(QCoreApplication::instance() == NULL)
    return; //TODO: throw exception here in Iris
  if(QCoreApplication::instance()->thread() == QThread::currentThread())
  {
    connect( this, SIGNAL( createWidgetSignal() ),
             this, SLOT(createWidgetSlot()) );
    connect( this, SIGNAL( destroyWidgetSignal() ),
             this, SLOT(destroyWidgetSlot()) );
    connect( this, SIGNAL( destroyWidgetSignalBlocking() ),
             this, SLOT(destroyWidgetSlot()) );
  }
  else
  {
    connect( this, SIGNAL( createWidgetSignal() ),
             this, SLOT(createWidgetSlot()),
             Qt::BlockingQueuedConnection );
    connect( this, SIGNAL( destroyWidgetSignal() ),
             this, SLOT(destroyWidgetSlot()) );
    connect( this, SIGNAL( destroyWidgetSignalBlocking() ),
             this, SLOT(destroyWidgetSlot()),
             Qt::BlockingQueuedConnection );
    moveToThread(QCoreApplication::instance()->thread());
  }
  emit createWidgetSignal();
}

SpectrumWarsTxGuiWrapper::~SpectrumWarsTxGuiWrapper()
{
  if(destroyed_)
    emit destroyWidgetSignal();
  else
    emit destroyWidgetSignalBlocking();
}

void SpectrumWarsTxGuiWrapper::setFrequency(double frequency)
{
  if(master_)
  {
    iris::Event e;
    e.eventName = "guifrequency";
    e.data.push_back(frequency);
    master_->postEvent(e);
  }
}

void SpectrumWarsTxGuiWrapper::setBandwidth(double bandwidth)
{
  if(master_)
  {
    iris::Event e;
    e.eventName = "guibandwidth";
    e.data.push_back(bandwidth);
    master_->postEvent(e);
  }
}

void SpectrumWarsTxGuiWrapper::setGain(double gain)
{
  if(master_)
  {
    iris::Event e;
    e.eventName = "guigain";
    e.data.push_back(gain);
    master_->postEvent(e);
  }
}

void SpectrumWarsTxGuiWrapper::createWidgetSlot()
{
  widget_ = new SpectrumWarsTxGuiWidget(params_, this);
  destroyed_ = false;
  widget_->setAttribute(Qt::WA_DeleteOnClose, true);
  connect(widget_, SIGNAL( destroyed() ),
          this, SLOT( widgetDestroyed() ));
  widget_->show();
}

void SpectrumWarsTxGuiWrapper::destroyWidgetSlot()
{
  if(widget_)
    delete widget_;
  widget_ = NULL;
}

void SpectrumWarsTxGuiWrapper::widgetDestroyed()
{
  destroyed_ = true;
}
