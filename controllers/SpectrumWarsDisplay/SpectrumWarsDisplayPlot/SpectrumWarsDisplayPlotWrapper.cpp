#include "SpectrumWarsDisplayPlotWrapper.h"

#include "SpectrumWarsDisplayPlotWidget.h"
#include "graphics/qt/common/Events.h"
#include <qapplication.h>
#include <QThread>

using namespace std;

SpectrumWarsDisplayPlotWrapper::SpectrumWarsDisplayPlotWrapper(int numDataPoints, int numRows)
    :widget_(NULL)
    ,destroyed_(true)
{
  if(QCoreApplication::instance() == NULL)
    return; //TODO: throw exception here in Iris
  if(QCoreApplication::instance()->thread() == QThread::currentThread())
  {
    connect( this, SIGNAL( createWidgetSignal(int, int) ),
             this, SLOT(createWidgetSlot(int, int)) );
    connect( this, SIGNAL( destroyWidgetSignal() ),
             this, SLOT(destroyWidgetSlot()) );
    connect( this, SIGNAL( destroyWidgetSignalBlocking() ),
             this, SLOT(destroyWidgetSlot()) );
  }
  else
  {
    connect( this, SIGNAL( createWidgetSignal(int, int) ),
             this, SLOT(createWidgetSlot(int, int)),
             Qt::BlockingQueuedConnection );
    connect( this, SIGNAL( destroyWidgetSignal() ),
             this, SLOT(destroyWidgetSlot()));
    connect( this, SIGNAL( destroyWidgetSignalBlocking() ),
             this, SLOT(destroyWidgetSlot()),
             Qt::BlockingQueuedConnection );
    moveToThread(QCoreApplication::instance()->thread());
  }
  emit createWidgetSignal(numDataPoints, numRows);
}

SpectrumWarsDisplayPlotWrapper::~SpectrumWarsDisplayPlotWrapper()
{
  if(destroyed_)
    emit destroyWidgetSignal();
  else
    emit destroyWidgetSignalBlocking();
}

void SpectrumWarsDisplayPlotWrapper::createWidgetSlot(int numDataPoints, int numRows)
{
  widget_ = new SpectrumWarsDisplayPlotWidget(numDataPoints, numRows);
  destroyed_ = false;
  widget_->setAttribute(Qt::WA_DeleteOnClose, true);
  connect(widget_, SIGNAL( destroyed() ),
          this, SLOT( widgetDestroyed() ));
  connect(this, SIGNAL(setWidgetTitle(QString)),
          widget_, SLOT(setWidgetTitle(QString)));
  connect(this, SIGNAL(setWidgetPXAxisRange(double, double)),
          widget_, SLOT(setPlotXAxisRange(double, double)));
  connect(this, SIGNAL(setWidgetPXAxisScale(double, double)),
          widget_, SLOT(setPlotXAxisScale(double, double)));
  connect(this, SIGNAL(setWidgetSXAxisRange(double, double)),
          widget_, SLOT(setSpectrogramXAxisRange(double, double)));
  connect(this, SIGNAL(setWidgetPYAxisScale(double, double)),
          widget_, SLOT(setPlotYAxisScale(double, double)));
  connect(this, SIGNAL(setWidgetSYAxisRange(double, double)),
          widget_, SLOT(setSpectrogramYAxisRange(double, double)));
  connect(this, SIGNAL(setWidgetSZAxisScale(double, double)),
          widget_, SLOT(setSpectrogramZAxisScale(double, double)));
  connect(this, SIGNAL(setWidgetLevelLeft(double)),
          widget_, SLOT(setLevelLeft(double)));
  connect(this, SIGNAL(setWidgetLevelRight(double)),
          widget_, SLOT(setLevelRight(double)));

  widget_->resize( 800, 600 );
  widget_->show();
  widget_->moveToThread(QApplication::instance()->thread());
}

void SpectrumWarsDisplayPlotWrapper::destroyWidgetSlot()
{
  if(widget_)
    delete widget_;
  widget_ = NULL;
}

void SpectrumWarsDisplayPlotWrapper::widgetDestroyed()
{
  destroyed_ = true;
}

void SpectrumWarsDisplayPlotWrapper::plotNewData(float* data, int numPoints)
{
  if(destroyed_)
    return;
  qApp->postEvent(widget_, new RealDataEvent(data, numPoints));
}

void SpectrumWarsDisplayPlotWrapper::plotNewData(double* data, int numPoints)
{
  if(destroyed_)
    return;
  qApp->postEvent(widget_, new RealDataEvent(data, numPoints));
}


void SpectrumWarsDisplayPlotWrapper::setTitle(std::string title)
{
  if(destroyed_)
    return;
  QString str = QString::fromUtf8(title.c_str());
  emit setWidgetTitle(str);
}

void SpectrumWarsDisplayPlotWrapper::setPlotXAxisRange(double xMin, double xMax)
{
  if(destroyed_)
    return;
  emit setWidgetPXAxisRange(xMin, xMax);
}

void SpectrumWarsDisplayPlotWrapper::setPlotXAxisScale(double xMin, double xMax)
{
  if(destroyed_)
    return;
  emit setWidgetPXAxisScale(xMin, xMax);
}

void SpectrumWarsDisplayPlotWrapper::setSpectrogramXAxisRange(double xMin, double xMax)
{
  if(destroyed_)
    return;
  emit setWidgetSXAxisRange(xMin, xMax);
}

void SpectrumWarsDisplayPlotWrapper::setPlotYAxisScale(double yMin, double yMax)
{
  if(destroyed_)
    return;
  emit setWidgetPYAxisScale(yMin, yMax);
}

void SpectrumWarsDisplayPlotWrapper::setSpectrogramYAxisRange(double yMin, double yMax)
{
  if(destroyed_)
    return;
  emit setWidgetSYAxisRange(yMin, yMax);
}

void SpectrumWarsDisplayPlotWrapper::setSpectrogramZAxisScale(double zMin, double zMax)
{
  if(destroyed_)
    return;
  emit setWidgetSZAxisScale(zMin, zMax);
}

void SpectrumWarsDisplayPlotWrapper::setLevelLeft(double level)
{
  if(destroyed_)
    return;
  emit setWidgetLevelLeft(level);
}

void SpectrumWarsDisplayPlotWrapper::setLevelRight(double level)
{
  if(destroyed_)
    return;
  emit setWidgetLevelRight(level);
}
