#include "TS2KProtocol.h"

const char* TS2KProtocol::getReadCommandString(TS2K_RX_CMD command) {
  switch (command) {
    case TS2K_CMD_READ_FREQUENCY_VFO_1:
      return "FA;";
    case TS2K_CMD_READ_MODE_VFO_1:
      return "MD;";
    case TS2K_CMD_READ_SMETER_VFO_1:
      return "SM;";
    case TS2K_CMD_READ_AGC:
      return "GT;";
    case TS2K_CMD_READ_FILTER_LOW:
      return "SL;";
    case TS2K_CMD_READ_FILTER_HIGH:
      return "SH;";
    case TS2K_CMD_READ_VOLUME:
      return "AG;";
    case TS2K_CMD_READ_CURRENT_SAMPLE_RATE:
      return "SA;";
    default:
      return "";
      break;
  }
}

const char* getVFOFrequencyCmdStr(void) {
  return "FA;";
}

const char* getVFOModeCmdStr(void) {
  return "MD;";
}

const char* getVFOSMeterCmdStr(void) {
  return "SM0;";
}
const char* getAGCCmdStr(void) {
  return "GT;";
}

const char* getFilterLowHzCmdStr(void) {
  return "SL;";
}

const char* getFilterHighHzCmdStr(void) {
  return "SH;";
}

const char* getReadVolumeCmdStr(void) {
  return "AG;";
}

const char* getReadCurrentSampleRateCmdStr(void) {
  return "SA;";
}