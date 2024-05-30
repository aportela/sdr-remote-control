#ifndef SDR_REMOTE_CONTROL_SERIAL_CONNECTION_H
#define SDR_REMOTE_CONTROL_SERIAL_CONNECTION_H

#include <Arduino.h>

class SerialConnection {
public:
  SerialConnection(HardwareSerial* serialPort, long speed, long timeout);

  bool tryConnection(void);

  void send(String str);

  String loop(void);

  uint64_t lastRXActivity = 0;
private:
  HardwareSerial* serial;
  void flush(void);
};

#endif