#include "LGFXScreenConnecting.hpp"
#include "../utils/FPS.hpp"
#include "../DisplayConfiguration.hpp"
#include "../CommonDefines.hpp"

#define LABEL_TITLE_TEXT "SDR-REMOTE-CONTROL"
#define LABEL_TITLE_TEXT_COLOR TFT_WHITE
#define LABEL_TITLE_TEXT_BG_COLOR TFT_BLACK
#define LABEL_TITLE_FONT &fonts::Font0
#define LABEL_TITLE_FONT_SIZE 2
#define LABEL_TITLE_X_OFFSET DISPLAY_WIDTH / 2
#define LABEL_TITLE_Y_OFFSET 16

#define LABEL_VERSION_Y_OFFSET 40

#define SSWANIMATION_WIDTH DISPLAY_WIDTH / 2
#define SSWANIMATION_X_OFFSET DISPLAY_WIDTH / 4
#define SSWANIMATION_Y_OFFSET 80

#define LABEL_FPS_X_OFFSET DISPLAY_WIDTH / 2
#define LABEL_FPS_Y_OFFSET SSWANIMATION_Y_OFFSET + SSWA_SPECTRUM_SCOPE_HEIGHT + SSWA_WATERFALL_HEIGHT + 16

using namespace aportela::microcontroller::utils;

LGFXScreenConnecting::LGFXScreenConnecting(LovyanGFX *display) : LGFXScreen(display)
{
    if (display != nullptr)
    {
        this->parentDisplay->setTextDatum(CC_DATUM);
        this->parentDisplay->setTextColor(LABEL_TITLE_TEXT_COLOR, LABEL_TITLE_TEXT_BG_COLOR);
        this->parentDisplay->setFont(LABEL_TITLE_FONT);
        this->parentDisplay->setTextSize(LABEL_TITLE_FONT_SIZE);
        this->parentDisplay->drawString(LABEL_TITLE_TEXT, LABEL_TITLE_X_OFFSET, LABEL_TITLE_Y_OFFSET);

        char str[128];

        snprintf(str, sizeof(str), "v%s (%s)", SDR_REMOTE_CONTROL_CURRENT_VERSION, SDR_REMOTE_CONTROL_CURRENT_VERSION_DATE);
        this->parentDisplay->drawString(str, DISPLAY_WIDTH / 2, LABEL_VERSION_Y_OFFSET);

        this->parentDisplay->setTextSize(1);
        snprintf(str, sizeof(str), "Searching TS-2000 CAT connection (%d baud)", DEFAULT_SERIAL_SPEED);
        this->parentDisplay->drawString(str, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - 10);
        this->animatedScreenPtr = new LGFXSSWAnimation(this->parentDisplay, SSWANIMATION_WIDTH);
    }
}

LGFXScreenConnecting::~LGFXScreenConnecting()
{
    this->parentDisplay = nullptr;
}

bool LGFXScreenConnecting::Refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        bool changed = true;
        this->animatedScreenPtr->refresh(SSWANIMATION_X_OFFSET, SSWANIMATION_Y_OFFSET);
#ifdef DEBUG_FPS
        uint16_t currentFPS = FPS::GetFPS();
        if (force || this->previousFPS != currentFPS)
        {
            char str[10];
            snprintf(str, sizeof(str), "FPS: %04u", currentFPS);
            this->parentDisplay->drawString(str, LABEL_FPS_X_OFFSET, LABEL_FPS_Y_OFFSET);
            changed = true;
        }
#endif
        return (changed);
    }
    else
    {
        return (false);
    }
}
