#ifndef SDR_REMOTE_CONTROL_SDRRADIO_DUMMY_CONNECTION_H
#define SDR_REMOTE_CONTROL_SDRRADIO_DUMMY_CONNECTION_H

#include "../SerialConnection.hpp"

class DummyConnection : public SerialConnection
{
private:
    uint64_t oldFrequency = 0;
    uint8_t oldSMeter = 0;

public:
    DummyConnection(HardwareSerial *serialPort, long speed, long timeout);
    ~DummyConnection() {}

    void loop(Transceiver *trx) override;

    bool tryConnection(Transceiver *trx) override;
};

#endif // SDR_REMOTE_CONTROL_SDRRADIO_DUMMY_CONNECTION_H
