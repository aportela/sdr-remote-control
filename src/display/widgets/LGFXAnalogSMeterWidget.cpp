#include "LGFXAnalogSMeterWidget.hpp"

#define MIN_DB -54     // S0
#define MAX_DB 60      // S9+60
#define DB_BAR_STEPS 3 // 3 db between each bar

#define BAR_DISABLED_BACKGROUND_COLOR 0x8410

LGFXAnalogSMeterWidget::LGFXAnalogSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr)
    : LGFXTransceiverStatusWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
    if (displayPtr != nullptr)
    {
        this->expSprite = new lgfx::LGFX_Sprite(displayPtr);
        this->expSprite->setColorDepth(8);
        this->expSprite->createSprite(60, 28);
        this->templateSprite = new lgfx::LGFX_Sprite(displayPtr);
        this->templateSprite->setColorDepth(8);
        this->templateSprite->createSprite(this->width, this->height);
        // this->templateSprite->fillScreen(0xFF9C);
        this->templateSprite->fillScreen(TFT_WHITE);
        this->refresh(true);
    }
}

LGFXAnalogSMeterWidget::~LGFXAnalogSMeterWidget()
{
    delete this->templateSprite;
    this->templateSprite = nullptr;
}

void LGFXAnalogSMeterWidget::createSMeter(void)
{
    this->parentDisplayPtr->fillRoundRect(this->xOffset + this->padding, this->yOffset + this->padding, ANALOG_SMETER_WIDGET_BACKGROUND_WIDTH, ANALOG_SMETER_WIDGET_BACKGROUND_HEIGHT, 4, 0xF75B); // 0xE6F9
    this->parentDisplayPtr->drawFastHLine(this->xOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, this->yOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET, ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH, TFT_BLACK);
    this->parentDisplayPtr->drawFastHLine(this->xOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, this->yOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET + 1, ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH, TFT_BLACK);

    for (int i = 0; i < _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
    {
#define B_WIDTH 3
#define B_MARGIN 6
        uint16_t x = this->xOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET + (i * (B_WIDTH + B_MARGIN));
        bool isCurrentLongBar = (i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 17 || i == 22 || i == 27 || i == 37);
        this->parentDisplayPtr->fillRect(x, this->yOffset + this->padding + (isCurrentLongBar ? ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 16 : ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 8), B_WIDTH, isCurrentLongBar ? 16 : 8, TFT_BLACK);
    }
}

void LGFXAnalogSMeterWidget::refreshSMeterDBLabel(bool force, int8_t dB)
{
    this->parentDisplayPtr->setTextSize(_DIGITAL_SMETER_WIDGET_S_LABEL_FONT_SIZE);
    this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
    if (force)
    {
        this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_Y_OFFSET);
        this->parentDisplayPtr->print("S");
    }
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_BASE_NUMBER_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_Y_OFFSET);
    bool showDBExp = dB > 0;
    if (dB < -48)
    {
        this->parentDisplayPtr->print("0");
    }
    else if (dB < -42)
    {
        this->parentDisplayPtr->print("1");
    }
    else if (dB < -36)
    {
        this->parentDisplayPtr->print("2");
    }
    else if (dB < -30)
    {
        this->parentDisplayPtr->print("3");
    }
    else if (dB < -24)
    {
        this->parentDisplayPtr->print("4");
    }
    else if (dB < -18)
    {
        this->parentDisplayPtr->print("5");
    }
    else if (dB < -12)
    {
        this->parentDisplayPtr->print("6");
    }
    else if (dB < -6)
    {
        this->parentDisplayPtr->print("7");
    }
    else if (dB < 0)
    {
        this->parentDisplayPtr->print("8");
    }
    else
    {
        this->parentDisplayPtr->print("9");
    }
    if (!showDBExp)
    {
        // only refresh base dB label if previous value has exp label
        if (force || this->previousDBValue >= 0)
        {
            this->parentDisplayPtr->print("dB ");
        }
    }
    else
    {
        this->expSprite->fillScreen(TFT_BLACK);
        this->expSprite->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);
        this->expSprite->setTextSize(_DIGITAL_SMETER_WIDGET_S_SUB_LABEL_FONT_SIZE);
        this->expSprite->setCursor(0, 0);
        this->expSprite->printf("+%ddB", dB);
        this->expSprite->pushSprite(this->xOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_EXP_X_OFFSET, this->yOffset + this->padding + _DIGITAL_SMETER_WIDGET_S_LABEL_Y_OFFSET);
    }
}

void LGFXAnalogSMeterWidget::refreshSMeter(int8_t dB)
{
    uint16_t xof = map(this->previousDBValue, MIN_DB, MAX_DB, this->xOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH);
    // clear previous value
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + xof, this->yOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_OFFSET, ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_WIDTH, ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_HEIGHT, 0xF75B);
    // draw current value
    xof = map(dB, MIN_DB, MAX_DB, this->xOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH);
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + xof, this->yOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_OFFSET, ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_WIDTH, ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_HEIGHT, TFT_BLUE);
}

bool LGFXAnalogSMeterWidget::refresh(bool force)
{
    bool changed = force;
    if (force)
    {
        this->createSMeter();
    }
    if (force || this->previousDBValue != this->currentTransceiverStatusPtr->signalMeterdBLevel)
    {
        this->refreshSMeter(this->currentTransceiverStatusPtr->signalMeterdBLevel);
        this->refreshSMeterDBLabel(force, this->currentTransceiverStatusPtr->signalMeterdBLevel);
        this->previousDBValue = this->currentTransceiverStatusPtr->signalMeterdBLevel;
        changed = true;
    }
    return (changed);
}
