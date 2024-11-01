#include "LGFXAnalogSMeterWidget.hpp"

#define SHORT_BAR_LENGTH 8
#define LONG_BAR_LENGTH 16
#define TOTAL_DEGREES 140.0
#define BAR_INCREMENT_DEGREES (TOTAL_DEGREES / (float)_DIGITAL_SMETER_WIDGET_BAR_COUNT)
#define RADIUS 100
#define BAR_START_DEGREE 160.0
#define ARC_ANGLE_OFFSET 20
#define ARC_START_ANGLE 180 + ARC_ANGLE_OFFSET
#define ARC_END_ANGLE -ARC_ANGLE_OFFSET

LGFXAnalogSMeterWidget::LGFXAnalogSMeterWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr)
    : LGFXTransceiverStatusWidget(displayPtr, width, height, xOffset, yOffset, padding, currentTransceiverStatusPtr)
{
    if (displayPtr != nullptr)
    {
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

void LGFXAnalogSMeterWidget::refreshAnalogBar(int8_t value)
{
    float angle = (BAR_START_DEGREE - (5 * BAR_INCREMENT_DEGREES));

    float radian = radians(angle);

    int x = this->width / 2;
    int y = this->height + 24; // 16 px added because we use a 140 degrees (not 180 degrees)
    int startX = x + (50 * cos(radian));
    int startY = y - (50 * sin(radian));

    int longitud_barra = 100;

    int endX = x + ((50 + longitud_barra) * cos(radian));
    int endY = y - ((50 + longitud_barra) * sin(radian));

    this->parentDisplayPtr->drawLine(this->xOffset + startX, this->yOffset + startY, this->xOffset + endX, this->yOffset + endY, TFT_RED);
}

bool LGFXAnalogSMeterWidget::refresh(bool force)
{
    // -54 -> 60
    bool changed = force;
    if (force)
    {
        int x = this->width / 2;
        int y = this->height + 24; // 16 px added because we use a 140 degrees (not 180 degrees)

        this->templateSprite->drawArc(x, y, RADIUS, RADIUS, ARC_START_ANGLE, ARC_END_ANGLE, TFT_BLACK);
        this->templateSprite->drawArc(x, y, RADIUS - 1, RADIUS - 1, ARC_START_ANGLE, ARC_END_ANGLE, TFT_BLACK);
        this->templateSprite->drawArc(x, y, RADIUS - 2, RADIUS - 1, ARC_START_ANGLE, ARC_END_ANGLE, TFT_BLACK);
        this->templateSprite->setTextSize(1);
        for (int i = 0; i <= _DIGITAL_SMETER_WIDGET_BAR_COUNT; ++i)
        {
            float angle = (BAR_START_DEGREE - (i * BAR_INCREMENT_DEGREES));

            float radian = radians(angle);

            int startX = x + (RADIUS * cos(radian));
            int startY = y - (RADIUS * sin(radian));

            int longitud_barra = (i % 2 == 0) ? LONG_BAR_LENGTH : SHORT_BAR_LENGTH;

            int endX = x + ((RADIUS + longitud_barra) * cos(radian));
            int endY = y - ((RADIUS + longitud_barra) * sin(radian));

            this->templateSprite->drawLine(startX, startY, endX, endY, TFT_BLACK);

            bool showLabel = (i == 0 || i == 2 || i == 6 || i == 10 || i == 14 || i == 18 || i == 22 || i == 26 || i == 30 || i == _DIGITAL_SMETER_WIDGET_BAR_COUNT);
            if (showLabel)
            {
                int labelX = endX + (20 * cos(radian)) - 5; // TODO: adjust centering
                int labelY = endY - (20 * sin(radian));
                this->templateSprite->setTextColor(TFT_BLACK);
                this->templateSprite->setTextSize(1);
                this->templateSprite->setCursor(labelX, labelY);
                char label[8] = {'\0'};
                switch (i)
                {
                case 0:
                    snprintf(label, sizeof(label), "S");
                    break;
                case 2:
                    snprintf(label, sizeof(label), "1");
                    break;
                case 6:
                    snprintf(label, sizeof(label), "3");
                    break;
                case 10:
                    snprintf(label, sizeof(label), "5");
                    break;

                case 14:
                    snprintf(label, sizeof(label), "7");
                    break;
                case 18:
                    snprintf(label, sizeof(label), "9");
                    break;
                case 22:
                    snprintf(label, sizeof(label), "+15dB");
                    break;
                case 26:
                    snprintf(label, sizeof(label), "+30dB");
                    break;
                case 30:
                    snprintf(label, sizeof(label), "+45dB");
                    break;
                case _DIGITAL_SMETER_WIDGET_BAR_COUNT:
                    snprintf(label, sizeof(label), "+60dB");
                    break;
                }
                this->templateSprite->printf("%s", label);
            }
        }
    }
    if (force || this->currentTransceiverStatusPtr->signalMeterdBLevel != this->previousDBValue)
    {
        this->templateSprite->pushSprite(this->xOffset, this->yOffset);
        this->refreshAnalogBar(0);
        this->previousDBValue = this->currentTransceiverStatusPtr->signalMeterdBLevel;
        changed = true;
    }
    return (changed);
}
