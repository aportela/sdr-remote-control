#include <Arduino.h>
#include <cinttypes>
#include "LGFXFilterWidget.hpp"
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

LGFXFilterWidget::LGFXFilterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, Transceiver *transceiverPtr) : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding), transceiverPtr(transceiverPtr)
{
  if (displayPtr != nullptr)
  {
    this->refresh(true);
  }
}

LGFXFilterWidget::~LGFXFilterWidget()
{
}

void LGFXFilterWidget::refreshPlot(uint64_t minLowCut, uint64_t minHighCut, uint64_t maxLowCut, uint64_t maxHighCut, uint64_t currentLowCut, uint64_t currentHighCut)
{
#define TOP_LINE_MAX_LENGTH 120
#define BOTTOM_LINE_LENGTH 180
#define TOP_LINE_X_OFFSET (BOTTOM_LINE_LENGTH - TOP_LINE_MAX_LENGTH) / 2
#define TOP_LINE_Y_OFFSET 8
#define BOTTOM_LINE_Y_OFFSET 50
#define LINE_OFFSET TOP_LINE_X_OFFSET / 2

  uint8_t topHLineLength = (currentHighCut + currentLowCut) * TOP_LINE_MAX_LENGTH / (maxLowCut + maxHighCut);
  // top horizontal line
  this->parentDisplayPtr->drawFastHLine(this->xOffset + this->padding + TOP_LINE_X_OFFSET, this->yOffset + this->padding + TOP_LINE_Y_OFFSET, topHLineLength, TEXT_COLOR_ACTIVE);
  // bottom horizontal line
  this->parentDisplayPtr->drawFastHLine(this->xOffset + this->padding, this->yOffset + this->padding + BOTTOM_LINE_Y_OFFSET, BOTTOM_LINE_LENGTH, TEXT_COLOR_ACTIVE);
  // left line
  this->parentDisplayPtr->drawLine(this->xOffset + this->padding + LINE_OFFSET, this->yOffset + this->padding + BOTTOM_LINE_Y_OFFSET, this->xOffset + this->padding + TOP_LINE_X_OFFSET, this->yOffset + this->padding + TOP_LINE_Y_OFFSET, TEXT_COLOR_ACTIVE);
  // right line
  this->parentDisplayPtr->drawLine(this->xOffset + this->padding + TOP_LINE_X_OFFSET + topHLineLength, this->yOffset + this->padding + TOP_LINE_Y_OFFSET, this->xOffset + this->padding + TOP_LINE_X_OFFSET + topHLineLength + LINE_OFFSET, this->yOffset + this->padding + BOTTOM_LINE_Y_OFFSET, TEXT_COLOR_ACTIVE);
  // vertical center line
  if (currentLowCut == currentHighCut)
  {
    this->parentDisplayPtr->drawFastVLine(this->xOffset + this->padding + (BOTTOM_LINE_LENGTH / 2), this->yOffset + this->padding + TOP_LINE_Y_OFFSET, 42, TEXT_COLOR_ACTIVE);
  }
  else
  {
    this->parentDisplayPtr->drawFastVLine(this->xOffset + this->padding + (BOTTOM_LINE_LENGTH / 2), this->yOffset + this->padding + TOP_LINE_Y_OFFSET, 42, TEXT_COLOR_ACTIVE);
  }
}

void LGFXFilterWidget::refreshLabels(uint64_t lowCut, uint64_t highCut)
{
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
  this->parentDisplayPtr->setTextSize(_FILTER_WIDGET_FONT_SIZE);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_LABELS_X_OFFSET, this->yOffset + this->padding + _FILTER_WIDGET_LABELS_Y_OFFSET);
  this->parentDisplayPtr->printf("BANDWITH:%" PRIu64 "Hz", lowCut + highCut);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_LABELS_X_OFFSET, this->yOffset + this->padding + _FILTER_WIDGET_LABELS_Y_OFFSET + (_FILTER_WIDGET_LABELS_FONT_PIXEL_HEIGHT * 1));
  this->parentDisplayPtr->printf(" LOW CUT:%" PRIu64 "Hz", lowCut);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_LABELS_X_OFFSET, this->yOffset + this->padding + _FILTER_WIDGET_LABELS_Y_OFFSET + (_FILTER_WIDGET_LABELS_FONT_PIXEL_HEIGHT * 2));
  this->parentDisplayPtr->printf("  HI CUT:%" PRIu64 "Hz", highCut);
}

bool LGFXFilterWidget::refresh(bool force)
{
  bool changed = force;
  if (changed || this->transceiverPtr->changed > 0)
  {
    if (force || (this->transceiverPtr->changed & TRX_CFLAG_ACTIVE_VFO_FILTER_LOW) || (this->transceiverPtr->changed & TRX_CFLAG_ACTIVE_VFO_FILTER_HIGH))
    {
      // this->refreshPlot(100, 2400, 200, 3700, 200, 2400);
      // this->refreshPlot(0, 0, 3000, 3000, 1200, 1200);
      this->refreshPlot(0, 0, 3000, 3000, 3000, 3000);
      // this->refreshLabels(200, 2400);
      this->refreshLabels(1200, 1200);
      this->transceiverPtr->changed &= ~TRX_CFLAG_ACTIVE_VFO_FILTER_LOW;
      this->transceiverPtr->changed &= ~TRX_CFLAG_ACTIVE_VFO_FILTER_HIGH;
      changed = true;
    }
  }
  return (changed);
}