#include "LGFXVolumeSquelchWidget.hpp"
#include "../../DisplayConfiguration.hpp"

#ifdef DISPLAY_LOVYANN_ILI9488_480x320

#include "../ILI9488/ScreenConnectedDefines.hpp"

#elif defined(DISPLAY_LOVYANN_ST7789_240x320)

#include "../ST7789/ScreenConnectedDefines.hpp"

#else

#error NO DISPLAY DEFINED

#endif // DISPLAY_LOVYANN_ILI9488_480x320

#define TEXT_COLOR_ACTIVE TFT_WHITE
#define TEXT_COLOR_SECONDARY 0x528A
#define TEXT_COLOR_NOT_ACTIVE 0x18C3
#define TEXT_BACKGROUND_COLOR TFT_BLACK

LGFXVolumeSquelchWidget::LGFXVolumeSquelchWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Transceiver *transceiverPtr) : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding), transceiverPtr(transceiverPtr)
{
  if (displayPtr != nullptr)
  {
  }
}

LGFXVolumeSquelchWidget::~LGFXVolumeSquelchWidget()
{
}

bool LGFXVolumeSquelchWidget::refresh(bool force)
{
  bool changed = force;
  if (changed || this->transceiverPtr->changed > 0)
  {
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_AF_GAIN))
    {
      this->transceiverPtr->changed &= ~TRX_CFLAG_AF_GAIN;
      changed = true;
    }
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_SECONDARY_VFO_FILTER_HIGH))
    {
      this->transceiverPtr->changed &= ~TRX_CFLAG_AF_GAIN;
      changed = true;
    }
  }
  return (changed);
}