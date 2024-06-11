#include "Transceiver.hpp"

Transceiver::Transceiver(void) {
  this->powerStatus = TRX_PS_OFF;
  this->powerStatus = TRX_PS_ON;
  this->activeVFOIndex = 0;
  this->VFO[0].frequency = 7050123;
  this->VFO[0].mode = TRX_VFO_MD_USB;
  this->VFO[0].step = 0;
  this->VFO[1].frequency = 0;
  this->VFO[1].mode = TRX_VFO_MD_LSB;
  this->VFO[1].step = 0;
  this->changed = TRX_CFLAG_TRANSMIT_RECEIVE_POWER_STATUS | TRX_CFLAG_TRANSMIT_RECEIVE_STATUS | TRX_CFLAG_ACTIVE_VFO_INDEX | TRX_CFLAG_ACTIVE_VFO_MODE | TRX_CFLAG_ACTIVE_VFO_FREQUENCY | TRX_CFLAG_ACTIVE_VFO_STEP | TRX_CFLAG_SIGNAL_METER_LEVEL | TRX_CFLAG_AUDIO_MUTE | TRX_CFLAG_AF_GAIN | TRX_CFLAG_FILTER_LOW | TRX_CFLAG_FILTER_HIGH;
  this->signalMeterLevel = 0;
  this->AFGain = 50;
  this->audioMuted = TRX_AUDIO_NOT_MUTED;
}

// change current active VFO
void Transceiver::setActiveVFOIndex(uint8_t VFOIndex) {
  if (VFOIndex == 0 || VFOIndex == 1) {
    this->activeVFOIndex = 0;
    this->changed |= TRX_CFLAG_ACTIVE_VFO_INDEX;
  }
}

// set vfo frequency
void Transceiver::setVFOFrequency(uint8_t VFOIndex, uint64_t frequency) {
  if (VFOIndex == 0 || VFOIndex == 1) {
    this->VFO[VFOIndex].frequency = frequency;
    this->changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
  }
}

// set vfo mode
void Transceiver::setVFOMode(uint8_t VFOIndex, TRXVFOMode mode) {
  if (VFOIndex == 0 || VFOIndex == 1) {
    this->VFO[VFOIndex].mode = mode;
    this->changed |= TRX_CFLAG_ACTIVE_VFO_MODE;
  }
}

// set vfo step size (hz)
void Transceiver::setVFOHzStep(uint8_t VFOIndex, uint64_t hz) {
  if (VFOIndex == 0 || VFOIndex == 1) {
    this->VFO[VFOIndex].step = hz;
    this->changed |= TRX_CFLAG_ACTIVE_VFO_STEP;
  }
}

// set vfo low filter size (hz)
void Transceiver::setVFOLowFilterHz(uint8_t VFOIndex, uint32_t hz) {
  if (VFOIndex == 0 || VFOIndex == 1) {
    this->VFO[VFOIndex].LF = hz;
    this->VFO[VFOIndex].BW = this->VFO[VFOIndex].LF + this->VFO[VFOIndex].HF;
    this->changed |= TRX_CFLAG_FILTER_LOW;
  }
}

// set vfo high filter size (hz)
void Transceiver::setVFOHighFilterHz(uint8_t VFOIndex, uint32_t hz) {
  if (VFOIndex == 0 || VFOIndex == 1) {
    this->VFO[VFOIndex].HF = hz;
    this->VFO[VFOIndex].BW = this->VFO[VFOIndex].LF + this->VFO[VFOIndex].HF;
    this->changed |= TRX_CFLAG_FILTER_HIGH;
  }
}

// set signal level meter
void Transceiver::setSignalMeterLevel(uint8_t level) {
  this->signalMeterLevel = level;
  this->changed |= TRX_CFLAG_SIGNAL_METER_LEVEL;
}

// set af gain
void Transceiver::setAFGain(uint8_t value) {
  this->AFGain = value;
  this->changed |= TRX_CFLAG_AF_GAIN;
}

// set audio status to muted
void Transceiver::setAudioMuted() {
  this->audioMuted = TRX_AUDIO_MUTED;
  this->changed |= TRX_CFLAG_AUDIO_MUTE;
}

// set audio status to unmuted
void Transceiver::setAudioUnMuted() {
  this->audioMuted = TRX_AUDIO_NOT_MUTED;
  this->changed |= TRX_CFLAG_AUDIO_MUTE;
}

void Transceiver::incSerialCommandCount(void) {
  this->serialCommandCount++;
}

uint64_t Transceiver::getSerialCommandCount(void) {
  return(this->serialCommandCount);
}