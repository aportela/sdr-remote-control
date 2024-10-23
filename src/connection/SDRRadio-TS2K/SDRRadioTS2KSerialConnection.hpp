#ifndef SDR_REMOTE_CONTROL_SDRRADIO_TS2K_SERIAL_CONNECTION_H
#define SDR_REMOTE_CONTROL_SDRRADIO_TS2K_SERIAL_CONNECTION_H

#include "../SerialConnection.hpp"

class SDRRadioTS2KSerialConnection : public SerialConnection
{
private:
    TransceiverStatus *trxStatusPtr = nullptr;

public:
    SDRRadioTS2KSerialConnection(HardwareSerial *serialPort, long speed, long timeout);
    ~SDRRadioTS2KSerialConnection();

    void loop(Transceiver *trx, const TransceiverStatus *currentTrxStatus = nullptr) override;

    void syncLocalToRemote(Transceiver *trx, const TransceiverStatus *currentTrxStatus = nullptr) override;
    bool tryConnection(Transceiver *trx) override;
};

#endif // SDR_REMOTE_CONTROL_SDRRADIO_TS2K_SERIAL_CONNECTION_H
