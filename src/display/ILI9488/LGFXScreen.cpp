#include "LGFXScreen.hpp"
#include "../SizesAndOffsets-320x240.hpp"

// #define DEBUG_SCREEN_TITLE_BOUNDS

LGFXScreen::LGFXScreen(LovyanGFX *display) : parentDisplay(display)
{
    if (display != nullptr)
    {
        this->parentDisplay->fillScreen(TFT_BLACK);
        this->parentDisplay->setTextWrap(false);
#ifdef DEBUG_SCREEN_TITLE_BOUNDS
        this->parentDisplay->drawRect(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, TFT_WHITE);
#endif
    }
}

LGFXScreen::~LGFXScreen()
{
    this->parentDisplay = nullptr;
}
