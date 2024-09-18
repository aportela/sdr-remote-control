#ifndef SDR_REMOTE_CONTROL_TRANSCEIVER_H
#define SDR_REMOTE_CONTROL_TRANSCEIVER_H

#include <stdint.h>
#include <stddef.h>
#include <string>

// bitmask definitions for checking changed values
#define TRX_CFLAG_TRANSMIT_RECEIVE_POWER_STATUS (1 << 0) // 1
#define TRX_CFLAG_TRANSMIT_RECEIVE_STATUS (1 << 1)       // 2
#define TRX_CFLAG_VFO_INDEX (1 << 2)                     // 4
#define TRX_CFLAG_ACTIVE_VFO_FREQUENCY (1 << 3)          // 8
#define TRX_CFLAG_ACTIVE_VFO_MODE (1 << 4)               // 16
#define TRX_CFLAG_ACTIVE_VFO_STEP (1 << 5)               // 32
#define TRX_CFLAG_ACTIVE_VFO_FILTER_LOW (1 << 6)         // 64
#define TRX_CFLAG_ACTIVE_VFO_FILTER_HIGH (1 << 7)        // 128
#define TRX_CFLAG_SECONDARY_VFO_FREQUENCY (1 << 8)       // 256
#define TRX_CFLAG_SECONDARY_VFO_MODE (1 << 9)            // 512
#define TRX_CFLAG_SECONDARY_VFO_STEP (1 << 10)           // 1024
#define TRX_CFLAG_SECONDARY_VFO_FILTER_LOW (1 << 11)     // 2048
#define TRX_CFLAG_SECONDARY_VFO_FILTER_HIGH (1 << 12)    // 4096
#define TRX_CFLAG_SIGNAL_METER_LEVEL (1 << 13)           // 8192
#define TRX_CFLAG_AUDIO_MUTE (1 << 14)                   // 16384
#define TRX_CFLAG_AF_GAIN (1 << 15)                      // 32768
#define TRX_CFLAG_SEND_CAT (1 << 16)                     // 65536

typedef enum
{
  TRX_AUDIO_NOT_MUTED = 0,
  TRX_AUDIO_MUTED = 1
} TRxAudioMuteStatus;

// SDR Radio Console (by Simon Brown / G4ELI) Serial CAT https://www.sdr-radio.com/SerialPort
typedef enum
{
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

typedef struct
{
  uint64_t frequency;
  TRXVFOMode mode;
  uint32_t LF;         // low filter
  uint32_t HF;         // high filter
  uint32_t BW;         // bandwith
  uint64_t customStep; // hz
} TRXVFO;

class Transceiver
{
private:
  bool lockedByControls;

public:
  uint32_t changed;
  bool poweredOn;
  char radioName[32] = "unknown";
  uint8_t activeVFOIndex;
  TRXVFO VFO[2];
  uint8_t signalMeterLevel;
  uint8_t AFGain;
  TRxAudioMuteStatus audioMuted;

  Transceiver();

  // check if data is locked by external controls
  bool isLockedByControls();

  // set lock
  void setLockedByControls(bool status);

  // change current active VFO
  void setVFOIndex(uint8_t VFOIndex);

  // set (active) vfo frequency
  void setActiveVFOFrequency(uint64_t frequency);

  // increment (active) vfo frequency by specified hz
  void incrementActiveVFOFrequency(uint64_t hz);

  // decrement (active) vfo frequency by specified hz
  void decrementActiveVFOFrequency(uint64_t hz);

  // set (secondary) vfo frequency
  void setSecondaryVFOFrequency(uint64_t frequency);

  // set (active) vfo mode
  void setActiveVFOMode(TRXVFOMode mode);

  // set (secondary) vfo mode
  void setSecondaryVFOMode(TRXVFOMode mode);

  // set (active) vfo custom step size (hz)
  void setActiveVFOHzCustomStep(uint64_t hz);

  // set (secondary) vfo custom step size (hz)
  void setSecondaryVFOHzCustomStep(uint64_t hz);

  // set (active) vfo low filter size (hz)
  void setActiveVFOLowFilterHz(uint32_t hz);

  // set (secondary) vfo low filter size (hz)
  void setSecondaryVFOLowFilterHz(uint32_t hz);

  // set (active) vfo high filter size (hz)
  void setActiveVFOHighFilterHz(uint32_t hz);

  // set (secondary) vfo high filter size (hz)
  void setSecondaryVFOHighFilterHz(uint32_t hz);

  // set signal level meter
  void setSignalMeterLevel(uint8_t level);

  // set af gain
  void setAFGain(uint8_t value);

  // increment ag gain by specified units
  void incrementAFGain(uint8_t units);

  // decrement ag gain by specified units
  void decrementAFGain(uint8_t units);

  // set audio status to muted
  void setAudioMuted();

  // set audio status to unmuted
  void setAudioUnMuted();

  void incSerialCommandCount(void);

  uint64_t getSerialCommandCount(void);

private:
  uint64_t serialCommandCount = 0;
};

#endif
