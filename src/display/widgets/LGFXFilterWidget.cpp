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

LGFXFilterWidget::LGFXFilterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding)
{
  if (displayPtr != nullptr)
  {
    this->plotSprite = new lgfx::LGFX_Sprite(displayPtr);
    this->plotSprite->setColorDepth(8);
    this->plotSprite->createSprite(180, 46);
    if (currentTransceiverStatusPtr != nullptr)
    {
      this->refresh(true, currentTransceiverStatusPtr);
    }
  }
}

LGFXFilterWidget::~LGFXFilterWidget()
{
  delete this->plotSprite;
  this->plotSprite = nullptr;
}

void LGFXFilterWidget::refreshPlot(bool force, uint64_t minLowCut, uint64_t minHighCut, uint64_t maxLowCut, uint64_t maxHighCut, uint64_t currentLowCut, uint64_t currentHighCut)
{
  this->plotSprite->fillScreen(TFT_BLACK);

  uint8_t topHLineLength = (currentHighCut + currentLowCut) * _FILTER_WIDGET_TRAPEZIUM_TOP_LINE_MAX_LENGTH / (maxLowCut + maxHighCut);
  uint8_t topHLineXOffset = (_FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_LENGTH / 2) - (topHLineLength / 2);
  uint8_t topHLineYOffset = 0; //_FILTER_WIDGET_TRAPEZIUM_TOP_LINE_Y_OFFSET;

  // top horizontal line
  this->plotSprite->drawFastHLine(topHLineXOffset, topHLineYOffset, topHLineLength, TEXT_COLOR_ACTIVE);
  // bottom horizontal line
  this->plotSprite->drawFastHLine(0, _FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_Y_OFFSET, _FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_LENGTH, TEXT_COLOR_ACTIVE);

  // left line
  this->plotSprite->drawLine(topHLineXOffset - _FILTER_WIDGET_TRAPEZIUM_LR_LINES_HORIZONTAL_OFFSET, _FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_Y_OFFSET, topHLineXOffset, topHLineYOffset, TEXT_COLOR_ACTIVE);
  // right line
  this->plotSprite->drawLine(topHLineXOffset + topHLineLength, topHLineYOffset, topHLineXOffset + topHLineLength + _FILTER_WIDGET_TRAPEZIUM_LR_LINES_HORIZONTAL_OFFSET, _FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_Y_OFFSET, TEXT_COLOR_ACTIVE);
  // vertical center line
  uint8_t centerVLineXOffset = currentLowCut == currentHighCut ? (_FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_LENGTH / 2) : (_FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_LENGTH / 2) - (topHLineLength / 2) + ((currentLowCut)*_FILTER_WIDGET_TRAPEZIUM_TOP_LINE_MAX_LENGTH / (maxLowCut + maxHighCut));
  uint8_t centerVLineYOffset = 1;
  this->plotSprite->drawFastVLine(centerVLineXOffset, centerVLineYOffset, _FILTER_WIDGET_TRAPEZIUM_CENTER_V_LINE_LENGTH, TEXT_COLOR_ACTIVE);

  this->plotSprite->pushSprite(this->xOffset + this->padding, this->yOffset + this->padding);
}

void LGFXFilterWidget::refreshLabels(bool force, uint64_t lowCut, uint64_t highCut)
{
  this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
  this->parentDisplayPtr->setTextSize(_FILTER_WIDGET_FONT_SIZE);

  if (force)
  {
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_LEFT_LABELS_X_OFFSET, this->yOffset + this->padding);
    this->parentDisplayPtr->print("FILTER");
  }
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_LEFT_LABELS_X_OFFSET, this->yOffset + this->padding + (_FILTER_WIDGET_LABELS_FONT_PIXEL_HEIGHT * 1));
  this->parentDisplayPtr->print("PRESET");
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_LEFT_LABELS_X_OFFSET, this->yOffset + this->padding + (_FILTER_WIDGET_LABELS_FONT_PIXEL_HEIGHT * 2));
  this->parentDisplayPtr->print(" 1/8");
  // TODO: human units
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_RIGHT_LABELS_X_OFFSET, this->yOffset + this->padding);
  this->parentDisplayPtr->printf("BANDWITH:%" PRIu64 "Hz   ", lowCut + highCut);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_RIGHT_LABELS_X_OFFSET, this->yOffset + this->padding + (_FILTER_WIDGET_LABELS_FONT_PIXEL_HEIGHT * 1));
  this->parentDisplayPtr->printf(" LOW CUT:%" PRIu64 "Hz   ", lowCut);
  this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _FILTER_WIDGET_RIGHT_LABELS_X_OFFSET, this->yOffset + this->padding + (_FILTER_WIDGET_LABELS_FONT_PIXEL_HEIGHT * 2));
  this->parentDisplayPtr->printf("  HI CUT:%" PRIu64 "Hz   ", highCut);
}

bool LGFXFilterWidget::refresh(bool force, const TransceiverStatus *currentTransceiverStatusPtr)
{
  bool changed = force;
  if (force || (this->oldLFValue != currentTransceiverStatusPtr->VFO[currentTransceiverStatusPtr->activeVFOIndex].LF || this->oldHFValue != currentTransceiverStatusPtr->VFO[currentTransceiverStatusPtr->activeVFOIndex].HF))
  {
    this->refreshPlot(force, 0, 0, 3800, 3800, currentTransceiverStatusPtr->VFO[currentTransceiverStatusPtr->activeVFOIndex].LF, currentTransceiverStatusPtr->VFO[currentTransceiverStatusPtr->activeVFOIndex].HF);
    this->refreshLabels(force, currentTransceiverStatusPtr->VFO[currentTransceiverStatusPtr->activeVFOIndex].LF, currentTransceiverStatusPtr->VFO[currentTransceiverStatusPtr->activeVFOIndex].HF);
    this->oldLFValue = currentTransceiverStatusPtr->VFO[currentTransceiverStatusPtr->activeVFOIndex].LF;
    this->oldHFValue = currentTransceiverStatusPtr->VFO[currentTransceiverStatusPtr->activeVFOIndex].HF;
    changed = true;
  }
  return (changed);
}