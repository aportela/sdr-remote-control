#include "Transceiver.hpp"
#include <cstdint>

#include <Arduino.h>

#define QUEUE_PEEK_MS_TO_TICKS_TIMEOUT 5
#define MAX_SYNC_QUEUE_ELEMENTS 128

Transceiver::Transceiver(void)
{
  this->syncQueue = xQueueCreate(MAX_SYNC_QUEUE_ELEMENTS, sizeof(TransceiverSyncCommand));
  this->statusQueue = xQueueCreate(1, sizeof(TransceiverStatus));
  TransceiverStatus trxStatus;
  this->setCurrentStatus(&trxStatus, false);
}

Transceiver::~Transceiver()
{
  vQueueDelete(this->statusQueue);
}

uint16_t Transceiver::getBandIndex(uint64_t currentFrequency, uint16_t currentBandIndex)
{
  if (currentFrequency >= RadioBands[currentBandIndex].minFrequency && currentFrequency <= RadioBands[currentBandIndex].maxFrequency)
  {
    return (currentBandIndex);
  }
  else
  {
    uint16_t newBandIndex = 0;
    // RadioBands[0] is default (with dummy values) so we ignore this first index
    if (currentBandIndex > 0)
    {
      if (currentFrequency < RadioBands[currentBandIndex].minFrequency)
      {
        // band is lower
        if (currentBandIndex > 1)
        {
          // there are more available Radiobands below current index
          for (uint16_t i = currentBandIndex - 1; i > 0 && newBandIndex == 0; i--)
          {
            if (currentFrequency >= RadioBands[i].minFrequency && currentFrequency <= RadioBands[i].maxFrequency)
            {
              newBandIndex = i;
            }
          }
        }
      }
      else if (currentFrequency > RadioBands[currentBandIndex].maxFrequency)
      {
        // band is upper
        if (currentBandIndex < RADIO_BANDS_SIZE - 1)
        {
          // there are more available RadioBands after current index
          for (uint16_t i = currentBandIndex + 1; i < RADIO_BANDS_SIZE && newBandIndex == 0; i++)
          {
            if (currentFrequency >= RadioBands[i].minFrequency && currentFrequency <= RadioBands[i].maxFrequency)
            {
              newBandIndex = i;
            }
          }
        }
      }
      return (newBandIndex);
    }
  }
}

