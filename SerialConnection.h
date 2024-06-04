#ifndef SDR_REMOTE_CONTROL_SERIAL_CONNECTION_H
#define SDR_REMOTE_CONTROL_SERIAL_CONNECTION_H

#include <Arduino.h>
#include <String.h>
#include "Transceiver.h"

class SerialConnection {
public:
  SerialConnection(HardwareSerial* serialPort, long speed, long timeout);

  bool tryConnection(void);

  void send(String str);

  void loop(Transceiver* trx);

  uint64_t lastRXActivity = 0;
  uint64_t lastRXValidCommand = 0;
  uint64_t lastTXActivity = 0;
private:
  HardwareSerial* serial;
  void flush(void);
};

#endif