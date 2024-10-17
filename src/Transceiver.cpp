#include "Transceiver.hpp"
#include <cstdint>

#include <Arduino.h>

#define QUEUE_PEEK_MS_TO_TICKS_TIMEOUT 5

Transceiver::Transceiver(void)
{
  this->statusQueue = xQueueCreate(1, sizeof(TransceiverStatus));
  TransceiverStatus trxStatus;
  this->setCurrentStatus(&trxStatus, false);
}

Transceiver::~Transceiver()
{
  vQueueDelete(this->statusQueue);
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

        currentStatus.VFO[VFOIndex].frequency = frequency;
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

bool Transceiver::setActiveVFOFrequency(uint64_t frequency, bool fromISR)
{
  if (frequency >= 0 && (frequency >= MIN_FREQUENCY || frequency <= MAX_FREQUENCY))
  {
    TransceiverStatus currentStatus;
    if (this->getCurrentStatus(&currentStatus, fromISR))
    {
      currentStatus.VFO[currentStatus.activeVFOIndex].frequency = frequency;
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

bool Transceiver::setVFOFilterLowCut(uint8_t VFOIndex, uint32_t LF, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (VFOIndex < TRANSCEIVER_VFO_COUNT)
    {
      currentStatus.VFO[VFOIndex].LF = LF;
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
      currentStatus.VFO[VFOIndex].HF = HF;
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
    switch (unitType)
    {
    case SIGNAL_METER_TS2K_SDR_RADIO_LEVEL:
      currentStatus.signalMeterdBLevel = (units * 3) - 54;
      break;
    case SIGNAL_METER_DB_UNITS:
      currentStatus.signalMeterdBLevel = units;
      break;
    default:
      break;
    }
    return (this->setCurrentStatus(&currentStatus, fromISR));
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
    currentStatus.AFGain = value;
    return (this->setCurrentStatus(&currentStatus, fromISR));
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

bool Transceiver::decrementAFGain(uint8_t units, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
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

bool Transceiver::setSquelch(uint8_t value, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    currentStatus.squelch = value;
    return (this->setCurrentStatus(&currentStatus, fromISR));
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

bool Transceiver::decrementSquelch(uint8_t units, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
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

bool Transceiver::setAudioMuted(bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    currentStatus.audioMuted = true;
    return (this->setCurrentStatus(&currentStatus, fromISR));
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
    currentStatus.audioMuted = false;
    return (this->setCurrentStatus(&currentStatus, fromISR));
  }
  else
  {
    return (false);
  }
}

void Transceiver::incSerialCommandCount(void)
{
  this->serialCommandCount++;
}

uint64_t Transceiver::getSerialCommandCount(void)
{
  return (this->serialCommandCount);
}
