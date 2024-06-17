#ifndef SDR_REMOTE_CONTROL_SDRRADIO_TS2K_SERIAL_CONNECTION_H
#define SDR_REMOTE_CONTROL_SDRRADIO_TS2K_SERIAL_CONNECTION_H

#include <Arduino.h>
#include <String.h>
#include "../SerialConnection.hpp"

class SDRRadioTS2KSerialConnection : public SerialConnection
{
public:
    SDRRadioTS2KSerialConnection(HardwareSerial *serialPort, long speed, long timeout);
    ~SDRRadioTS2KSerialConnection() {}

    void loop(Transceiver *trx) override;

protected:
    bool tryConnection(Transceiver *trx) override;
};

#endif