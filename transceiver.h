#ifndef SDR_REMOTE_CONTROL_TRANSCEIVER_H
#define SDR_REMOTE_CONTROL_TRANSCEIVER_H

#include <stdint.h>
#include <stddef.h>

#define TRX_CFLAG_TRANSMIT_RECEIVE_STATUS 0x01
#define TRX_CFLAG_ACTIVE_VFO_INDEX 0x02
#define TRX_CFLAG_ACTIVE_VFO_MODE 0x04
#define TRX_CFLAG_ACTIVE_VFO_FREQUENCY 0x08

#ifdef TS2K_SDR_RADIO_CONSOLE

// SDR Radio Console (by Simon Brown / G4ELI) Serial CAT https://www.sdr-radio.com/SerialPort
typedef enum {
  DSB = 0,
  LSB = 1,
  USB = 2,
  // CW upper sideband
  CW_U = 3,
  FM = 4,
  // synchronous AM, includes ECSS
  SAM = 5,
  RESERVED = 6,
  // CW lower sideband
  CW_L = 7,
  WFM = 8,
  BFM = 9
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
  "BFM"
};

#else

// TS-2000 (from Kenwood TS-2000 manual)
typedef enum {
  LSB = 1,
  USB = 2,
  CW = 3,
  FM = 4,
  AM = 5,
  FSK = 6,
  CR_R = 7,
  RESERVED = 8,
  FSK_R = 9
} VFOMode_;

const char* VFOModeNames[] = {
  "LSB",
  "USB",
  "CW",
  "FM",
  "AM",
  "FSK",
  "CRR",
  "",
  "FSKR"
};

#endif

typedef struct {
  char currentVFOFrequencyAsString[12];
  uint64_t currentVFOFrequencyAsInt;
  //VFOMode currentVFOMode;
  uint8_t changed;
  uint64_t currentHzStep;
} sdrRemoteTransceiver;

void initSDRRemoteTransceiver(sdrRemoteTransceiver* trx);

void setCurrentHzStep(sdrRemoteTransceiver* trx, uint64_t hz);

#endif
