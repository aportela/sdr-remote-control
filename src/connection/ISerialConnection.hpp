#ifndef SDR_REMOTE_CONTROL_SERIAL_CONNECTION_INTERFACE
#define SDR_REMOTE_CONTROL_SERIAL_CONNECTION_INTERFACE

#include <stdint.h>

#include "../Transceiver.hpp"

class ISerialConnection
{
public:
    virtual ~ISerialConnection() {}
    virtual void loop(Transceiver *trx, const TransceiverStatus *currentTrxStatus) = 0;
    virtual void syncLocalToRemote(Transceiver *trx, const TransceiverStatus *currentTrxStatus) = 0;
};

#endif