#include "Transceiver.hpp"

#define QUEUE_PEEK_MS_TO_TICKS_TIMEOUT 5

Transceiver::Transceiver(void)
{
  this->statusQueue = xQueueCreate(1, sizeof(TransceiverStatus));
  /*
  //this->lockedByControls = false;
  this->poweredOn = false;
  // this->poweredOn = true;
  this->activeVFOIndex = 0;
  this->VFO[0].frequency = 7050123;
  this->VFO[0].mode = TRX_VFO_MD_LSB;
  this->VFO[0].frequencyStep = 1;
  this->VFO[1].frequency = 145625000;
  this->VFO[1].mode = TRX_VFO_MD_FM;
  this->VFO[1].frequencyStep = 1000;
  this->changed = ((uint16_t)1 << 16) - 1; // ALL (uint16_t) flags active
  this->signalMeterdBLevel = 0;
  this->AFGain = 50;
  this->audioMuted = false;
  */
}

// TODO: add xQueuePeekFromISR / xQueueOverwriteFromISR methods
Transceiver::~Transceiver()
{
  vQueueDelete(this->statusQueue);
}

bool Transceiver::getCurrentStatus(TransceiverStatus *status, bool fromISR)
{
  if (this->statusQueue != nullptr)
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
  if (this->statusQueue != nullptr)
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
      currentStatus.changed |= TRX_CFLAG_VFO_INDEX;
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

bool Transceiver::setVFOFrequency(uint8_t VFOIndex, uint64_t frequency, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    if (VFOIndex < TRANSCEIVER_VFO_COUNT)
    {
      currentStatus.VFO[VFOIndex].frequency = frequency;
      if (VFOIndex == currentStatus.activeVFOIndex)
      {
        currentStatus.changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
      }
      else
      {
        currentStatus.changed |= TRX_CFLAG_SECONDARY_VFO_FREQUENCY;
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

bool Transceiver::setActiveVFOFrequency(uint64_t frequency, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    currentStatus.VFO[currentStatus.activeVFOIndex].frequency = frequency;
    currentStatus.changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
    return (this->setCurrentStatus(&currentStatus, fromISR));
  }
  else
  {
    return (false);
  }
}

bool Transceiver::incrementActiveVFOFrequency(uint64_t hz, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    currentStatus.VFO[currentStatus.activeVFOIndex].frequency += hz;
    currentStatus.changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
    return (this->setCurrentStatus(&currentStatus, fromISR));
  }
  else
  {
    return (false);
  }
}

bool Transceiver::decrementActiveVFOFrequency(uint64_t hz, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    currentStatus.VFO[currentStatus.activeVFOIndex].frequency -= hz;
    currentStatus.changed |= TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
    return (this->setCurrentStatus(&currentStatus, fromISR));
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

bool Transceiver::setLockedByControls(bool locked, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    currentStatus.lockedByControls = locked;
    return (this->setCurrentStatus(&currentStatus, fromISR));
  }
  else
  {
    return (false);
  }
}

bool Transceiver::setSignalMeter(TRXSMeterUnitType unitType, uint8_t units, bool fromISR)
{
  TransceiverStatus currentStatus;
  if (this->getCurrentStatus(&currentStatus, fromISR))
  {
    switch (unitType)
    {
    case SIGNAL_METER_TS2K_SDR_RADIO_LEVEL:
      currentStatus.signalMeterTS2KSDRRadioUnits = units;
      currentStatus.signalMeterdBLevel = units * 3;
      break;
    case SIGNAL_METER_DB_UNITS:
      currentStatus.signalMeterTS2KSDRRadioUnits = units / 3;
      currentStatus.signalMeterdBLevel = units;
      break;
    default:
      break;
    }
    currentStatus.changed |= TRX_CFLAG_SIGNAL_METER_DB_LEVEL;
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
    currentStatus.changed |= TRX_CFLAG_AF_GAIN;
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
    currentStatus.changed |= TRX_CFLAG_AF_GAIN;
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
    currentStatus.changed |= TRX_CFLAG_AF_GAIN;
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
    currentStatus.changed |= TRX_CFLAG_AUDIO_MUTE;
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
    currentStatus.changed |= TRX_CFLAG_AUDIO_MUTE;
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
