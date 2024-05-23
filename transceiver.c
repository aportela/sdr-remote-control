#include "transceiver.h"

void initSDRRemoteTransceiver(sdrRemoteTransceiver* trx) {
  if (trx != NULL) {
    trx->powerStatus = TRX_PS_OFF;
    strncpy(trx->currentVFOFrequencyAsString, "00000000000", sizeof(trx->currentVFOFrequencyAsString) - 1);
    trx->currentVFOFrequencyAsString[sizeof(trx->currentVFOFrequencyAsString) - 1] = '\0';
    trx->currentVFOFrequencyAsInt = 0;
    trx->currentVFOMode = VFO_MD_RESERVED;
    trx->changed = 0;
    trx->currentHzStep = 1;
    trx->SMeter = 0;
  }
}

void setCurrentHzStep(sdrRemoteTransceiver* trx, uint64_t hz) {
  if (trx != NULL ){
    trx->currentHzStep = hz;
  }
}
