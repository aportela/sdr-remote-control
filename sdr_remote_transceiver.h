#ifndef SDR_REMOTE_CONTROL_TRANSCEIVER_H
#define SDR_REMOTE_CONTROL_TRANSCEIVER_H

#include <stdint.h>
#include <stddef.h>

// bitmask definitions for checking changed values
#define TRX_CFLAG_TRANSMIT_RECEIVE_POWER_STATUS (1 << 0)  // 1
#define TRX_CFLAG_TRANSMIT_RECEIVE_STATUS (1 << 1)        // 2
#define TRX_CFLAG_ACTIVE_VFO_INDEX (1 << 2)               // 4
#define TRX_CFLAG_ACTIVE_VFO_MODE (1 << 3)                // 8
#define TRX_CFLAG_ACTIVE_VFO_FREQUENCY (1 << 4)           // 16
#define TRX_CFLAG_ACTIVE_VFO_STEP (1 << 5)                // 32
#define TRX_CFLAG_SIGNAL_METER_LEVEL (1 << 6)             // 64
#define TRX_CFLAG_AUDIO_MUTE (1 << 7)                     // 128
#define TRX_CFLAG_AF_GAIN (1 << 8)                        // 256
#define TRX_CFLAG_FILTER_LOW (1 << 9)                     // 512
#define TRX_CFLAG_FILTER_HIGH (1 << 10)                   // 1024

typedef enum {
  TRX_PS_OFF = 0,
  TRX_PS_ON = 1
} TRxPowerStatus;

typedef enum {
  TRX_AUDIO_NOT_MUTED = 0,
  TRX_AUDIO_MUTED = 1
} TRxAudioMuteStatus;

// SDR Radio Console (by Simon Brown / G4ELI) Serial CAT https://www.sdr-radio.com/SerialPort
typedef enum {
  TRX_VFO_MD_DSB = 0,
  TRX_VFO_MD_LSB = 1,
  TRX_VFO_MD_USB = 2,
  // CW upper sideband
  TRX_VFO_MD_CW_U = 3,
  TRX_VFO_MD_FM = 4,
  // synchronous AM, includes ECSS
  TRX_VFO_MD_SAM = 5,
  TRX_VFO_MD_RESERVED = 6,
  // CW lower sideband
  TRX_VFO_MD_CW_L = 7,
  TRX_VFO_MD_WFM = 8,
  TRX_VFO_MD_BFM = 9,
  TRX_VFO_MODE_ERROR = 10
} TRXVFOMode;

const char *TRXVFOModeNames[] = {
  "DSB",
  "LSB",
  "USB",
  "CWU",
  "FM",
  "SAM",
  "",
  "CWL",
  "WFM",
  "BFM",
  "???"
};

typedef struct
{
  uint64_t frequency;
  TRXVFOMode mode;
  uint64_t step;  // hz
} TRXVFO;

typedef struct
{
  uint32_t changed;
  TRxPowerStatus powerStatus;
  uint8_t activeVFOIndex;
  TRXVFO VFO[2];
  uint8_t signalMeterLevel;
  uint8_t AFGain;
  TRxAudioMuteStatus audioMuted;
} sdrRemoteTransceiver;

// init default values
void initSDRRemoteTransceiver(sdrRemoteTransceiver *trx);

// change current active VFO
void setTRXActiveVFOIndex(sdrRemoteTransceiver *trx, uint8_t VFOIndex);

// set vfo frequency
void setTRXVFOFrequency(sdrRemoteTransceiver *trx, uint8_t VFOIndex, uint64_t frequency);

// set vfo mode
void setTRXVFOMode(sdrRemoteTransceiver *trx, uint8_t VFOIndex, TRXVFOMode mode);

// set vfo step size (hz)
void setTRXVFOHzStep(sdrRemoteTransceiver *trx, uint8_t VFOIndex, uint64_t hz);

// set signal level meter
void setTRXSignalMeterLevel(sdrRemoteTransceiver *trx, uint8_t level);

// set af gain
void setTRXAFGain(sdrRemoteTransceiver *trx, uint8_t value);

// set audio status to muted
void setTRXAudioMuted(sdrRemoteTransceiver *trx);

// set audio status to unmuted
void setTRXAudioUnMuted(sdrRemoteTransceiver *trx);

#endif
