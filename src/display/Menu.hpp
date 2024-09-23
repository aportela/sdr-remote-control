#ifndef SDR_REMOTE_CONTROL_MENU_H
#define SDR_REMOTE_CONTROL_MENU_H

#include "IMenu.hpp"

class Menu : public IMenu
{
protected:
    uint16_t width;
    uint16_t height;
    uint16_t xOffset;
    uint16_t yOffset;
    uint8_t currentPage;
    uint8_t totalPages;

public:
    Menu(void);
    virtual void refresh(bool forceAll) = 0;
    virtual void previousPage(void) = 0;
    virtual void nextPage(void) = 0;
};
#endif
