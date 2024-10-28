#include "TransceiverSyncCommand.hpp"

TransceiverSyncCommand::TransceiverSyncCommand()
{
}

TransceiverSyncCommand::TransceiverSyncCommand(TransceiverSyncCommandType cmd)
{
    this->cmdType = cmd;
}

TransceiverSyncCommand::TransceiverSyncCommand(TransceiverSyncCommandType cmd, bool value)
{
    this->cmdType = cmd;
    this->booleanValue = value;
}

TransceiverSyncCommand::TransceiverSyncCommand(TransceiverSyncCommandType cmd, uint8_t value)
{
    this->cmdType = cmd;
    this->uIntValue = value;
}

TransceiverSyncCommand::TransceiverSyncCommand(TransceiverSyncCommandType cmd, uint64_t value)
{
    this->cmdType = cmd;
    this->uIntValue = value;
}

TransceiverSyncCommand::~TransceiverSyncCommand()
{
}

TransceiverSyncCommandType TransceiverSyncCommand::getCommandType(void)
{
    return (this->cmdType);
}

bool TransceiverSyncCommand::getBooleanValue(void)
{
    return (this->booleanValue);
}

uint64_t TransceiverSyncCommand::getUIntValue(void)
{
    return (this->uIntValue);
}
