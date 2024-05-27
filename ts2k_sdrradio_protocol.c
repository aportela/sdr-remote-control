#include "ts2k_sdrradio_protocol.h"

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

// get power status
void ts2kReadCommandPowerStatus(char* command) {
  strcpy(command, "PS;");
}

// verify command type
bool ts2kIsPowerStatusCommandResponse(char* commandResponse) {
  return strlen(commandResponse) == 3 && strncmp(commandResponse, "PS", 2) == 0;
}

// check if power status command response is "ON"
bool ts2kParsePowerStatusCommandResponse(char* commandResponse) {
  return strcmp(commandResponse, "PS1") == 0;
}


// get audio mute status
void ts2kReadCommandAudioMute(char* command) {
  strcpy(command, "MU;");
}

// verify command type
bool ts2kIsAudioMuteCommandResponse(char* commandResponse) {
  return strlen(commandResponse) == 3 && strncmp(commandResponse, "MU", 2) == 0;
}

// parse & return audio mute status command response as unsigned integer (8)
uint8_t ts2kParseAudioMuteCommandResponse(char* commandResponse) {
  uint8_t status;
  if (sscanf(commandResponse, "MU%u", &status) == 1) {
    return status;
  } else {
    return 0;
  }
}

// set audio mute status
void ts2kWriteCommandAudioMute(char* command, uint8_t status) {
  sprintf(command, "MU%01d;", status == 1 ? 1 : 0);
}

// get AF gain (volume)
void ts2kReadCommandAFGain(char* command) {
  strcpy(command, "AG0;");
}

// verify command type
bool ts2kIsAFGainCommandResponse(char* commandResponse) {
  return strlen(commandResponse) == 5 && strncmp(commandResponse, "AG", 2) == 0;
}

// parse & return AF gain command response as unsigned integer
uint8_t ts2kParseAGFGainCommandResponse(char* commandResponse) {
  uint8_t volume;
  if (sscanf(commandResponse, "AG%u", &volume) == 1) {
    return volume;
  } else {
    return 0;
  }
}

// set AF gain (volume)
void ts2kWriteCommandAFGain(char* command, uint8_t volume) {
  sprintf(command, "AG0%03d;", volume < 100 ? volume : 100);
}

// get current frequency (Hz)
void ts2kReadCommandFrequency(char* command) {
  strcpy(command, "FA;");
}

// verify command type
bool ts2kIsFrequencyCommandResponse(char* commandResponse) {
  return strlen(commandResponse) == 13 && strncmp(commandResponse, "FA", 2) == 0;
}

// parse & return frequency command response as unsigned integer (64)
uint64_t ts2kParseFrequencyCommandResponse(char* commandResponse) {
  uint64_t frequency;
  if (sscanf(commandResponse, "FA%llu", &frequency) == 1) {
    return frequency;
  } else {
    return 0;
  }
}

// set current frequency (Hz)
void ts2kWriteCommandFrequency(char* command, uint64_t frequency) {
  sprintf(command, "FA%011llu;", frequency);
}

// get current mode
void ts2kReadCommandMode(char* command) {
  strcpy(command, "MD;");
}

// verify command type
bool ts2kIsModeCommandResponse(char* commandResponse) {
  return strlen(commandResponse) == 3 && strncmp(commandResponse, "MD", 2) == 0;
}

// parse & return read current frequency command response as uts2kMode enum
ts2kMode ts2kParseModeCommandResponse(char* commandResponse) {
  ts2kMode mode;
  if (sscanf(commandResponse, "MD%u", &mode) == 1) {
    return mode < 9 ? mode : TS2K_MODE_ERROR;
  } else {
    return TS2K_MD_RESERVED;
  }
}

// set current mode
void ts2kWriteCommandMode(char* command, ts2kMode mode) {
  sprintf(command, "MD%d;", (uint8_t)mode);
}

// get current signal meter level
void ts2kReadCommandSignalMeterLevel(char* command) {
  strcpy(command, "SM0;");
}

// verify command type
bool ts2kIsSignalMeterLevelCommandResponse(char* commandResponse) {
  return strlen(commandResponse) == 4 && strncmp(commandResponse, "SM", 2) == 0;
}

// parse & return signal meter level command response as unsigned integer (16)
uint16_t ts2kParseSignalMeterLevelCommandResponse(char* commandResponse) {
  uint16_t level;
  if (sscanf(commandResponse, "SM%u", &level) == 1) {
    return level;
  } else {
    return 0;
  }
}

// get current filter high value (Hz)
void ts2kReadCommandFilterHighHz(char* command) {
  strcpy(command, "SH;");
}

// verify command type
bool ts2kIsFilterHighCommandResponse(char* commandResponse) {
  return strlen(commandResponse) == 7 && strncmp(commandResponse, "SH", 2) == 0;
}

// parse & return filter high command response as unsigned integer (32)
uint32_t ts2kParseFilterHighCommandResponse(char* commandResponse) {
  uint16_t hz;
  if (sscanf(commandResponse, "SH%u", &hz) == 1) {
    return hz;
  } else {
    return 0;
  }
}

// set current filter high value (Hz)
void ts2kWriteCommandFilterHighHz(char* command, uint32_t hz) {
  sprintf(command, "Sh%05u;", hz);
}

// get current filter low value (Hz)
void ts2kReadCommandFilterLowHz(char* command) {
  strcpy(command, "SL;");
}

// verify command type
bool ts2kIsFilterLowCommandResponse(char* commandResponse) {
  return strlen(commandResponse) == 7 && strncmp(commandResponse, "SL", 2) == 0;
}

// parse & return filter low command response as unsigned integer (32)
uint32_t ts2kParseFilterLowCommandResponse(char* commandResponse) {
  uint16_t hz;
  if (sscanf(commandResponse, "SL%u", &hz) == 1) {
    return hz;
  } else {
    return 0;
  }
}

// set current filter low value (Hz)
void ts2kWriteCommandFilterLowHz(char* command, uint32_t hz) {
  sprintf(command, "SL%05u;", hz);
}
