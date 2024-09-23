#include "LGFXMenuButton.hpp"

#define BUTTON_WIDTH 111
#define BUTTON_HEIGHT 46

#define BUTTON_NOT_ACTIVE_BACKGROUND_COLOR 0x0000
#define BUTTON_ACTIVE_BACKGROUND_COLOR 0xFFFF

#define BUTTON_NOT_ACTIVE_LABEL_COLOR 0xFFFF
#define BUTTON_ACTIVE_LABEL_COLOR 0x0000

LGFXMenuButton::LGFXMenuButton(LovyanGFX *displayPtr, uint8_t index, uint16_t xOffset, uint16_t yOffset, const char *topLabel, const char *bottomLabel, bool active, ButtonCallback onClick)
    : MenuButton(index, xOffset, yOffset, BUTTON_WIDTH, BUTTON_HEIGHT, const_cast<char *>(topLabel), const_cast<char *>(bottomLabel), active, onClick)
{
    if (displayPtr != nullptr)
    {
        this->parentDisplayPtr = displayPtr;
    }
}

LGFXMenuButton::~LGFXMenuButton()
{
    this->parentDisplayPtr = nullptr;
}

void LGFXMenuButton::draw(void)
{
    if (this->changed)
    {
        if (!this->active)
        {
            this->parentDisplayPtr->fillRect(this->xOffset, this->yOffset, this->width, this->height, BUTTON_NOT_ACTIVE_BACKGROUND_COLOR);
            this->parentDisplayPtr->drawRect(this->xOffset, this->yOffset, this->width, this->height, BUTTON_ACTIVE_BACKGROUND_COLOR);
        }
        else
        {
            this->parentDisplayPtr->fillRect(this->xOffset, this->yOffset, this->width, this->height, BUTTON_ACTIVE_BACKGROUND_COLOR);
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
        this->parentDisplayPtr->setCursor(this->xOffset + 44, this->yOffset + 5);
        this->parentDisplayPtr->print(this->topLabel);
        this->parentDisplayPtr->setCursor(this->xOffset + 8, this->yOffset + 26);
        this->parentDisplayPtr->print(this->bottomLabel);
        this->changed = false;
    }
}