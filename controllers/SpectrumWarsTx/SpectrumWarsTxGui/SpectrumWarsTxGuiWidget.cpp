#include "SpectrumWarsTxGuiWidget.h"
#include "graphics/qt/common/Spectrogramplot.h"
#include "graphics/qt/common/Lineplot.h"
#include "graphics/qt/common/Events.h"
#include "KnobSpin.h"

#include <QLayout>
#include <QDoubleSpinBox>


SpectrumWarsTxGuiWidget::SpectrumWarsTxGuiWidget(SWTxGuiParams params,
                                                 SWTxGuiCallback* callback,
                                                 QWidget *parent)
  :QWidget(parent)
{
  params_ = params;
  callback_ = callback;
  knob1_ = new KnobSpin("Frequency (MHz)", params_.minFreq, params_.maxFreq,
                        0.1, this);
  knob2_ = new KnobSpin("Bandwidth (MHz)", params_.minBW, params_.maxBW,
                        0.25, this);
  knob3_ = new KnobSpin("Gain", params_.minGain, params_.maxGain,
                        0.1, this);

  connect(knob1_, SIGNAL(valueChanged(double)),
          this, SLOT(setFrequency(double)));
  connect(knob2_, SIGNAL(valueChanged(double)),
          this, SLOT(setBandwidth(double)));
  connect(knob3_, SIGNAL(valueChanged(double)),
          this, SLOT(setGain(double)));

  QHBoxLayout* hLayout1 = new QHBoxLayout(this);
  hLayout1->addWidget(knob1_);
  hLayout1->addWidget(knob2_);
  hLayout1->addWidget(knob3_);

  setPalette( QPalette( QColor( 192, 192, 192 ) ) );
  updateGradient();
}

SpectrumWarsTxGuiWidget::~SpectrumWarsTxGuiWidget()
{
}

void SpectrumWarsTxGuiWidget::setFrequency(double frequency)
{
  double delta = 0.05;
  if(frequency > frequency_+delta || frequency < frequency_-delta)
  {
    frequency_ = frequency;
    callback_->setFrequency(frequency_);
  }
}

void SpectrumWarsTxGuiWidget::setBandwidth(double bandwidth)
{
  double delta = 0.05;
  if(bandwidth > bandwidth_+delta || bandwidth < bandwidth_-delta)
  {
    bandwidth_ = bandwidth;
    callback_->setBandwidth(bandwidth_);
  }
}

void SpectrumWarsTxGuiWidget::setGain(double gain)
{
  double delta = 0.05;
  if(gain > gain_+delta || gain < gain_-delta)
  {
    gain_ = gain;
    callback_->setGain(gain_);
  }
}

void SpectrumWarsTxGuiWidget::updateGradient()
{
    QPalette pal = palette();

    const QColor buttonColor = pal.color( QPalette::Button );
    const QColor lightColor = pal.color( QPalette::Light );
    const QColor midLightColor = pal.color( QPalette::Midlight );

#ifdef Q_WS_X11
    // Qt 4.7.1: QGradient::StretchToDeviceMode is buggy on X11

    QLinearGradient gradient( rect().topLeft(), rect().topRight() );
#else
    QLinearGradient gradient( 0, 0, 1, 0 );
    gradient.setCoordinateMode( QGradient::StretchToDeviceMode );
#endif

    gradient.setColorAt( 0.0, midLightColor );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );

    pal.setBrush( QPalette::Window, gradient );
    setPalette( pal );
}
