#include "ts2k_sdrradio_protocol.h"

/*
  SDRRADIO PROTOCOL vs Original Kenwood TS-2000 protocol DIFFs

  WARNING
  As read on https://www.sdr-radio.com/SerialPort SDR Radio implementation changed some parts of TS-2000 protocol

  # AF Gain (volume)

  SDR Radio TS-2000 protocol volume range is 0...100
  Original Kenwood TS-2000 protocol volume range is 000...255

  # DSP Filter High/Low (SH/SL)
  
  SDR Radio TS-2000 protocol value (hz) range is 0...99999
  Original Kenwood TS-2000 protocol value (custom) depends current mode 00...11 (each one has fixed hz corresponding values)

*/

// get AF gain (volume)
void ts2kReadCommandGetAFGain(char* command) {
  strcpy(command, "AG0;");
}

// set AF gain (volume)
void ts2kWriteCommandSetAFGain(char* command, uint8_t volume) {
  sprintf(command, "AG0%02d;", volume < 100 ? volume : 100);
}

void ts2kReadCommandGetFrequency(char* command) {
  strcpy(command, "FA;");
}

void ts2kWriteCommandSetFrequency(char* command, uint64_t frequency) {
  sprintf(command, "FA%011llu;", frequency);
}

void ts2kReadCommandGetMode(char* command) {
  strcpy(command, "MD;");
}

void ts2kWriteCommandSetMode(char* command, ts2kMode mode) {
  sprintf(command, "MD%d;", (uint8_t)mode);
}

void ts2kReadCommandGetSignalMeterLevel(char* command) {
  strcpy(command, "SM0;");
}

void ts2kReadCommandGetFilterHighHz(char* command) {
  strcpy(command, "SH;");
}

void ts2kWriteCommandSetFilterHighHz(char* command, uint32_t hz) {
  sprintf(command, "Sh%05u;", hz);
}

void ts2kReadCommandGetFilterLowHz(char* command) {
  strcpy(command, "SL;");
}

void ts2kWriteCommandSetFilterLowHz(char* command, uint32_t hz) {
  sprintf(command, "SL%05u;", hz);
}
