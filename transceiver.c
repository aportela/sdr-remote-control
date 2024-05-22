#include "transceiver.h"

void initSDRRemoteTransceiver(sdrRemoteTransceiver* trx) {
  if (trx != NULL) {
    strncpy(trx->currentVFOFrequencyAsString, "00000000000", sizeof(trx->currentVFOFrequencyAsString) - 1);
    trx->currentVFOFrequencyAsString[sizeof(trx->currentVFOFrequencyAsString) - 1] = '\0';
    trx->currentVFOFrequencyAsInt = 0;
    trx->changed = 0;
  }
}
