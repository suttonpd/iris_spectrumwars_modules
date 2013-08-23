#ifndef SPECTRUMWARSDISPLAYPLOTWIDGET_H
#define SPECTRUMWARSDISPLAYPLOTWIDGET_H

#include <qapplication.h>
#include <qwidget.h>
#include <complex>

class RealDataEvent;
class Lineplot;
class Spectrogramplot;
class QwtThermo;
class QPushButton;

class SpectrumWarsDisplayPlotWidget
  : public QWidget
{
  Q_OBJECT

public:
  SpectrumWarsDisplayPlotWidget(int numDataPoints, int numRows, QWidget* parent = 0);
  virtual ~SpectrumWarsDisplayPlotWidget();

public slots:
  void customEvent( QEvent * e );
  void setWidgetTitle(QString title);
  void setPlotXAxisRange(double xMin, double xMax);
  void setPlotXAxisScale(double xMin, double xMax);
  void setSpectrogramXAxisRange(double xMin, double xMax);
  void setPlotYAxisScale(double yMin, double yMax);
  void setSpectrogramYAxisRange(double yMin, double yMax);
  void setSpectrogramZAxisScale(double zMin, double zMax);
  void setLevelLeft(double level);
  void setLevelRight(double level);
  void autoscale();

protected:
  virtual void timerEvent(QTimerEvent *event);

private:
  void plotData(RealDataEvent* e);
  Lineplot* p_;
  Spectrogramplot* s_;
  QPushButton* b_;
  QwtThermo* tLeft_;
  QwtThermo* tRight_;

  double* data_;
  int numPoints_;
  int timerId_;
  bool haveNewData_;
};

#endif // SPECTRUMWARSDISPLAYPLOTWIDGET_H
