#ifndef SWRXGUICALLBACK_H
#define SWRXGUICALLBACK_H

class SWRxGuiCallback
{
public:
  virtual void setFrequency(double frequency) = 0;
  virtual void setBandwidth(double bandwidth) = 0;
  virtual void setGain(double gain) = 0;
};

#endif // SWRXGUICALLBACK_H
