#ifndef SPECTRUMWARSDISPLAYPLOTWRAPPER_H
#define SPECTRUMWARSDISPLAYPLOTWRAPPER_H

#include <qapplication.h>

class SpectrumWarsDisplayPlotWidget;

class SpectrumWarsDisplayPlotWrapper
    : QObject
{
  Q_OBJECT

public:
  SpectrumWarsDisplayPlotWrapper(int numDataPoints, int numRows);
  ~SpectrumWarsDisplayPlotWrapper();

  void plotNewData(float* data, int numPoints);
  void plotNewData(double* data, int numPoints);
  void setTitle(std::string title);
  void setPlotXAxisRange(double xMin, double xMax);
  void setPlotXAxisScale(double xMin, double xMax);
  void setSpectrogramXAxisRange(double xMin, double xMax);
  void setPlotYAxisScale(double yMin, double yMax);
  void setSpectrogramYAxisRange(double yMin, double yMax);
  void setSpectrogramZAxisScale(double zMin, double zMax);
  void setLevelLeft(double level);
  void setLevelRight(double level);

public slots:
  void createWidgetSlot(int numDataPoints, int numRows);
  void destroyWidgetSlot();
  void widgetDestroyed();

signals:
  void createWidgetSignal(int numDataPoints, int numRows);
  void destroyWidgetSignal();
  void destroyWidgetSignalBlocking();
  void setWidgetTitle(QString title);
  void setWidgetPXAxisRange(double xMin, double xMax);
  void setWidgetPXAxisScale(double xMin, double xMax);
  void setWidgetSXAxisRange(double xMin, double xMax);
  void setWidgetPYAxisScale(double yMin, double yMax);
  void setWidgetSYAxisRange(double yMin, double yMax);
  void setWidgetSZAxisScale(double zMin, double zMax);
  void setWidgetLevelLeft(double level);
  void setWidgetLevelRight(double level);

private:
  SpectrumWarsDisplayPlotWidget* widget_;
  bool destroyed_;
};

#endif // SPECTRUMWARSDISPLAYPLOTWRAPPER_H
