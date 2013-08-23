#include "SpectrumWarsDisplayPlot.h"
#include "SpectrumWarsDisplayPlotWrapper.h"

using namespace std;

SpectrumWarsDisplayPlot::SpectrumWarsDisplayPlot(int numDataPoints, int numRows)
{
  plot_ = new SpectrumWarsDisplayPlotWrapper(numDataPoints, numRows);
}

SpectrumWarsDisplayPlot::~SpectrumWarsDisplayPlot()
{
  delete plot_;
}

void SpectrumWarsDisplayPlot::plotNewData(float* data, int numPoints)
{
  plot_->plotNewData(data, numPoints);
}

void SpectrumWarsDisplayPlot::plotNewData(double* data, int numPoints)
{
  plot_->plotNewData(data, numPoints);
}

void SpectrumWarsDisplayPlot::setTitle(std::string title)
{
  plot_->setTitle(title);
}

void SpectrumWarsDisplayPlot::setPlotXAxisRange(double xMin, double xMax)
{
  plot_->setPlotXAxisRange(xMin, xMax);
}

void SpectrumWarsDisplayPlot::setPlotXAxisScale(double xMin, double xMax)
{
  plot_->setPlotXAxisScale(xMin, xMax);
}

void SpectrumWarsDisplayPlot::setSpectrogramXAxisRange(double xMin, double xMax)
{
  plot_->setSpectrogramXAxisRange(xMin, xMax);
}

void SpectrumWarsDisplayPlot::setPlotYAxisScale(double yMin, double yMax)
{
  plot_->setPlotYAxisScale(yMin, yMax);
}

void SpectrumWarsDisplayPlot::setSpectrogramYAxisRange(double yMin, double yMax)
{
  plot_->setSpectrogramYAxisRange(yMin, yMax);
}

void SpectrumWarsDisplayPlot::setSpectrogramZAxisScale(double zMin, double zMax)
{
  plot_->setSpectrogramZAxisScale(zMin, zMax);
}

void SpectrumWarsDisplayPlot::setLevelLeft(double level)
{
  plot_->setLevelLeft(level);
}

void SpectrumWarsDisplayPlot::setLevelRight(double level)
{
  plot_->setLevelRight(level);
}
