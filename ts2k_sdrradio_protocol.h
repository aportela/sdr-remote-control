#ifndef SDR_REMOTE_CONTROL_TS2K_SDRRADIO_PROTOCOL_H
#define SDR_REMOTE_CONTROL_TS2K_SDRRADIO_PROTOCOL_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define TS2K_MAX_CMD_LENGTH 31

typedef enum {
  TS2K_MD_DSB = 0,
  TS2K_MD_LSB = 1,
  TS2K_MD_USB = 2,
  // CW upper sideband
  TS2K_MD_CW_U = 3,
  TS2K_MD_FM = 4,
  // synchronous AM, includes ECSS
  TS2K_MD_SAM = 5,
  TS2K_MD_RESERVED = 6,
  // CW lower sideband
  TS2K_MD_CW_L = 7,
  TS2K_MD_WFM = 8,
  TS2K_MD_BFM = 9,
  TS2K_MODE_ERROR = 10
} ts2kMode;

// get power status
void ts2kReadCommandGetPowerStatus(char* command);

// get AF gain (volume)
void ts2kReadCommandGetAFGain(char* command);

// set AF gain (volume)
void ts2kWriteCommandSetAFGain(char* command, uint8_t volume);

// get current frequency (Hz)
void ts2kReadCommandGetFrequency(char* command);

// set current frequency (Hz)
void ts2kWriteCommandSetFrequency(char* command, uint64_t frequency);

// get current mode
void ts2kReadCommandGetMode(char* command);

// set current mode
void ts2kWriteCommandSetMode(char* command, ts2kMode mode);

// get current signal meter level
void ts2kReadCommandGetSignalMeterLevel(char* command);

// get current filter high value (Hz)
void ts2kReadCommandGetFilterHighHz(char* command);

// set current filter high value (Hz)
void ts2kWriteCommandSetFilterHighHz(char* command, uint32_t hz);

// get current filter low value (Hz)
void ts2kReadCommandGetFilterLowHz(char* command);

// set current filter low value (Hz)
void ts2kWriteCommandSetFilterLowHz(char* command, uint32_t hz);

#endif