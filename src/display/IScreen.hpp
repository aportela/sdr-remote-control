#ifndef SDR_REMOTE_CONTROL_SCREEN_INTERFACE_H
#define SDR_REMOTE_CONTROL_SCREEN_INTERFACE_H

class IScreen
{
public:
    virtual ~IScreen() {}
    virtual bool Refresh(bool force = false) = 0;
};

#endif // SDR_REMOTE_CONTROL_SCREEN_INTERFACE_H
