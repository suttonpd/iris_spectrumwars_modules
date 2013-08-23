#include "SpectrumWarsRxGuiWrapper.h"

#include "SpectrumWarsRxGuiWidget.h"
#include "graphics/qt/common/Events.h"
#include <qapplication.h>
#include <QThread>

using namespace std;

SpectrumWarsRxGuiWrapper::SpectrumWarsRxGuiWrapper(SWRxGuiParams params, iris::Controller* master)
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

SpectrumWarsRxGuiWrapper::~SpectrumWarsRxGuiWrapper()
{
  if(destroyed_)
    emit destroyWidgetSignal();
  else
    emit destroyWidgetSignalBlocking();
}

void SpectrumWarsRxGuiWrapper::setFrequency(double frequency)
{
  if(master_)
  {
    iris::Event e;
    e.eventName = "guifrequency";
    e.data.push_back(frequency);
    master_->postEvent(e);
  }
}

void SpectrumWarsRxGuiWrapper::setBandwidth(double bandwidth)
{
  if(master_)
  {
    iris::Event e;
    e.eventName = "guibandwidth";
    e.data.push_back(bandwidth);
    master_->postEvent(e);
  }
}

void SpectrumWarsRxGuiWrapper::setGain(double gain)
{
  if(master_)
  {
    iris::Event e;
    e.eventName = "guigain";
    e.data.push_back(gain);
    master_->postEvent(e);
  }
}

void SpectrumWarsRxGuiWrapper::createWidgetSlot()
{
  widget_ = new SpectrumWarsRxGuiWidget(params_, this);
  destroyed_ = false;
  widget_->setAttribute(Qt::WA_DeleteOnClose, true);
  connect(widget_, SIGNAL( destroyed() ),
          this, SLOT( widgetDestroyed() ));
  widget_->show();
}

void SpectrumWarsRxGuiWrapper::destroyWidgetSlot()
{
  if(widget_)
    delete widget_;
  widget_ = NULL;
}

void SpectrumWarsRxGuiWrapper::widgetDestroyed()
{
  destroyed_ = true;
}
