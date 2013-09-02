#ifndef SPECTRUMWARSDISPLAYPLOTWIDGET_H
#define SPECTRUMWARSDISPLAYPLOTWIDGET_H

#include <qapplication.h>
#include <qwidget.h>
#include <complex>

class RealDataEvent;
class Psdplot;
class Spectrogramplot;
class QwtThermo;
class QPushButton;
class QLabel;

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
  void setLine1(double xLocation, double width);
  void setLine2(double xLocation, double width);
  void autoscale();

protected:
  virtual void timerEvent(QTimerEvent *event);

private:
  void plotData(RealDataEvent* e);
  Psdplot* p_;
  Spectrogramplot* s_;
  QPushButton* b_;
  QwtThermo* tLeft_;
  QLabel* lLeft_;
  QwtThermo* tRight_;
  QLabel* lRight_;

  double* data_;
  int numPoints_;
  int timerId_;
  bool haveNewData_;
  bool haveWinner_;
};

#endif // SPECTRUMWARSDISPLAYPLOTWIDGET_H
