#ifndef SDR_REMOTE_CONTROL_TRANSCEIVER_SYNC_COMMAND_H
#define SDR_REMOTE_CONTROL_TRANSCEIVER_SYNC_COMMAND_H

#include <cstdint>
#include <cstdbool>

enum TransceiverSyncCommandType
{
    TSCT_NONE,
    TSCT_SET_FREQUENCY,
    TSCT_INCREASE_FREQUENCY,
    TSCT_DECREASE_FREQUENCY
};

class TransceiverSyncCommand
{
private:
    bool booleanValue = false;
    uint64_t uIntValue = 0;

    TransceiverSyncCommandType cmdType = TSCT_NONE;

public:
    TransceiverSyncCommand(TransceiverSyncCommandType cmd, bool value);
    TransceiverSyncCommand(TransceiverSyncCommandType cmd, uint64_t value);

    ~TransceiverSyncCommand();

    TransceiverSyncCommandType getCommandType(void);
    bool getBooleanValue(void);
    uint64_t getUIntValue(void);
};

#endif // SDR_REMOTE_CONTROL_TRANSCEIVER_SYNC_COMMAND_H
