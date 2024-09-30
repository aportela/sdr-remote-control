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
  uint8_t topHLineLength = (currentHighCut + currentLowCut) * _FILTER_WIDGET_TRAPEZIUM_TOP_LINE_MAX_LENGTH / (maxLowCut + maxHighCut);
  uint8_t topHLineXOffset = this->xOffset + this->padding + (_FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_LENGTH / 2) - (topHLineLength / 2);
  uint8_t topHLineYOffset = this->yOffset + this->padding + _FILTER_WIDGET_TRAPEZIUM_TOP_LINE_Y_OFFSET;

  // top horizontal line
  this->parentDisplayPtr->drawFastHLine(topHLineXOffset, topHLineYOffset, topHLineLength, TEXT_COLOR_ACTIVE);
  // bottom horizontal line
  this->parentDisplayPtr->drawFastHLine(this->xOffset + this->padding, this->yOffset + this->padding + _FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_Y_OFFSET, _FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_LENGTH, TEXT_COLOR_ACTIVE);

  // left line
  this->parentDisplayPtr->drawLine(topHLineXOffset - _FILTER_WIDGET_TRAPEZIUM_LR_LINES_HORIZONTAL_OFFSET, this->yOffset + this->padding + _FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_Y_OFFSET, topHLineXOffset, topHLineYOffset, TEXT_COLOR_ACTIVE);
  // right line
  this->parentDisplayPtr->drawLine(topHLineXOffset + topHLineLength, topHLineYOffset, topHLineXOffset + topHLineLength + _FILTER_WIDGET_TRAPEZIUM_LR_LINES_HORIZONTAL_OFFSET, this->yOffset + this->padding + _FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_Y_OFFSET, TEXT_COLOR_ACTIVE);
  // vertical center line
  uint8_t centerVLineXOffset = currentLowCut == currentHighCut ? this->xOffset + this->padding + (_FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_LENGTH / 2) : this->xOffset + this->padding + (_FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_LENGTH / 2) - (topHLineLength / 2) + ((currentLowCut)*_FILTER_WIDGET_TRAPEZIUM_TOP_LINE_MAX_LENGTH / (maxLowCut + maxHighCut));
  uint8_t centerVLineYOffset = this->yOffset + this->padding + _FILTER_WIDGET_TRAPEZIUM_TOP_LINE_Y_OFFSET;
  this->parentDisplayPtr->drawFastVLine(centerVLineXOffset, centerVLineYOffset, _FILTER_WIDGET_TRAPEZIUM_CENTER_V_LINE_LENGTH, TEXT_COLOR_ACTIVE);
}

void LGFXFilterWidget::refreshLabels(uint64_t lowCut, uint64_t highCut)
{
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
  this->parentDisplayPtr->setTextSize(_FILTER_WIDGET_FONT_SIZE);

  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_LEFT_LABELS_X_OFFSET, this->yOffset + this->padding + _FILTER_WIDGET_LABELS_Y_OFFSET);
  this->parentDisplayPtr->print("FILTER");
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_LEFT_LABELS_X_OFFSET, this->yOffset + this->padding + _FILTER_WIDGET_LABELS_Y_OFFSET + (_FILTER_WIDGET_LABELS_FONT_PIXEL_HEIGHT * 1));
  this->parentDisplayPtr->print("PRESET");
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_LEFT_LABELS_X_OFFSET, this->yOffset + this->padding + _FILTER_WIDGET_LABELS_Y_OFFSET + (_FILTER_WIDGET_LABELS_FONT_PIXEL_HEIGHT * 2));
  this->parentDisplayPtr->print(" 1/8");
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_RIGHT_LABELS_X_OFFSET, this->yOffset + this->padding + _FILTER_WIDGET_LABELS_Y_OFFSET);
  this->parentDisplayPtr->printf("BANDWITH:%" PRIu64 "Hz", lowCut + highCut);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_RIGHT_LABELS_X_OFFSET, this->yOffset + this->padding + _FILTER_WIDGET_LABELS_Y_OFFSET + (_FILTER_WIDGET_LABELS_FONT_PIXEL_HEIGHT * 1));
  this->parentDisplayPtr->printf(" LOW CUT:%" PRIu64 "Hz", lowCut);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_RIGHT_LABELS_X_OFFSET, this->yOffset + this->padding + _FILTER_WIDGET_LABELS_Y_OFFSET + (_FILTER_WIDGET_LABELS_FONT_PIXEL_HEIGHT * 2));
  this->parentDisplayPtr->printf("  HI CUT:%" PRIu64 "Hz", highCut);
}

bool LGFXFilterWidget::refresh(bool force, const TransceiverStatus *currentTrxStatus)
{
  bool changed = force;
  if (changed || currentTrxStatus->changed > 0)
  {
    if (force || (currentTrxStatus->changed & TRX_CFLAG_ACTIVE_VFO_FILTER_LOW) || (currentTrxStatus->changed & TRX_CFLAG_ACTIVE_VFO_FILTER_HIGH))
    {
      this->refreshPlot(0, 0, 3800, 3800, 100, 2900);
      this->refreshLabels(100, 2900);
      // TODO
      // this->transceiverPtr->changed &= ~TRX_CFLAG_ACTIVE_VFO_FILTER_LOW;
      // this->transceiverPtr->changed &= ~TRX_CFLAG_ACTIVE_VFO_FILTER_HIGH;
      changed = true;
    }
  }
  return (changed);
}