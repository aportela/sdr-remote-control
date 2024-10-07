#ifndef SDR_REMOTE_CONTROL_SDRRADIO_TS2K_SERIAL_CONNECTION_H
#define SDR_REMOTE_CONTROL_SDRRADIO_TS2K_SERIAL_CONNECTION_H

#include "../SerialConnection.hpp"

class DummyConnection : public SerialConnection
{
private:
    uint64_t oldFrequency = 0;
    uint8_t oldSMeter = 0;

public:
    DummyConnection(HardwareSerial *serialPort, long speed, long timeout);
    ~DummyConnection() {}

    void loop(Transceiver *trx, const TransceiverStatus *currentTrxStatus = nullptr) override;

    void syncLocalToRemote(Transceiver *trx, const TransceiverStatus *currentTrxStatus = nullptr) override;
    bool tryConnection(Transceiver *trx) override;
};

#endif