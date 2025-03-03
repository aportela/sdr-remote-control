#include "LGFXScreen.hpp"
#include "../DisplayConfiguration.hpp"

#define DEBUG_SCREEN_TITLE_BOUNDS

LGFXScreen::LGFXScreen(LovyanGFX *display) : parentDisplay(display)
{
    if (display != nullptr)
    {
        this->parentDisplay->fillScreen(TFT_BLACK);
        this->parentDisplay->setTextWrap(false);
#ifdef DEBUG_SCREEN_BOUNDS

        this->parentDisplay->drawRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, DEBUG_SCREEN_WIDGETS_BOUNDS_BORDER_COLOR);

#endif // DEBUG_SCREEN_BOUNDS
    }
}

LGFXScreen::~LGFXScreen()
{
    this->parentDisplay = nullptr;
}
