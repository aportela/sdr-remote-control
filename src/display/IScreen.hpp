#ifndef SDR_REMOTE_CONTROL_SCREEN_INTERFACE_H
#define SDR_REMOTE_CONTROL_SCREEN_INTERFACE_H

#include "../Transceiver.hpp"

class IScreen
{
public:
    virtual ~IScreen() {}
    virtual bool Refresh(bool force) = 0;
};

#endif // SDR_REMOTE_CONTROL_SCREEN_INTERFACE_H
