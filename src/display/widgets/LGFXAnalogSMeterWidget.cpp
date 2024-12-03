#include "LGFXAnalogSMeterWidget.hpp"

#define MIN_DB -54     // S0
#define MAX_DB 60      // S9+60
#define DB_BAR_STEPS 3 // 3 db between each bar

#define ANALOG_SMETER_BACKGROUND_COLOR 0xF75B // // 0xE6F9
#define ANALOG_SMETER_TEXT_COLOR TEXT_COLOR_NOT_ACTIVE
#define ANALOG_SMETER_INDICATOR_COLOR 0xF388

LGFXAnalogSMeterWidget::LGFXAnalogSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr)
    : LGFXTransceiverStatusWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
    this->smeter = new SMeter(this->currentTransceiverStatusPtr->signalMeterdBLevel);
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
    delete this->smeter;
    this->smeter = nullptr;
    delete this->templateSprite;
    this->templateSprite = nullptr;
}

#ifdef USE_HORIZONTAL_GRADIENT_ON_ANALOG_SMETER_WIDGET

uint32_t LGFXAnalogSMeterWidget::getGradientColor(int index)
{
    uint32_t TFT_GREEN = 0x00FF00; // RGB: (0, 255, 0)
    uint32_t ORANGE = 0xFFA500;    // RGB: (255, 165, 0)
    uint32_t TFT_RED = 0xFF0000;   // RGB: (255, 0, 0)

    int greenR = (TFT_GREEN >> 16) & 0xFF;
    int greenG = (TFT_GREEN >> 8) & 0xFF;
    int greenB = TFT_GREEN & 0xFF;

    int orange_R = (ORANGE >> 16) & 0xFF;
    int orange_G = (ORANGE >> 8) & 0xFF;
    int orange_B = ORANGE & 0xFF;

    int redR = (TFT_RED >> 16) & 0xFF;
    int redG = (TFT_RED >> 8) & 0xFF;
    int redB = TFT_RED & 0xFF;

    const int num_steps = 342;

    if (index < 0 || index >= num_steps)
    {
        return 0;
    }

    uint32_t color;

    if (index <= 120)
    {
        int R = greenR + ((orange_R - greenR) * index) / 120;
        int G = greenG + ((orange_G - greenG) * index) / 120;
        int B = greenB + ((orange_B - greenB) * index) / 120;

        color = (R << 16) | (G << 8) | B;
    }
    else
    {
        int i = index - 120;

        int R = orange_R + ((redR - orange_R) * i) / 222;
        int G = orange_G + ((redG - orange_G) * i) / 222;
        int B = orange_B + ((redB - orange_B) * i) / 222;

        color = (R << 16) | (G << 8) | B;
    }

    return (color);
}

#endif

void LGFXAnalogSMeterWidget::createSMeter(void)
{
    this->parentDisplayPtr->fillRoundRect(this->xOffset + this->padding, this->yOffset + this->padding, ANALOG_SMETER_WIDGET_BACKGROUND_WIDTH, ANALOG_SMETER_WIDGET_BACKGROUND_HEIGHT, 4, ANALOG_SMETER_BACKGROUND_COLOR);

    this->parentDisplayPtr->setTextSize(1);
    this->parentDisplayPtr->setTextColor(ANALOG_SMETER_TEXT_COLOR);
    this->parentDisplayPtr->setCursor(this->xOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_NUMBERS_X_OFFSET, this->yOffset + this->padding + 2);
    this->parentDisplayPtr->print("1     3     5     7     9     +15     +30            +60");

#ifdef USE_HORIZONTAL_GRADIENT_ON_ANALOG_SMETER_WIDGET
    for (uint16_t i = 0; i < ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH; i++)
    {
        this->parentDisplayPtr->fillRect(this->xOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET + i, this->yOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 5, 1, 5, getGradientColor(i));
    }
#else
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, this->yOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 5, 154, 5, TFT_GREEN);
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + 159, this->yOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 5, ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH - 155, 5, TFT_RED);
#endif
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, this->yOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET, ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH, 2, ANALOG_SMETER_TEXT_COLOR);

    for (int i = 1; i < _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
    {
#define B_WIDTH 3
#define B_MARGIN 6
        uint16_t x = this->xOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET + (i * (B_WIDTH + B_MARGIN));
        bool isCurrentLongBar = (i == 1 || i == 3 || i == 5 || i == 7 || i == 9 || i == 11 || i == 13 || i == 15 || i == 17 || i == 22 || i == 27 || i == 37);
        this->parentDisplayPtr->fillRect(x, this->yOffset + this->padding + (isCurrentLongBar ? ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 16 : ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 8), B_WIDTH, isCurrentLongBar ? 16 : 8, ANALOG_SMETER_TEXT_COLOR);
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
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + xof, this->yOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_OFFSET, ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_WIDTH, ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_HEIGHT, ANALOG_SMETER_BACKGROUND_COLOR);
    // draw current value
    xof = map(dB, MIN_DB, MAX_DB, this->xOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH);
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + xof, this->yOffset + this->padding + ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_OFFSET, ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_WIDTH, ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_HEIGHT, ANALOG_SMETER_INDICATOR_COLOR);
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
