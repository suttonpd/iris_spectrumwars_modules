#ifndef SPECTRUMWARSDISPLAYPLOT_H
#define SPECTRUMWARSDISPLAYPLOT_H

#include <string>

class SpectrumWarsDisplayPlotWrapper;

class SpectrumWarsDisplayPlot
{
public:
  SpectrumWarsDisplayPlot(int numDataPoints, int numRows);
  ~SpectrumWarsDisplayPlot();

  template<class Iterator>
  void plotNewData(Iterator begin, Iterator end);
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
  void setLine1(double xLocation, double width);
  void setLine2(double xLocation, double width);
  void setvLine1(double xLocation);

private:
  SpectrumWarsDisplayPlotWrapper* plot_;
};

template<class Iterator>
void SpectrumWarsDisplayPlot::plotNewData(Iterator begin, Iterator end)
{
  int numPoints = end-begin;
  double* data = new double[numPoints];

  for(int i=0;begin!=end;begin++,i++)
  {
    data[i] = *begin;
  }

  plotNewData(data, numPoints);
  delete[] data;
}

#endif // SPECTRUMWARSDISPLAYPLOT_H
