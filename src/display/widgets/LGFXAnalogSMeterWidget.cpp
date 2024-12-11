#include "LGFXAnalogSMeterWidget.hpp"

#define ANALOG_SMETER_BACKGROUND_COLOR 0xF75B // 0xE6F9
#define ANALOG_SMETER_TEXT_COLOR TEXT_COLOR_NOT_ACTIVE
#define ANALOG_SMETER_INDICATOR_COLOR 0xF388
#define ANALOG_SMETER_BAR_LOW_VALUE_BAR_COLOR COLOR_GREEN
#define ANALOG_SMETER_BAR_HIGH_VALUE_BAR_COLOR COLOR_RED

LGFXAnalogSMeterWidget::LGFXAnalogSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXSMeterWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
    if (displayPtr != nullptr && currentTransceiverStatusPtr != nullptr)
    {
        this->refresh(true);
    }
}

LGFXAnalogSMeterWidget::~LGFXAnalogSMeterWidget()
{
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

void LGFXAnalogSMeterWidget::init(void)
{
    // background
    this->fillRoundRect(
        _ANALOG_SMETER_WIDGET_BACKGROUND_X_OFFSET, // x
        _ANALOG_SMETER_WIDGET_BACKGROUND_Y_OFFSET, // y
        _ANALOG_SMETER_WIDGET_BACKGROUND_WIDTH,    // width
        _ANALOG_SMETER_WIDGET_BACKGROUND_HEIGHT,   // height
        4,                                         // radius
        ANALOG_SMETER_BACKGROUND_COLOR             // color
    );

    // "background border"
    this->drawRoundRect(
        _ANALOG_SMETER_WIDGET_BACKGROUND_X_OFFSET, // x
        _ANALOG_SMETER_WIDGET_BACKGROUND_Y_OFFSET, // y
        _ANALOG_SMETER_WIDGET_BACKGROUND_WIDTH,    // width
        _ANALOG_SMETER_WIDGET_BACKGROUND_HEIGHT,   // height
        4,                                         // radius
        TEXT_COLOR_SECONDARY                       // color
    );

    // "S 1 3 4 7 9  +15  +30    +60" container labels
    this->parentDisplayPtr->setFont(_ANALOG_SMETER_WIDGET_SUNITS_RANGE_FONT_FAMILY);
    this->parentDisplayPtr->setTextSize(_ANALOG_SMETER_WIDGET_SUNITS_RANGE_FONT_SIZE);
    // WARNING: if text background color is set (ANALOG_SMETER_BACKGROUND_COLOR) on second parameter, this custom font overflows adding extra white space filled with bg color
    this->parentDisplayPtr->setTextColor(ANALOG_SMETER_TEXT_COLOR);
    this->printAtCursor(_ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_S_X_OFFSET, _ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "S");
    this->printAtCursor(_ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_S1_X_OFFSET, _ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "1");
    this->printAtCursor(_ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_S3_X_OFFSET, _ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "3");
    this->printAtCursor(_ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_S5_X_OFFSET, _ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "5");
    this->printAtCursor(_ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_S7_X_OFFSET, _ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "7");
    this->printAtCursor(_ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_S9_X_OFFSET, _ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "9");
    this->printAtCursor(_ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS15_X_OFFSET, _ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "+15");
    this->printAtCursor(_ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS30_X_OFFSET, _ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "+30");
    this->printAtCursor(_ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS60_X_OFFSET, _ANALOG_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET, "+60dB");
    this->restorePreviousFont();

#ifdef USE_HORIZONTAL_GRADIENT_ON_ANALOG_SMETER_WIDGET
    for (uint16_t i = 0; i < _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH; i++)
    {
        this->fillRect(
            _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET + i, // x
            _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 5, // y
            1,                                               // width
            5,                                               // height
            getGradientColor(i)                              // color
        );
    }
#else
/*
    this->fillRect(
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET,         // x
        7 + _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 5, // y
        155,                                                 // width
        5,                                                   // height
        ANALOG_SMETER_BAR_LOW_VALUE_BAR_COLOR                // Color
    );
    this->fillRect(
        159,                                                 // x
        7 + _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET - 5, // y
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH - 155,     // width
        5,                                                   // height
        ANALOG_SMETER_BAR_HIGH_VALUE_BAR_COLOR               // color
    );
    */
#endif
    // horizontal line
    this->fillRect(
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, // x
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET, // y
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH,   // width
        2,                                           // height
        ANALOG_SMETER_TEXT_COLOR                     // color
    );
    // vertical lines
    for (int i = 0; i < _ANALOG_SMETER_WIDGET_BAR_COUNT; i++)
    {
        uint16_t x = _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET + (i * (_ANALOG_SMETER_WIDGET_VLINES_WIDTH + _ANALOG_SMETER_WIDGET_VLINES_MARGIN));
        bool isCurrentLongBar = (i == 0 || i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 31 || i == 36);
        this->fillRect(
            x,                                                                                                                     // x
            (isCurrentLongBar ? _ANALOG_SMETER_WIDGET_LONG_VLINES_Y_OFFSET : _ANALOG_SMETER_WIDGET_SHORT_VLINES_Y_OFFSET),         // y
            _ANALOG_SMETER_WIDGET_VLINES_WIDTH,                                                                                    // width
            isCurrentLongBar ? _ANALOG_SMETER_WIDGET_CENTER_LONG_VLINES_LENGTH : _ANALOG_SMETER_WIDGET_CENTER_SHORT_VLINES_LENGTH, // height
            ANALOG_SMETER_TEXT_COLOR                                                                                               // color
        );
    }
}

void LGFXAnalogSMeterWidget::update(int8_t dB)
{
    uint16_t xof = map(this->previousDBSmoothedValue, SMETER_MIN_DB, SMETER_MAX_DB, this->xOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH);
    // clear previous value
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + xof, this->yOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_OFFSET, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_WIDTH, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_HEIGHT, ANALOG_SMETER_BACKGROUND_COLOR);

    // draw current value
    xof = map(dB, SMETER_MIN_DB, SMETER_MAX_DB, this->xOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH);
    this->parentDisplayPtr->fillRect(this->xOffset + this->padding + xof, this->yOffset + this->padding + _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_OFFSET, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_WIDTH, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_HEIGHT, ANALOG_SMETER_INDICATOR_COLOR);
}