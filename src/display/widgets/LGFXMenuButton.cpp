#include "LGFXMenuButton.hpp"

#define BUTTON_NOT_ACTIVE_BACKGROUND_COLOR 0x0000
#define BUTTON_ACTIVE_BACKGROUND_COLOR 0xFFFF

#define BUTTON_NOT_ACTIVE_LABEL_COLOR 0xFFFF
#define BUTTON_ACTIVE_LABEL_COLOR 0x0000

LGFXMenuButton::LGFXMenuButton(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, uint8_t index, const char *topLabel, const char *bottomLabel, bool active, ButtonCallback onClick)
    : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding), index(index), active(active), onClick(onClick), changed(true)
{
    if (displayPtr != nullptr)
    {
        snprintf(this->topLabel, sizeof(this->topLabel), "%s", strlen(topLabel) == MENU_BUTTON_TOP_LABEL_LENGTH ? topLabel : "LE");
        snprintf(this->bottomLabel, sizeof(this->bottomLabel), "%s", strlen(bottomLabel) == MENU_BUTTON_BOTTOM_LABEL_LENGTH ? bottomLabel : "LEN  ERR");
        this->refresh(true);
    }
}

LGFXMenuButton::~LGFXMenuButton()
{
}

bool LGFXMenuButton::refresh(bool force)
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
        this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _MENU_BUTTON_WIDGET_TOP_LABEL_X_OFFSET, this->yOffset + this->padding + _MENU_BUTTON_WIDGET_TOP_LABEL_Y_OFFSET);
        this->parentDisplayPtr->print(this->topLabel);
        this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _MENU_BUTTON_WIDGET_MAIN_LABEL_X_OFFSET, this->yOffset + this->padding + _MENU_BUTTON_WIDGET_MAIN_LABEL_Y_OFFSET);
        this->parentDisplayPtr->print(this->bottomLabel);
        this->changed = false;
    }
    return (changed);
}

void LGFXMenuButton::onChange(void)
{
    this->changed = true;
}

bool LGFXMenuButton::isActive(void)
{
    return (this->active);
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