bool Transceiver::getCurrentStatus(TransceiverStatus *status, bool fromISR)
{
  if (this->statusQueue != nullptr && status != nullptr)
  {
    if (!fromISR)
    {
      return (xQueuePeek(this->statusQueue, status, pdMS_TO_TICKS(QUEUE_PEEK_MS_TO_TICKS_TIMEOUT)) == pdPASS);
    }
    else
    {
      return (xQueuePeekFromISR(this->statusQueue, status) == pdPASS);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setCurrentStatus(const TransceiverStatus *status, bool fromISR)
{
  if (this->statusQueue != nullptr && status != nullptr)
  {
    if (!fromISR)
    {
      return (xQueueOverwrite(this->statusQueue, status) != pdPASS);
    }
    else
    {
      BaseType_t xHigherPriorityTaskWoken = pdFALSE;
      bool result = xQueueOverwriteFromISR(this->statusQueue, status, &xHigherPriorityTaskWoken) != pdPASS;
      if (xHigherPriorityTaskWoken == pdTRUE)
      {
        portYIELD_FROM_ISR();
      }
      return (result);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setPowerOnStatus(bool powerOnStatus, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    currentStatus.poweredOn = powerOnStatus;
    return (this->setCurrentStatus(&currentStatus, fromISR));
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setRadioName(const char *radioName, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    snprintf(currentStatus.radioName, sizeof(currentStatus.radioName), "%s", radioName);
    return (this->setCurrentStatus(&currentStatus, fromISR));
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setActiveVFO(uint8_t VFOIndex, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (VFOIndex < TRANSCEIVER_VFO_COUNT)
    {
      if (currentStatus.activeVFOIndex != VFOIndex)
      {
        currentStatus.activeVFOIndex = VFOIndex;
        return (this->setCurrentStatus(&currentStatus, fromISR));
      }
      else
      {
        return (false);
      }
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::toggleActiveVFO(bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    switch (currentStatus.activeVFOIndex)
    {
    case 0:
      currentStatus.activeVFOIndex = 1;
      break;
    case 1:
      currentStatus.activeVFOIndex = 0;
      break;
    }
    return (this->setCurrentStatus(&currentStatus, fromISR));
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setVFOFrequency(uint8_t VFOIndex, uint64_t frequency, bool fromISR)
{
  if (frequency >= 0 && frequency <= MAX_FREQUENCY)
  {
    TransceiverStatus currentStatus;
    if (this->getCurrentStatus(&currentStatus, fromISR))
    {
      if (VFOIndex < TRANSCEIVER_VFO_COUNT)
      {
        if (currentStatus.VFO[VFOIndex].frequency != frequency)
        {
          currentStatus.VFO[VFOIndex].frequency = frequency;
          currentStatus.VFO[VFOIndex].currentBandIndex = this->getBandIndex(currentStatus.VFO[VFOIndex].frequency, currentStatus.VFO[VFOIndex].currentBandIndex);
          return (this->setCurrentStatus(&currentStatus, fromISR));
        }
        else
        {
          return (false);
        }
      }
      else
      {
        return (false);
      }
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setActiveVFOFrequency(uint64_t frequency, bool fromISR)
{
  if (frequency >= 0 && (frequency >= MIN_FREQUENCY || frequency <= MAX_FREQUENCY))
  {
    TransceiverStatus currentStatus;
    if (this->getCurrentStatus(&currentStatus, fromISR))
    {
      if (currentStatus.VFO[currentStatus.activeVFOIndex].frequency != frequency)
      {
        currentStatus.VFO[currentStatus.activeVFOIndex].frequency = frequency;
        currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex = this->getBandIndex(currentStatus.VFO[currentStatus.activeVFOIndex].frequency, currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex);
        return (this->setCurrentStatus(&currentStatus, fromISR));
      }
      else
      {
        return (false);
      }
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::incrementActiveVFOFrequency(uint64_t hz, bool fromISR, bool changedByRemote)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    uint64_t hzIncrement = hz * currentStatus.VFO[currentStatus.activeVFOIndex].frequencyStep;
    if (currentStatus.VFO[currentStatus.activeVFOIndex].frequency + hzIncrement <= MAX_FREQUENCY)
    {
      if (!changedByRemote)
      {
        currentStatus.lastFrequencyChangedByLocalControl = millis();
      }
      currentStatus.VFO[currentStatus.activeVFOIndex].frequency += hzIncrement;
      currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex = this->getBandIndex(currentStatus.VFO[currentStatus.activeVFOIndex].frequency, currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex);
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::decrementActiveVFOFrequency(uint64_t hz, bool fromISR, bool changedByRemote)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    uint64_t hzDecrement = hz * currentStatus.VFO[currentStatus.activeVFOIndex].frequencyStep;
    if (currentStatus.VFO[currentStatus.activeVFOIndex].frequency > 0 && hzDecrement <= currentStatus.VFO[currentStatus.activeVFOIndex].frequency)
    {
      if (!changedByRemote)
      {
        currentStatus.lastFrequencyChangedByLocalControl = millis();
      }
      currentStatus.VFO[currentStatus.activeVFOIndex].frequency -= hzDecrement;
      currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex = this->getBandIndex(currentStatus.VFO[currentStatus.activeVFOIndex].frequency, currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex);
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setVFOMode(uint8_t VFOIndex, TrxVFOMode mode, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (VFOIndex < TRANSCEIVER_VFO_COUNT)
    {
      if (currentStatus.VFO[VFOIndex].mode != mode)
      {
        currentStatus.VFO[VFOIndex].mode = mode;
        return (this->setCurrentStatus(&currentStatus, fromISR));
      }
      else
      {
        return (false);
      }
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setCurrentVFOMode(TrxVFOMode mode, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.VFO[currentStatus.activeVFOIndex].mode != mode)
    {
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = mode;
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::toggleActiveVFOMode(bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    switch (currentStatus.VFO[currentStatus.activeVFOIndex].mode)
    {
    case TRX_VFO_MD_CW_L:
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = TRX_VFO_MD_CW_U;
      break;
    case TRX_VFO_MD_CW_U:
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = TRX_VFO_MD_LSB;
      break;
    case TRX_VFO_MD_LSB:
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = TRX_VFO_MD_USB;
      break;
    case TRX_VFO_MD_USB:
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = TRX_VFO_MD_DSB;
      break;
    case TRX_VFO_MD_DSB:
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = TRX_VFO_MD_SAM;
      break;
    case TRX_VFO_MD_SAM:
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = TRX_VFO_MD_FM;
      break;
    case TRX_VFO_MD_FM:
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = TRX_VFO_MD_WFM;
      break;
    case TRX_VFO_MD_WFM:
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = TRX_VFO_MD_BFM;
      break;
    case TRX_VFO_MD_BFM:
    default:
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = TRX_VFO_MD_CW_L;
      break;
    }
    return (this->setCurrentStatus(&currentStatus, fromISR));
  }
  else
  {
    return (false);
  }
}

bool Transceiver::copyVFO(uint8_t sourceIndex, uint8_t destIndex, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (sourceIndex < TRANSCEIVER_VFO_COUNT && destIndex < TRANSCEIVER_VFO_COUNT)
    {
      currentStatus.VFO[destIndex].frequency = currentStatus.VFO[sourceIndex].frequency;
      currentStatus.VFO[destIndex].mode = currentStatus.VFO[sourceIndex].mode;
      currentStatus.VFO[destIndex].frequencyStep = currentStatus.VFO[sourceIndex].frequencyStep;
      currentStatus.VFO[destIndex].filter.LF = currentStatus.VFO[sourceIndex].filter.LF;
      currentStatus.VFO[destIndex].filter.HF = currentStatus.VFO[sourceIndex].filter.HF;
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setVFOFilterLowCut(uint8_t VFOIndex, uint32_t LF, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (VFOIndex < TRANSCEIVER_VFO_COUNT)
    {
      if (currentStatus.VFO[VFOIndex].filter.LF != LF)
      {
        currentStatus.VFO[VFOIndex].filter.LF = LF;
        return (this->setCurrentStatus(&currentStatus, fromISR));
      }
      else
      {
        return (false);
      }
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setCurrentVFOFilterLowCut(uint32_t LF, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.VFO[currentStatus.activeVFOIndex].filter.LF != LF)
    {
      currentStatus.VFO[currentStatus.activeVFOIndex].filter.LF = LF;
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setVFOFilterHighCut(uint8_t VFOIndex, uint32_t HF, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (VFOIndex < TRANSCEIVER_VFO_COUNT)
    {
      if (currentStatus.VFO[VFOIndex].filter.HF != HF)
      {
        currentStatus.VFO[VFOIndex].filter.HF = HF;
        return (this->setCurrentStatus(&currentStatus, fromISR));
      }
      else
      {
        return (false);
      }
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setCurrentVFOFilterHighCut(uint32_t HF, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.VFO[currentStatus.activeVFOIndex].filter.HF != HF)
    {
      currentStatus.VFO[currentStatus.activeVFOIndex].filter.HF = HF;
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setCurrentVFOFilter(uint32_t LF, uint32_t HF, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.VFO[currentStatus.activeVFOIndex].filter.LF != LF || currentStatus.VFO[currentStatus.activeVFOIndex].filter.HF != HF)
    {
      currentStatus.VFO[currentStatus.activeVFOIndex].filter.LF = LF;
      currentStatus.VFO[currentStatus.activeVFOIndex].filter.HF = HF;
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setVFOCustomStep(uint8_t VFOIndex, uint64_t frequencyStep, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (VFOIndex < TRANSCEIVER_VFO_COUNT)
    {
      if (currentStatus.VFO[VFOIndex].frequencyStep != frequencyStep)
      {
        currentStatus.VFO[VFOIndex].frequencyStep = frequencyStep;
        return (this->setCurrentStatus(&currentStatus, fromISR));
      }
      else
      {
        return (false);
      }
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::toggleActiveVFOCustomStep(bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.VFO[currentStatus.activeVFOIndex].frequencyStep < 100000000000)
    {
      currentStatus.VFO[currentStatus.activeVFOIndex].frequencyStep *= 10;
    }
    else
    {
      currentStatus.VFO[currentStatus.activeVFOIndex].frequencyStep = 1;
    }
    return (this->setCurrentStatus(&currentStatus, fromISR));
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setSignalMeter(TRXSMeterUnitType unitType, int8_t units, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    bool changed = false;
    int8_t newLevel = 0;
    switch (unitType)
    {
    case SIGNAL_METER_TS2K_SDR_RADIO_LEVEL:
      newLevel = (units * 3) - 54;
      changed = currentStatus.signalMeterdBLevel != newLevel;
      break;
    case SIGNAL_METER_DB_UNITS:
      newLevel = units;
      changed = currentStatus.signalMeterdBLevel != newLevel;
      break;
    default:
      break;
    }
    if (changed)
    {
      currentStatus.signalMeterdBLevel = newLevel;
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setAFGain(uint8_t value, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.AFGain != value)
    {
      currentStatus.AFGain = value;
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::incrementAFGain(uint8_t units, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.AFGain < 100)
    {
      if ((currentStatus.AFGain + units) <= 100)
      {
        currentStatus.AFGain += units;
      }
      else
      {
        currentStatus.AFGain = 100;
      }
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::decrementAFGain(uint8_t units, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.AFGain > 0)
    {
      if ((currentStatus.AFGain - units) >= 0)
      {
        currentStatus.AFGain -= units;
      }
      else
      {
        currentStatus.AFGain = 0;
      }
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setSquelch(uint8_t value, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.squelch != value)
    {
      currentStatus.squelch = value;
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::incrementSquelch(uint8_t units, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.squelch < 100)
    {
      if ((currentStatus.squelch + units) <= 100)
      {
        currentStatus.squelch += units;
      }
      else
      {
        currentStatus.squelch = 100;
      }
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::decrementSquelch(uint8_t units, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.squelch > 0)
    {
      if ((currentStatus.squelch - units) >= 0)
      {
        currentStatus.squelch -= units;
      }
      else
      {
        currentStatus.squelch = 0;
      }
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::toggleAFSquelchActiveStatus(bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    currentStatus.isVolumeControlActive = !currentStatus.isVolumeControlActive;
    currentStatus.isSquelchControlActive = !currentStatus.isSquelchControlActive;
    return (this->setCurrentStatus(&currentStatus, fromISR));
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setAudioMuted(bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (!currentStatus.audioMuted)
    {
      currentStatus.audioMuted = true;
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setAudioUnMuted(bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.audioMuted)
    {
      currentStatus.audioMuted = false;
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::increaseActiveVFOBand(bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex < RADIO_BANDS_SIZE && RadioBands[currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex + 1].minFrequency <= MAX_FREQUENCY)
    {
      currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex++;
      currentStatus.VFO[currentStatus.activeVFOIndex].frequency = RadioBands[currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex].minFrequency;
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = RadioBands[currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex].modulationMode;
      // TODO: filter
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::decreaseActiveVFOBand(bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex > 1 && RadioBands[currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex - 1].minFrequency >= MIN_FREQUENCY)
    {
      currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex--;
      currentStatus.VFO[currentStatus.activeVFOIndex].frequency = RadioBands[currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex].minFrequency;
      currentStatus.VFO[currentStatus.activeVFOIndex].mode = RadioBands[currentStatus.VFO[currentStatus.activeVFOIndex].currentBandIndex].modulationMode;
      // TODO: filter
      return (this->setCurrentStatus(&currentStatus, fromISR));
    }
    else
    {
      return (false);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::enqueueSyncCommand(TransceiverSyncCommand *trxSyncCmd, bool fromISR)
{
  if (this->syncQueue != nullptr && trxSyncCmd != nullptr)
  {
    // TODO: remove first element if queueSend returns errQUEUE_FULL
    if (!fromISR)
    {
      return (xQueueSend(this->syncQueue, trxSyncCmd, portMAX_DELAY) != pdPASS);
    }
    else
    {
      BaseType_t xHigherPriorityTaskWoken = pdFALSE;
      bool result = xQueueSendFromISR(this->syncQueue, trxSyncCmd, &xHigherPriorityTaskWoken) != pdPASS;
      if (xHigherPriorityTaskWoken == pdTRUE)
      {
        portYIELD_FROM_ISR();
      }
      return (result);
    }
  }
  else
  {
    return (false);
  }
}

bool Transceiver::dequeueSyncCommand(TransceiverSyncCommand *trxSyncCmd, bool fromISR)
{
  if (this->syncQueue != nullptr && trxSyncCmd != nullptr)
  {
    if (!fromISR)
    {
      return (xQueueReceive(this->syncQueue, trxSyncCmd, 1) == pdPASS);
    }
    else
    {
      BaseType_t xHigherPriorityTaskWoken = pdFALSE;
      bool result = xQueueReceiveFromISR(this->syncQueue, trxSyncCmd, &xHigherPriorityTaskWoken) == pdPASS;
      if (xHigherPriorityTaskWoken == pdTRUE)
      {
        portYIELD_FROM_ISR();
      }
      return (result);
    }
  }
  else
  {
    return (false);
  }
}
