#include "SpectrumWarsDisplayPlotWidget.h"
#include "WinnerLabel.h"
#include "Psdplot.h"
#include "graphics/qt/common/Spectrogramplot.h"
#include "graphics/qt/common/Events.h"

#include <qwt/qwt_thermo.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <algorithm>
#include <boost/lambda/lambda.hpp>

using namespace std;
namespace bl = boost::lambda;


SpectrumWarsDisplayPlotWidget::SpectrumWarsDisplayPlotWidget(int numDataPoints, int numRows, QWidget *parent)
  :QWidget(parent)
{
  p_ = new Psdplot();
  p_->setAxisAutoScale(QwtPlot::yLeft, false);
  s_ = new Spectrogramplot(numDataPoints, numRows);
  b_ = new QPushButton("Autoscale");
  connect(b_, SIGNAL(clicked()), this, SLOT(autoscale()));

  tLeft_ = new QwtThermo();
  tLeft_->setPipeWidth(100);
  tLeft_->setMaxValue(100);
  tLeft_->setFillBrush(QBrush(Qt::red));
  QString a("<span style=\"font-size:16pt; font-weight:600;\">Team A</span>");
  lLeft_ = new QLabel(a);
  lLeft_->setTextFormat(Qt::RichText);
  lLeft_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  tRight_ = new QwtThermo();
  tRight_->setPipeWidth(100);
  tRight_->setMaxValue(100);
  tRight_->setFillBrush(QBrush(Qt::blue));
  QString b("<span style=\"font-size:16pt; font-weight:600;\">Team B</span>");
  lRight_ = new QLabel(b);
  lRight_->setTextFormat(Qt::RichText);
  lRight_->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

  QVBoxLayout* vLayoutLeft = new QVBoxLayout();
  vLayoutLeft->addWidget(lLeft_);
  vLayoutLeft->addWidget(tLeft_);

  QVBoxLayout* vLayoutRight = new QVBoxLayout();
  vLayoutRight->addWidget(lRight_);
  vLayoutRight->addWidget(tRight_);

  QVBoxLayout* vLayout1 = new QVBoxLayout();
  vLayout1->addWidget(p_,1);
  vLayout1->addWidget(s_,3);
  vLayout1->addWidget(b_);

  QHBoxLayout* hLayout1 = new QHBoxLayout(this);
  hLayout1->addLayout(vLayoutLeft);
  hLayout1->addLayout(vLayout1);
  hLayout1->addLayout(vLayoutRight);

  numPoints_ = numDataPoints;
  data_ = new double[numPoints_];
  timerId_ = startTimer(10);
  haveNewData_ = false;
}

SpectrumWarsDisplayPlotWidget::~SpectrumWarsDisplayPlotWidget()
{
  delete p_;
  delete s_;
  delete tLeft_;
  delete tRight_;
}

void SpectrumWarsDisplayPlotWidget::customEvent( QEvent * e )
{
  if(e->type() == RealDataEvent::type)
  {
    RealDataEvent* dataEvent = (RealDataEvent*)e;
    plotData(dataEvent);
  }
}

void SpectrumWarsDisplayPlotWidget::setWidgetTitle(QString title)
{
  setWindowTitle(title);
}

void SpectrumWarsDisplayPlotWidget::setPlotXAxisRange(double xMin, double xMax)
{
  p_->setXAxisRange(xMin, xMax);
}

void SpectrumWarsDisplayPlotWidget::setPlotXAxisScale(double xMin, double xMax)
{
  p_->setAxisScale(QwtPlot::xBottom, xMin, xMax);
}

void SpectrumWarsDisplayPlotWidget::setSpectrogramXAxisRange(double xMin, double xMax)
{
  s_->setXAxisRange(xMin, xMax);
}

void SpectrumWarsDisplayPlotWidget::setPlotYAxisScale(double yMin, double yMax)
{
  p_->setAxisScale(QwtPlot::yLeft, yMin, yMax);
}

void SpectrumWarsDisplayPlotWidget::setSpectrogramYAxisRange(double yMin, double yMax)
{
  s_->setYAxisRange(yMin, yMax);
}

void SpectrumWarsDisplayPlotWidget::setSpectrogramZAxisScale(double zMin, double zMax)
{
  s_->setZAxisScale(zMin, zMax);
}

void SpectrumWarsDisplayPlotWidget::setLevelLeft(double level)
{
  tLeft_->setValue(level);
  if(tLeft_->value() == tLeft_->maxValue())
    WinnerLabel* label = new WinnerLabel("Team A Wins!");
}

void SpectrumWarsDisplayPlotWidget::setLevelRight(double level)
{
  tRight_->setValue(level);
  if(tRight_->value() == tRight_->maxValue())
    WinnerLabel* label = new WinnerLabel("Team B Wins!");
}

void SpectrumWarsDisplayPlotWidget::setLine1(double xLocation, double width)
{
  p_->setLine1(xLocation, width);
}

void SpectrumWarsDisplayPlotWidget::setLine2(double xLocation, double width)
{
  p_->setLine2(xLocation, width);
}

void SpectrumWarsDisplayPlotWidget::autoscale()
{
  double min = s_->min();
  double max = s_->max();
  s_->setZAxisScale(min, max);
  p_->setAxisAutoScale(QwtPlot::yLeft, false);
  p_->setAxisScale(QwtPlot::yLeft, min, max);
}

void SpectrumWarsDisplayPlotWidget::timerEvent(QTimerEvent *event)
{
  if(event->timerId() == timerId_)
  {
    if(haveNewData_)
    {
      p_->replot();
      s_->replot();
      haveNewData_ = false;
    }
    return;
  }
  QWidget::timerEvent(event);
}

void SpectrumWarsDisplayPlotWidget::plotData(RealDataEvent* e)
{
  if(e->numPoints_ != numPoints_)
  {
    numPoints_ = e->numPoints_;
    delete [] data_;
    data_ = new double[numPoints_];
  }

  memcpy(data_, e->dataPoints_, numPoints_*sizeof(double));

  p_->setData(data_, numPoints_);
  s_->appendData(data_, numPoints_);
  haveNewData_ = true;
}
