#ifndef SDR_REMOTE_CONTROL_TRANSCEIVER_H
#define SDR_REMOTE_CONTROL_TRANSCEIVER_H

#include <stdint.h> 
#include <string.h>

#define TRX_CFLAG_TRANSMIT_RECEIVE_STATUS 0x01
#define TRX_CFLAG_ACTIVE_VFO_INDEX 0x02
#define TRX_CFLAG_ACTIVE_VFO_MODE 0x04
#define TRX_CFLAG_ACTIVE_VFO_FREQUENCY 0x08

typedef struct {
  char currentVFOFrequencyAsString[12];
  uint64_t currentVFOFrequencyAsInt;
  uint8_t changed;
} sdrRemoteTransceiver;

void initSDRRemoteTransceiver(sdrRemoteTransceiver* trx);

#endif
