#include "Transceiver.hpp"

Transceiver::Transceiver(void)
{
  this->poweredOn = false;
  this->poweredOn = true;
  this->activeVFOIndex = 0;
  this->VFO[0].frequency = 7050123;
  this->VFO[0].mode = TRX_VFO_MD_LSB;
  this->VFO[0].customStep = 1;
  this->VFO[1].frequency = 145625000;
  this->VFO[1].mode = TRX_VFO_MD_FM;
  this->VFO[1].customStep = 1000;
  this->changed = ((uint16_t)1 << 16) - 1; // ALL (uint16_t) flags active
  this->signalMeterLevel = 0;
  this->AFGain = 50;
  this->audioMuted = TRX_AUDIO_NOT_MUTED;
}

// change current active VFO
void Transceiver::setVFOIndex(uint8_t VFOIndex)
{
  if (VFOIndex == 0 || VFOIndex == 1)
  {
    this->activeVFOIndex = VFOIndex;
    this->changed |= TRX_CFLAG_VFO_INDEX;
  }
}

// set (active) vfo frequency
void Transceiver::setActiveVFOFrequency(uint64_t frequency)
{
  this->VFO[0].frequency = frequency;
  this->changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
}

// set (secondary) vfo frequency
void Transceiver::setSecondaryVFOFrequency(uint64_t frequency)
{
  this->VFO[1].frequency = frequency;
  this->changed |= TRX_CFLAG_SECONDARY_VFO_FREQUENCY;
}

// set (active) vfo mode
void Transceiver::setActiveVFOMode(TRXVFOMode mode)
{
  this->VFO[0].mode = mode;
  this->changed |= TRX_CFLAG_ACTIVE_VFO_MODE;
}

// set (secondary) vfo mode
void Transceiver::setSecondaryVFOMode(TRXVFOMode mode)
{
  this->VFO[1].mode = mode;
  this->changed |= TRX_CFLAG_SECONDARY_VFO_MODE;
}

// set (active) vfo custom step size (hz)
void Transceiver::setActiveVFOHzCustomStep(uint64_t hz)
{
  this->VFO[0].customStep = hz;
  this->changed |= TRX_CFLAG_ACTIVE_VFO_STEP;
}

// set (secondary) vfo custom step size (hz)
void Transceiver::setSecondaryVFOHzCustomStep(uint64_t hz)
{
  this->VFO[1].customStep = hz;
  this->changed |= TRX_CFLAG_SECONDARY_VFO_STEP;
}

// set (active) vfo low filter size (hz)
void Transceiver::setActiveVFOLowFilterHz(uint32_t hz)
{
  this->VFO[0].LF = hz;
  this->VFO[0].BW = this->VFO[0].LF + this->VFO[0].HF;
  this->changed |= TRX_CFLAG_ACTIVE_VFO_FILTER_LOW;
}

// set (secondary) vfo low filter size (hz)
void Transceiver::setSecondaryVFOLowFilterHz(uint32_t hz)
{
  this->VFO[1].LF = hz;
  this->VFO[1].BW = this->VFO[1].LF + this->VFO[1].HF;
  this->changed |= TRX_CFLAG_SECONDARY_VFO_FILTER_LOW;
}

// set (active) vfo high filter size (hz)
void Transceiver::setActiveVFOHighFilterHz(uint32_t hz)
{
  this->VFO[0].HF = hz;
  this->VFO[0].BW = this->VFO[0].LF + this->VFO[0].HF;
  this->changed |= TRX_CFLAG_ACTIVE_VFO_FILTER_HIGH;
}

// set (secondary) vfo high filter size (hz)
void Transceiver::setSecondaryVFOHighFilterHz(uint32_t hz)
{
  this->VFO[1].HF = hz;
  this->VFO[1].BW = this->VFO[1].LF + this->VFO[1].HF;
  this->changed |= TRX_CFLAG_SECONDARY_VFO_FILTER_HIGH;
}

// set signal level meter
void Transceiver::setSignalMeterLevel(uint8_t level)
{
  this->signalMeterLevel = level;
  this->changed |= TRX_CFLAG_SIGNAL_METER_LEVEL;
}

// set af gain
void Transceiver::setAFGain(uint8_t value)
{
  this->AFGain = value;
  this->changed |= TRX_CFLAG_AF_GAIN;
}

// set audio status to muted
void Transceiver::setAudioMuted()
{
  this->audioMuted = TRX_AUDIO_MUTED;
  this->changed |= TRX_CFLAG_AUDIO_MUTE;
}

// set audio status to unmuted
void Transceiver::setAudioUnMuted()
{
  this->audioMuted = TRX_AUDIO_NOT_MUTED;
  this->changed |= TRX_CFLAG_AUDIO_MUTE;
}

void Transceiver::incSerialCommandCount(void)
{
  this->serialCommandCount++;
}

uint64_t Transceiver::getSerialCommandCount(void)
{
  return (this->serialCommandCount);
}