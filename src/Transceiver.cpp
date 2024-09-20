#include "Transceiver.hpp"

Transceiver::Transceiver(void)
{
  this->lockedByControls = false;
  this->poweredOn = false;
  // this->poweredOn = true;
  this->activeVFOIndex = 0;
  this->VFO[0].frequency = 7050123;
  this->VFO[0].mode = TRX_VFO_MD_LSB;
  this->VFO[0].customStep = 1;
  this->VFO[1].frequency = 145625000;
  this->VFO[1].mode = TRX_VFO_MD_FM;
  this->VFO[1].customStep = 1000;
  this->changed = ((uint16_t)1 << 16) - 1; // ALL (uint16_t) flags active
  this->signalMeterdBLevel = 0;
  this->AFGain = 50;
  this->audioMuted = TRX_AUDIO_NOT_MUTED;
}

// check if data is locked by external controls
bool Transceiver::isLockedByControls()
{
  return (false);
  return (this->lockedByControls);
}

// set lock
void Transceiver::setLockedByControls(bool status)
{
  this->lockedByControls = status;
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

// increment (active) vfo frequency by specified hz
void Transceiver::incrementActiveVFOFrequency(uint64_t hz)
{
  this->VFO[0].frequency += hz;
  this->changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
}

// decrement (active) vfo frequency by specified hz
void Transceiver::decrementActiveVFOFrequency(uint64_t hz)
{
  this->VFO[0].frequency -= hz;
  this->changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
}

// set (secondary) vfo frequency
void Transceiver::setSecondaryVFOFrequency(uint64_t frequency)
{
  this->VFO[1].frequency = frequency;
  this->changed |= TRX_CFLAG_SECONDARY_VFO_FREQUENCY;
}

// set (active) vfo mode
void Transceiver::setActiveVFOMode(TrxVFOMode mode)
{
  this->VFO[0].mode = mode;
  this->changed |= TRX_CFLAG_ACTIVE_VFO_MODE;
}

// set (secondary) vfo mode
void Transceiver::setSecondaryVFOMode(TrxVFOMode mode)
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
void Transceiver::setSignalMeter(TRXSMeterUnitType unitType, uint8_t units)
{
  switch (unitType)
  {
  case SIGNAL_METER_TS2K_SDR_RADIO_LEVEL:
    this->signalMeterTS2KSDRRadioUnits = units;
    this->signalMeterdBLevel = units * 3;
    break;
  case SIGNAL_METER_DB_UNITS:
    this->signalMeterTS2KSDRRadioUnits = units / 3;
    this->signalMeterdBLevel = units;
    break;
  default:
    break;
  }
  this->changed |= TRX_CFLAG_SIGNAL_METER_DB_LEVEL;
}

// set af gain
void Transceiver::setAFGain(uint8_t value)
{
  this->AFGain = value;
  this->changed |= TRX_CFLAG_AF_GAIN;
}

// increment ag gain by specified units
void Transceiver::incrementAFGain(uint8_t units)
{
  if ((this->AFGain + units) <= 100)
  {
    this->AFGain += units;
  }
  else
  {
    this->AFGain = 100;
  }
  this->changed |= TRX_CFLAG_AF_GAIN;
}

// decrement ag gain by specified units
void Transceiver::decrementAFGain(uint8_t units)
{
  if ((this->AFGain - units) >= 0)
  {
    this->AFGain -= units;
  }
  else
  {
    this->AFGain = 0;
  }
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