#ifndef SDR_REMOTE_CONTROL_LGFX_MENU_H
#define SDR_REMOTE_CONTROL_LGFX_MENU_H

#include <stdio.h>
#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"
#include "LGFXMenuButton.hpp"
#include "../../MenuConfiguration.hpp"
#include "../../Menu.hpp"
#include <string.h>

class LGFXMenu : public LGFXWidget
{
private:
    LGFXMenuButton *buttons[TOTAL_MENU_ITEMS];
    uint8_t currentPage = 0;
    Menu *menuPtr = nullptr;

    void initMenu(void);

public:
    LGFXMenu(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Menu *menuPtr = nullptr);
    ~LGFXMenu();
    bool refresh(bool force = false) override;
};

#endif // SDR_REMOTE_CONTROL_LGFX_MENU_H
