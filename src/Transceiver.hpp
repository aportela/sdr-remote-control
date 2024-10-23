#ifndef SDR_REMOTE_CONTROL_TRANSCEIVER_H
#define SDR_REMOTE_CONTROL_TRANSCEIVER_H

#include <cstdint>
#include <cstdbool>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#define TRANSCEIVER_VFO_COUNT 2

#define MIN_FREQUENCY 150000 // 150 Khz (FUNCUBE DONGLE PRO +)
#define MAX_FREQUENCY 999999999999

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
#define TRX_CFLAG_SIGNAL_METER_DB_LEVEL (1 << 13)        // 8192
#define TRX_CFLAG_AUDIO_MUTE (1 << 14)                   // 16384
#define TRX_CFLAG_AF_GAIN (1 << 15)                      // 32768
#define TRX_CFLAG_SQUELCH (1 << 16)                      // 65536
#define TRX_CFLAG_SEND_CAT (1 << 17)                     // 131072

// SDR Radio Console (by Simon Brown / G4ELI) Serial CAT https://www.sdr-radio.com/SerialPort
enum TrxVFOMode
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
};

struct TrxVFOFilter
{
  uint32_t LF = 0; // low filter
  uint32_t HF = 0; // high filter
};

struct TrxVFO
{
  uint64_t frequency = 0;
  TrxVFOMode mode = TRX_VFO_MODE_ERROR;
  uint64_t frequencyStep = 1; // hz
  TrxVFOFilter filter;
  TrxVFO() = default;
};

enum TRXSMeterUnitType
{
  SIGNAL_METER_TS2K_SDR_RADIO_LEVEL = 1,
  SIGNAL_METER_DB_UNITS = 2
};

struct TransceiverStatus
{

  bool poweredOn = false;
  char radioName[32] = "unknown";
  uint8_t activeVFOIndex = 0;
  TrxVFO VFO[TRANSCEIVER_VFO_COUNT];
  int8_t signalMeterdBLevel = -54;
  uint8_t AFGain = 0;
  uint8_t squelch = 0;
  bool audioMuted = false;
  uint64_t lastFrequencyChangedByLocalControl = 0;
  uint64_t lastVolumeChangedByUser = 0;
  TransceiverStatus() = default;
};

class Transceiver
{
private:
  QueueHandle_t statusQueue;

public:
  Transceiver();

  ~Transceiver();

  bool getCurrentStatus(TransceiverStatus *status, bool fromISR = false);
  bool setCurrentStatus(const TransceiverStatus *status, bool fromISR = false);

  bool setPowerOnStatus(bool powerOnStatus, bool fromISR = false);
  bool setRadioName(const char *radioName, bool fromISR = false);
  bool toggleActiveVFO(bool fromISR = false);
  bool setActiveVFO(uint8_t VFOIndex, bool fromISR = false);
  bool setVFOFrequency(uint8_t VFOIndex, uint64_t frequency, bool fromISR = false);
  bool setActiveVFOFrequency(uint64_t frequency, bool fromISR = false);
  bool incrementActiveVFOFrequency(uint64_t hz, bool fromISR = false, bool changedByRemote = false);
  bool decrementActiveVFOFrequency(uint64_t hz, bool fromISR = false, bool changedByRemote = false);
  bool setVFOMode(uint8_t VFOIndex, TrxVFOMode mode, bool fromISR = false);
  bool toggleActiveVFOMode(bool fromISR = false);
  bool copyVFO(uint8_t sourceIndex, uint8_t destIndex, bool fromISR = false);
  bool setVFOFilterLowCut(uint8_t VFOIndex, uint32_t LF, bool fromISR = false);
  bool setVFOFilterHighCut(uint8_t VFOIndex, uint32_t HF, bool fromISR = false);
  bool setVFOCustomStep(uint8_t VFOIndex, uint64_t frequencyStep, bool fromISR = false);
  bool toggleActiveVFOCustomStep(bool fromISR = false);

  bool setSignalMeter(TRXSMeterUnitType unitType, int8_t units, bool fromISR = false);

  bool setAFGain(uint8_t value, bool fromISR = false);
  bool incrementAFGain(uint8_t units, bool fromISR = false);
  bool decrementAFGain(uint8_t units, bool fromISR = false);

  bool setSquelch(uint8_t value, bool fromISR = false);
  bool incrementSquelch(uint8_t units, bool fromISR = false);
  bool decrementSquelch(uint8_t units, bool fromISR = false);

  bool setAudioMuted(bool fromISR = false);
  bool setAudioUnMuted(bool fromISR = false);

  bool increaseActiveVFOBand(bool fromISR = false);
  bool decreaseActiveVFOBand(bool fromISR = false);
};

#endif // SDR_REMOTE_CONTROL_TRANSCEIVER_H
