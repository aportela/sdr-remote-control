#include "LGFXAnalogSMeterWidget.hpp"

#define ANALOG_SMETER_BACKGROUND_COLOR 0xF75B // 0xE6F9
#define ANALOG_SMETER_BACKGROUND_BORDER_COLOR TEXT_COLOR_SECONDARY
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
        ANALOG_SMETER_BACKGROUND_BORDER_COLOR      // color
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

    // vertical lines
    for (int i = 0; i < _ANALOG_SMETER_WIDGET_BAR_COUNT; i++)
    {
        uint16_t x = _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET + (i * (_ANALOG_SMETER_WIDGET_VLINES_WIDTH + _ANALOG_SMETER_WIDGET_VLINES_MARGIN));
        bool isCurrentLongBar = (i == 0 || i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 23 || i == 28 || i == 33 || i == 38 || i == 43);
        this->fillRect(
            x,                                                                                                                     // x
            (isCurrentLongBar ? _ANALOG_SMETER_WIDGET_LONG_VLINES_Y_OFFSET : _ANALOG_SMETER_WIDGET_SHORT_VLINES_Y_OFFSET),         // y
            _ANALOG_SMETER_WIDGET_VLINES_WIDTH,                                                                                    // width
            isCurrentLongBar ? _ANALOG_SMETER_WIDGET_CENTER_LONG_VLINES_LENGTH : _ANALOG_SMETER_WIDGET_CENTER_SHORT_VLINES_LENGTH, // height
            ANALOG_SMETER_TEXT_COLOR                                                                                               // color
        );
    }

    // horizontal line
    this->fillRect(
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET, // x
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET, // y
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH,   // width
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_HEIGHT,   // height
        ANALOG_SMETER_TEXT_COLOR                     // color
    );

    // left colored ("low values") horizontal bar
    this->fillRect(
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_LOW_VALUES_X_OFFSET, // x
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_LOW_VALUES_Y_OFFSET, // y
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_LOW_VALUES_WIDTH,    // width
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_LOW_VALUES_HEIGHT,   // height
        ANALOG_SMETER_BAR_LOW_VALUE_BAR_COLOR                   // Color
    );

    // right colored ("high values") horizontal bar
    this->fillRect(
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_HIGH_VALUES_X_OFFSET, // x
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_HIGH_VALUES_Y_OFFSET, // y
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_HIGH_VALUES_WIDTH,    // width
        _ANALOG_SMETER_WIDGET_CENTER_HLINE_HIGH_VALUES_HEIGHT,   // height
        ANALOG_SMETER_BAR_HIGH_VALUE_BAR_COLOR                   // color
    );
}

int16_t LGFXAnalogSMeterWidget::getVLineValueXOffset(int8_t dB)
{
    int16_t xOffset = 0;
    if (dB >= -54 && dB <= 60)
    {
        const int16_t baseValue = _ANALOG_SMETER_WIDGET_VLINES_WIDTH + _ANALOG_SMETER_WIDGET_VLINES_MARGIN;
        xOffset = baseValue * (dB + 54) / _ANALOG_SMETER_WIDGET_VLINES_WIDTH;
    }
    return _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET + xOffset;
}

void LGFXAnalogSMeterWidget::update(int8_t dB)
{
    // clear previous value
    this->fillRect(this->getVLineValueXOffset(this->previousDBSmoothedValue), _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_OFFSET, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_WIDTH, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_HEIGHT, ANALOG_SMETER_BACKGROUND_COLOR);
    // draw current value
    this->fillRect(this->getVLineValueXOffset(dB), _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_OFFSET, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_WIDTH, _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_HEIGHT, ANALOG_SMETER_INDICATOR_COLOR);
}