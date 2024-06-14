#ifndef SDR_REMOTE_CONTROL_SERIAL_CONNECTION_H
#define SDR_REMOTE_CONTROL_SERIAL_CONNECTION_H

#include <Arduino.h>
#include <String.h>
#include "ISerialConnection.hpp"

#define MILLISECONDS_BETWEEN_LOOP 10 // WARNING: this (10ms) transfer/process THOUSANDS of commands per second, set a higher value if you notice saturation on the serial port (data refresh rates will be reduced)
#define MILLISECONDS_WAITED_AFTER_FLUSH 2
#define MILLISECONDS_WAITED_AFTER_SEND 5
#define MILLISECONDS_TO_WAIT_BEFORE_TIMEOUT 5000

class SerialConnection : public ISerialConnection
{
public:
    // TODO: party bits ?
    SerialConnection(HardwareSerial *serialPort, long speed, long timeout);
    ~SerialConnection() {}

    virtual void loop(Transceiver *trx) = 0;
    bool isDisconnectedByTimeout(void);

protected:
    HardwareSerial *serial;
    uint64_t lastRXActivity = 0;
    uint64_t lastRXValidCommand = 0;
    uint64_t lastTXActivity = 0;
    void flush(void);
    void send(String str);
    virtual bool tryConnection(void) = 0;
};

#endif