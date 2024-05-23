#ifndef SDR_REMOTE_CONTROL_TRANSCEIVER_H
#define SDR_REMOTE_CONTROL_TRANSCEIVER_H

#include <stdint.h>
#include <stddef.h>

#define TRX_CFLAG_TRANSMIT_RECEIVE_STATUS 0x01
#define TRX_CFLAG_ACTIVE_VFO_INDEX 0x02
#define TRX_CFLAG_ACTIVE_VFO_MODE 0x04
#define TRX_CFLAG_ACTIVE_VFO_FREQUENCY 0x08
#define TRX_CFLAG_SMETER 0x16

typedef enum {
  TRX_PS_OFF = 0,
  TRX_PS_ON = 1
} TRxPowerStatus;

// SDR Radio Console (by Simon Brown / G4ELI) Serial CAT https://www.sdr-radio.com/SerialPort
typedef enum {
  VFO_MD_DSB = 0,
  VFO_MD_LSB = 1,
  VFO_MD_USB = 2,
  // CW upper sideband
  VFO_MD_CW_U = 3,
  VFO_MD_FM = 4,
  // synchronous AM, includes ECSS
  VFO_MD_SAM = 5,
  VFO_MD_RESERVED = 6,
  // CW lower sideband
  VFO_MD_CW_L = 7,
  VFO_MD_WFM = 8,
  VFO_MD_BFM = 9,
  VFO_MODE_ERROR = 10
} VFOMode_;

const char* VFOModeNames[] = {
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

typedef struct {
  TRxPowerStatus powerStatus;
  char currentVFOFrequencyAsString[12];
  uint64_t currentVFOFrequencyAsInt;
  VFOMode_ currentVFOMode;
  uint8_t changed;
  uint64_t currentHzStep;
  uint8_t SMeter;
  uint8_t volume;
} sdrRemoteTransceiver;

void initSDRRemoteTransceiver(sdrRemoteTransceiver* trx);

void setCurrentHzStep(sdrRemoteTransceiver* trx, uint64_t hz);

#endif
