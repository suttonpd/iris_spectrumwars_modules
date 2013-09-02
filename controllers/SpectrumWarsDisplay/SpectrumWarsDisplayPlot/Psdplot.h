/**
 * \file lib/generic/graphics/qt/common/Psdplot.h
 * \version 1.0
 *
 * \section COPYRIGHT
 *
 * Copyright 2012-2013 The Iris Project Developers. See the
 * COPYRIGHT file at the top-level directory of this distribution
 * and at http://www.softwareradiosystems.com/iris/copyright.html.
 *
 * \section LICENSE
 *
 * This file is part of the Iris Project.
 *
 * Iris is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * Iris is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * A copy of the GNU Lesser General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 * \section DESCRIPTION
 *
 * A simple line plotted using a QwtPlot.
 */

#ifndef PSDPLOT_H
#define PSDPLOT_H

#include <qapplication.h>
#include <qwt_plot.h>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_engine.h>
#include <qwt_scale_widget.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>

class Psdplot
  : public QwtPlot
{
  Q_OBJECT

public:
  Psdplot(QWidget* parent = 0);
  virtual ~Psdplot();

  void setData(double* data, int n);
  void setLine1(double xLocation, double width);
  void setLine2(double xLocation, double width);
  void setXAxisRange(double xMin, double xMax);
  void resetZoom();

public slots:
  void linkScales();

private:
  QwtPlotCurve* curve_;
  QwtPlotCurve* line1_;
  QwtPlotCurve* line2_;

  QwtPlotPanner* panner_;
  QwtPlotZoomer* zoomer_;
  QwtPlotMagnifier* magnifier_;

  double* indexPoints_;
  double* dataPoints_;
  double l1IndexPoints_[2];
  double l1DataPoints_[2];
  double l2IndexPoints_[2];
  double l2DataPoints_[2];

  int64_t numPoints_;
  int counter_;
  double xMin_;
  double xMax_;
  bool haveLine1_;
  bool haveLine2_;
};

#endif // PSDPLOT_H
