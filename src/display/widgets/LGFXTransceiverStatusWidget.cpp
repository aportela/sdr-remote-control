#include "LGFXTransceiverStatusWidget.hpp"
#include "../../DisplayConfiguration.hpp"

#ifdef DISPLAY_LOVYANN_ILI9488_480x320

#include "../ILI9488/ScreenConnectedDefines.hpp"

#elif defined(DISPLAY_LOVYANN_ST7789_240x320)

#include "../ST7789/ScreenConnectedDefines.hpp"

#else

#error NO DISPLAY DEFINED

#endif // DISPLAY_LOVYANN_ILI9488_480x320

LGFXTransceiverStatusWidget::LGFXTransceiverStatusWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding), currentTransceiverStatusPtr(currentTransceiverStatusPtr)
{
}

LGFXTransceiverStatusWidget::~LGFXTransceiverStatusWidget()
{
    this->currentTransceiverStatusPtr = nullptr;
}
