#ifndef SDR_REMOTE_CONTROL_SDRRADIO_TS2K_SERIAL_CONNECTION_H
#define SDR_REMOTE_CONTROL_SDRRADIO_TS2K_SERIAL_CONNECTION_H

#include "../SerialConnection.hpp"

class SDRRadioTS2KSerialConnection : public SerialConnection
{
private:
    uint64_t lastFrequency = 0;
    TrxVFOMode lastMode = TRX_VFO_MODE_ERROR;
    uint32_t lastLFFilter = 0;
    uint32_t lastHFFilter = 0;
    uint8_t lastAFGain = 0;
    bool lastMutedStatus = false;
    int8_t lastSMeterUnits = 0;

public:
    SDRRadioTS2KSerialConnection(HardwareSerial *serialPort, long speed, long timeout);
    ~SDRRadioTS2KSerialConnection();

    void loop(Transceiver *trx) override;

    bool tryConnection(Transceiver *trx) override;
};

#endif // SDR_REMOTE_CONTROL_SDRRADIO_TS2K_SERIAL_CONNECTION_H
