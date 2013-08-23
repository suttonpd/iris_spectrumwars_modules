#ifndef SWTXGUICALLBACK_H
#define SWTXGUICALLBACK_H

class SWTxGuiCallback
{
public:
  virtual void setFrequency(double frequency) = 0;
  virtual void setBandwidth(double bandwidth) = 0;
  virtual void setGain(double gain) = 0;
};

#endif // SWTXGUICALLBACK_H
