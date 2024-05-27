#ifndef SDR_REMOTE_CONTROL_TS2K_SDRRADIO_PROTOCOL_H
#define SDR_REMOTE_CONTROL_TS2K_SDRRADIO_PROTOCOL_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#define TS2K_MAX_CMD_LENGTH 31

/*
  SDRRADIO PROTOCOL vs Original Kenwood TS-2000 protocol DIFFs

  WARNING
  As read on https://www.sdr-radio.com/SerialPort SDR Radio implementation changed some parts of TS-2000 protocol

  # AF Gain / volume (command "AG")

  SDR Radio TS-2000 protocol volume range is 0...100
  Original Kenwood TS-2000 protocol volume range is 000...255

  # DSP Filter High/Low (command "SH"/"SL")
  
  SDR Radio TS-2000 protocol value (hz) range is 0...99999
  Original Kenwood TS-2000 protocol value (custom) depends current mode 00...11 (each one has fixed hz corresponding values) 
*/

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
void ts2kReadCommandPowerStatus(char* command);

// verify command type
bool ts2kIsPowerStatusCommandResponse(char* commandResponse);

// check if power status command response is "ON"
bool ts2kParsePowerStatusCommandResponse(char* commandResponse);

// get audio mute status
void ts2kReadCommandAudioMute(char* command);

// verify command type
bool ts2kIsAudioMuteCommandResponse(char* commandResponse);

// parse & return audio mute status command response as unsigned integer (8)
uint8_t ts2kParseAudioMuteCommandResponse(char* commandResponse);

// set audio mute status
void ts2kWriteCommandAudioMute(char* command, uint8_t status);

// get AF gain (volume)
void ts2kReadCommandAFGain(char* command);

// verify command type
bool ts2kIsAFGainCommandResponse(char* commandResponse);

// parse & return AF gain command response as unsigned integer (8)
uint8_t ts2kParseAGFGainCommandResponse(char* commandResponse);

// set AF gain (volume)
void ts2kWriteCommandAFGain(char* command, uint8_t volume);

// get current frequency (Hz)
void ts2kReadCommandFrequency(char* command);

// verify command type
bool ts2kIsFrequencyCommandResponse(char* commandResponse);

// parse & return frequency command response as unsigned integer (64)
uint64_t ts2kParseFrequencyCommandResponse(char* commandResponse);

// set current frequency (Hz)
void ts2kWriteCommandFrequency(char* command, uint64_t frequency);

// get current mode
void ts2kReadCommandMode(char* command);

// verify command type
bool ts2kIsModeCommandResponse(char* commandResponse);

// parse & return read current frequency command response as uts2kMode enum
ts2kMode ts2kParseModeCommandResponse(char* commandResponse);

// set current mode
void ts2kWriteCommandMode(char* command, ts2kMode mode);

// get current signal meter level
void ts2kReadCommandSignalMeterLevel(char* command);

// verify command type
bool ts2kIsSignalMeterLevelCommandResponse(char* commandResponse);

// parse & return signal meter level command response as unsigned integer (16)
uint16_t ts2kParseSignalMeterLevelCommandResponse(char* commandResponse);

// get current filter high value (Hz)
void ts2kReadCommandFilterHighHz(char* command);

// verify command type
bool ts2kIsFilterHighCommandResponse(char* commandResponse);

// parse & return filter high command response as unsigned integer (32)
uint32_t ts2kParseFilterHighCommandResponse(char* commandResponse);

// set current filter high value (Hz)
void ts2kWriteCommandFilterHighHz(char* command, uint32_t hz);

// get current filter low value (Hz)
void ts2kReadCommandFilterLowHz(char* command);

// verify command type
bool ts2kIsFilterLowCommandResponse(char* commandResponse);

// parse & return filter low command response as unsigned integer (32)
uint32_t ts2kParseFilterLowCommandResponse(char* commandResponse);

// set current filter low value (Hz)
void ts2kWriteCommandFilterLowHz(char* command, uint32_t hz);

#endif