#ifndef SDR_REMOTE_CONTROL_TRANSCEIVER_H
#define SDR_REMOTE_CONTROL_TRANSCEIVER_H

#include <cstdint>
#include <cstdbool>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "TransceiverSyncCommand.hpp"
#include "CommonDefines.hpp"

#define TRANSCEIVER_VFO_COUNT 2

#define MIN_AF_GAIN 0
#define MAX_AF_GAIN 100
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
  uint16_t currentBandIndex = 0;
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

#define USE_SMOOTH_SIGNAL_METER

struct TransceiverStatus
{

  bool poweredOn = false;
  char radioName[32] = "unknown";
  uint8_t activeVFOIndex = 0;
  TrxVFO VFO[TRANSCEIVER_VFO_COUNT];
  int8_t signalMeterdBLevel = -54;
#ifdef USE_SMOOTH_SIGNAL_METER
  int8_t smoothedSignalMeterdBLevel = -54;
  uint64_t lastSmoothSignalMeterdBTimer = 0;
#endif
  uint8_t AFGain = 0;
  uint8_t squelch = 0;
  bool audioMuted = false;
  uint64_t lastFrequencyChangedByLocalControl = 0;
  uint64_t lastVolumeChangedByUser = 0;
  bool isVolumeControlActive = true;
  bool isSquelchControlActive = false;
  TransceiverStatus() = default;
};

enum RadioBandType
{
  RBT_NONE,
  RBT_AMATEUR,
  RBT_BROADCAST
};

// https://www.iaru-r1.org/about-us/organisation-and-history/regions/
enum RadioBandRegion
{
  RBR_REGION_NONE, //
  RBR_REGION_1,    // Africa, Europe, Middle East, and northern Asia
  RBR_REGION_2,    // America
  RBR_REGION_3,    // The rest of Asia and the Pacific
};

struct RadioBand
{
  RadioBandRegion region;
  RadioBandType type;
  char label[32];
  uint64_t minFrequency;
  uint64_t maxFrequency;
  TrxVFOMode modulationMode;
};

// https://www.iaru-r1.org/wp-content/uploads/2021/06/hf_r1_bandplan.pdf
// ALSO https://en.wikipedia.org/wiki/Shortwave_bands (for broadcast)
// TODO: SPLIT RANGES / MODES
const RadioBand RadioBands[] = {
    {RBR_REGION_NONE, RBT_NONE, "", 0, 0, TRX_VFO_MD_RESERVED},
#ifndef IARU_REGION
#elif IARU_REGION == 1
    {RBR_REGION_1, RBT_AMATEUR, "160m", 1810000, 2000000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "120m", 2300000, 2495000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "90m", 3200000, 3400000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "80m", 3500000, 3800000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "75m", 3900000, 4000000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "60m", 4750000, 4995000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "60m", 5351500, 5366500, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "49m", 5900000, 6200000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "41m", 7200000, 7450000, TRX_VFO_MD_RESERVED}, // TODO: conflict with AMATEUR 40m!!! (WARNING)
    {RBR_REGION_1, RBT_AMATEUR, "40m", 7000000, 7300000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "31m", 9400000, 9900000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "30m", 10100000, 10150000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "25m", 11600000, 12100000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "22m", 13570000, 13870000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "20m", 14000000, 14350000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "19m", 15100000, 15830000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "17m", 18068000, 18168000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "16m", 17480000, 17900000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "15m", 18900000, 19020000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "15m", 21000000, 21450000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "13m", 21450000, 21850000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "12m", 24890000, 24990000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_BROADCAST, "11m", 25670000, 26100000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "10m", 28000000, 29700000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "6m", 50000000, 54000000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "4m", 70000000, 70500000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "2m", 144000000, 146000000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "70cm", 430000000, 440000000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "23cm", 1240000000, 1300000000, TRX_VFO_MD_RESERVED},
    {RBR_REGION_1, RBT_AMATEUR, "13cm", 2300000000, 2450000000, TRX_VFO_MD_RESERVED},
#elif IARU_REGION == 2
#elif IARU_REGION == 3
#endif
};

#define RADIO_BANDS_SIZE (sizeof(RadioBands) / sizeof(RadioBands[0]))

class Transceiver
{
private:
  QueueHandle_t statusQueue;
  QueueHandle_t syncQueue;

  uint16_t getBandIndex(uint64_t currentFrequency, uint16_t currentBandIndex);

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
  bool setCurrentVFOMode(TrxVFOMode mode, bool fromISR = false);
  bool toggleActiveVFOMode(bool fromISR = false);
  bool copyVFO(uint8_t sourceIndex, uint8_t destIndex, bool fromISR = false);
  bool setVFOFilterLowCut(uint8_t VFOIndex, uint32_t LF, bool fromISR = false);
  bool setCurrentVFOFilterLowCut(uint32_t LF, bool fromISR = false);
  bool setVFOFilterHighCut(uint8_t VFOIndex, uint32_t HF, bool fromISR = false);
  bool setCurrentVFOFilterHighCut(uint32_t HF, bool fromISR = false);
  bool setCurrentVFOFilter(uint32_t LF, uint32_t HF, bool fromISR = false);
  bool setVFOCustomStep(uint8_t VFOIndex, uint64_t frequencyStep, bool fromISR = false);
  bool toggleActiveVFOCustomStep(bool fromISR = false);

  bool setSignalMeter(TRXSMeterUnitType unitType, int8_t units, bool fromISR = false);

  bool setAFGain(uint8_t value, bool fromISR = false);
  bool incrementAFGain(uint8_t units, bool fromISR = false);
  bool decrementAFGain(uint8_t units, bool fromISR = false);

  bool setSquelch(uint8_t value, bool fromISR = false);
  bool incrementSquelch(uint8_t units, bool fromISR = false);
  bool decrementSquelch(uint8_t units, bool fromISR = false);

  bool toggleAFSquelchActiveStatus(bool fromISR = false);

  bool setAudioMuted(bool fromISR = false);
  bool setAudioUnMuted(bool fromISR = false);

  bool increaseActiveVFOBand(bool fromISR = false);
  bool decreaseActiveVFOBand(bool fromISR = false);

  bool enqueueSyncCommand(TransceiverSyncCommand *trxSyncCmd = nullptr, bool fromISR = false);

  bool dequeueSyncCommand(TransceiverSyncCommand *trxSyncCmd = nullptr, bool fromISR = false);
};

#endif // SDR_REMOTE_CONTROL_TRANSCEIVER_H
