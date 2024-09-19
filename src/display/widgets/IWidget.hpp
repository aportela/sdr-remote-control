#ifndef SDR_REMOTE_CONTROL_WIDGET_INTERFACE_H
#define SDR_REMOTE_CONTROL_WIDGET_INTERFACE_H

class IWidget
{
public:
    virtual ~IWidget() {}
    virtual void refresh(void) = 0;
};

#endif // SDR_REMOTE_CONTROL_WIDGET_INTERFACE_H
