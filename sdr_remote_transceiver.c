#include "sdr_remote_transceiver.h"

// init default values
void initSDRRemoteTransceiver(sdrRemoteTransceiver *trx) {
  if (trx != NULL) {
    trx->powerStatus = TRX_PS_OFF;
    trx->activeVFOIndex = 0;
    trx->VFO[0].frequency = 0;
    trx->VFO[0].mode = TRX_VFO_MD_RESERVED;
    trx->VFO[0].step = 0;
    trx->VFO[1].frequency = 0;
    trx->VFO[1].mode = TRX_VFO_MD_RESERVED;
    trx->VFO[1].step = 0;
    trx->changed = 0;
    trx->signalMeterLevel = 0;
    trx->AFGain = 0;
    trx->audioMuted = TRX_AUDIO_NOT_MUTED;
  }
}

// change current active VFO
void setTRXActiveVFOIndex(sdrRemoteTransceiver *trx, uint8_t VFOIndex) {
  if (trx != NULL && (VFOIndex == 0 || VFOIndex == 1)) {
    trx->activeVFOIndex = 0;
    trx->changed |= TRX_CFLAG_ACTIVE_VFO_INDEX;
  }
}

// set vfo frequency
void setTRXVFOFrequency(sdrRemoteTransceiver *trx, uint8_t VFOIndex, uint64_t frequency) {
  if (trx != NULL && (VFOIndex == 0 || VFOIndex == 1)) {
    trx->VFO[VFOIndex].frequency = frequency;
    trx->changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
  }
}

// set vfo mode
void setTRXVFOMode(sdrRemoteTransceiver *trx, uint8_t VFOIndex, TRXVFOMode mode) {
  if (trx != NULL && (VFOIndex == 0 || VFOIndex == 1)) {
    trx->VFO[VFOIndex].mode = mode;
    trx->changed |= TRX_CFLAG_ACTIVE_VFO_MODE;
  }
}

// set vfo step size (hz)
void setTRXVFOHzStep(sdrRemoteTransceiver *trx, uint8_t VFOIndex, uint64_t hz) {
  if (trx != NULL && (VFOIndex == 0 || VFOIndex == 1)) {
    trx->VFO[VFOIndex].step = hz;
    trx->changed |= TRX_CFLAG_ACTIVE_VFO_STEP;
  }
}

// set signal level meter
void setTRXSignalMeterLevel(sdrRemoteTransceiver *trx, uint8_t level) {
  if (trx != NULL) {
    trx->signalMeterLevel = level;
    trx->changed |= TRX_CFLAG_SIGNAL_METER_LEVEL;
  }
}

// set af gain
void setTRXAFGain(sdrRemoteTransceiver *trx, uint8_t value) {
  if (trx != NULL) {
    trx->AFGain = value;
    trx->changed |= TRX_CFLAG_AF_GAIN;
  }
}

// set audio status to muted
void setTRXAudioMuted(sdrRemoteTransceiver *trx) {
  if (trx != NULL) {
    trx->audioMuted = TRX_AUDIO_MUTED;
    trx->changed |= TRX_CFLAG_AUDIO_MUTE;
  }
}

// set audio status to unmuted
void setTRXAudioUnMuted(sdrRemoteTransceiver *trx) {
  if (trx != NULL) {
    trx->audioMuted = TRX_AUDIO_NOT_MUTED;
    trx->changed |= TRX_CFLAG_AUDIO_MUTE;
  }
}
