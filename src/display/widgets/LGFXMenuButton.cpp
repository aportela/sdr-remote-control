#include "LGFXMenuButton.hpp"
#include "../../DisplayConfiguration.hpp"

#ifdef DISPLAY_LOVYANN_ILI9488_480x320

#include "../ILI9488/ScreenConnectedDefines.hpp"

#elif defined(DISPLAY_LOVYANN_ST7789_240x320)

#include "../ST7789/ScreenConnectedDefines.hpp"

#else

#error NO DISPLAY DEFINED

#endif // DISPLAY_LOVYANN_ILI9488_480x320

#define BUTTON_NOT_ACTIVE_BACKGROUND_COLOR 0x0000
#define BUTTON_ACTIVE_BACKGROUND_COLOR 0xFFFF

#define BUTTON_NOT_ACTIVE_LABEL_COLOR 0xFFFF
#define BUTTON_ACTIVE_LABEL_COLOR 0x0000

LGFXMenuButton::LGFXMenuButton(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, uint8_t index, const char *topLabel, const char *bottomLabel, bool active, ButtonCallback onClick)
    : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding), index(index), active(active), onClick(onClick), changed(true)
{
    snprintf(this->topLabel, sizeof(this->topLabel), "%s", strlen(topLabel) == MENU_BUTTON_TOP_LABEL_LENGTH ? topLabel : "LE");
    snprintf(this->bottomLabel, sizeof(this->bottomLabel), "%s", strlen(bottomLabel) == MENU_BUTTON_BOTTOM_LABEL_LENGTH ? bottomLabel : "LEN  ERR");
}

LGFXMenuButton::~LGFXMenuButton()
{
}

bool LGFXMenuButton::refresh(bool force, const TransceiverStatus *currentTrxStatus)
{
    bool changed = force || this->changed;
    if (changed)
    {
        if (!this->active)
        {
            this->parentDisplayPtr->fillRect(this->xOffset + this->padding, this->yOffset + this->padding, this->width, this->height, BUTTON_NOT_ACTIVE_BACKGROUND_COLOR);
            this->parentDisplayPtr->drawRect(this->xOffset + this->padding, this->yOffset + this->padding, this->width, this->height, BUTTON_ACTIVE_BACKGROUND_COLOR);
        }
        else
        {
            this->parentDisplayPtr->fillRect(this->xOffset + this->padding, this->yOffset + this->padding, this->width, this->height, BUTTON_ACTIVE_BACKGROUND_COLOR);
        }
        this->parentDisplayPtr->setTextSize(2);
        if (!active)
        {
            this->parentDisplayPtr->setTextColor(BUTTON_NOT_ACTIVE_LABEL_COLOR, BUTTON_NOT_ACTIVE_BACKGROUND_COLOR);
        }
        else
        {
            this->parentDisplayPtr->setTextColor(BUTTON_ACTIVE_LABEL_COLOR, BUTTON_ACTIVE_BACKGROUND_COLOR);
        }
        this->parentDisplayPtr->setCursor(this->xOffset + this->padding + 44, this->yOffset + this->padding + 5);
        this->parentDisplayPtr->print(this->topLabel);
        this->parentDisplayPtr->setCursor(this->xOffset + this->padding + 8, this->yOffset + this->padding + 26);
        this->parentDisplayPtr->print(this->bottomLabel);
        this->changed = false;
    }
    return (changed);
}

void LGFXMenuButton::onChange(void)
{
    this->changed = true;
}

void LGFXMenuButton::setActive(bool flag)
{
    this->active = flag;
    this->onChange();
}

void LGFXMenuButton::click(void)
{
    if (this->onClick)
    {
        this->onClick();
    }
}
