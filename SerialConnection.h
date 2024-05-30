#ifndef SDR_REMOTE_CONTROL_SERIAL_CONNECTION_H
#define SDR_REMOTE_CONTROL_SERIAL_CONNECTION_H

class SerialConnection {
public:
  SerialConnection(long speed, long timeout);

  bool tryConnection(void);

  void send(String str);

  String loop(void);

  uint64_t lastRXActivity = 0;
private:
  void flush(void);
};

#endif