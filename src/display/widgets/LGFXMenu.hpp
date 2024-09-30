#ifndef SDR_REMOTE_CONTROL_LGFX_MENU_H
#define SDR_REMOTE_CONTROL_LGFX_MENU_H

#include <stdio.h>
#include <LovyanGFX.hpp>
#include "LGFXWidget.hpp"
#include "LGFXMenuButton.hpp"
#include "../../MenuConfiguration.hpp"
#include <string.h>

class LGFXMenu : public LGFXWidget
{
private:
    LGFXMenuButton *buttons[TOTAL_MENU_BUTTONS];
    uint8_t currentPage = 0;
    uint8_t totalPages = 0;

    void initMenu(void);

public:
    LGFXMenu(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding);
    ~LGFXMenu();
    bool refresh(bool force, const TransceiverStatus *currentTrxStatus = nullptr) override;
    void setButtonEnabled(uint8_t btnIndex);
    void setButtonDisabled(uint8_t btnIndex);
    void previousPage(void);
    void nextPage(void);
};

#endif // SDR_REMOTE_CONTROL_LGFX_MENU_H
