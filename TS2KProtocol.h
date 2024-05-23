#ifndef SDR_REMOTE_CONTROL_TS2K_H
#define SDR_REMOTE_CONTROL_TS2K_H

#include <stdint.h>
#include <stddef.h>

#define TS2K_CMD_TERMINATOR ";"
#define TS2K_CMD_POWER_STATUS "PS"
#define TS2K_CMD_POWER_STATUS_RESPONSE_ON "PS1"

typedef enum {
  TS2K_CMD_READ_FREQUENCY_VFO_1,  // frequency in Hz    ("FA;")
  TS2K_CMD_READ_MODE_VFO_1,       // mode               ("MD;")
  TS2K_CMD_READ_SMETER_VFO_1,     // smeter             ("SM0;")
  TS2K_CMD_READ_AGC,               // AGC                ("GT;")
  TS2K_CMD_READ_FILTER_LOW,        // Filter low in Hz   ("SL;")
  TS2K_CMD_READ_FILTER_HIGH,       // Filter high in Hz  ("SH;")
  TS2K_CMD_READ_VOLUME,            // AF Gain            ("AG;")
  TS2K_CMD_READ_CURRENT_SAMPLE_RATE // AF Gain            ("SA;")
} TS2K_RX_CMD;

typedef enum {
  TS2K_AGC_OFF = 0,
  TS2K_AGC_FAST = 1,
  TS2K_AGC_MEDIUM = 2,
  TS2K_AGC_SLOW = 3
} TS2K_AGC;

class TS2KProtocol {
public:
  const char* getReadCommandString(TS2K_RX_CMD command);
  const char* getVFOFrequencyCmdStr(void);
  const char* getVFOModeCmdStr(void);
  const char* getVFOSMeterCmdStr(void);
  const char* getAGCCmdStr(void);
  const char* getFilterLowHzCmdStr(void);
  const char* getFilterHighHzCmdStr(void);
  const char* getReadVolumeCmdStr(void);
  const char* getReadCurrentSampleRateCmdStr(void);
private:
};

#endif