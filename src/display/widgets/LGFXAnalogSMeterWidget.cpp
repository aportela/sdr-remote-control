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
        this->refresh(true);
    }
}

LGFXAnalogSMeterWidget::~LGFXAnalogSMeterWidget()
{
}

bool LGFXAnalogSMeterWidget::refresh(bool force)
{
    bool changed = force;
    if (changed)
    {
        int x = DISPLAY_WIDTH / 2;
        int y = 300;

        this->parentDisplayPtr->drawArc(x, y, RADIUS, RADIUS, ARC_START_ANGLE, ARC_END_ANGLE, TEXT_COLOR_ACTIVE);
        this->parentDisplayPtr->drawArc(x, y, RADIUS - 1, RADIUS - 1, ARC_START_ANGLE, ARC_END_ANGLE, TEXT_COLOR_ACTIVE);
        this->parentDisplayPtr->drawArc(x, y, RADIUS - 2, RADIUS - 1, ARC_START_ANGLE, ARC_END_ANGLE, TEXT_COLOR_ACTIVE);
        this->parentDisplayPtr->setTextSize(1);
        for (int i = 0; i <= _DIGITAL_SMETER_WIDGET_BAR_COUNT; ++i)
        {
            float angle = (BAR_START_DEGREE - (i * BAR_INCREMENT_DEGREES));

            float radian = radians(angle);

            int startX = x + (RADIUS * cos(radian));
            int startY = y - (RADIUS * sin(radian));

            int longitud_barra = (i % 2 == 0) ? LONG_BAR_LENGTH : SHORT_BAR_LENGTH;

            int endX = x + ((RADIUS + longitud_barra) * cos(radian));
            int endY = y - ((RADIUS + longitud_barra) * sin(radian));

            this->parentDisplayPtr->drawLine(startX, startY, endX, endY, TEXT_COLOR_ACTIVE);

            bool showLabel = (i == 0 || i == 2 || i == 6 || i == 10 || i == 14 || i == 18 || i == 22 || i == 26 || i == 30 || i == _DIGITAL_SMETER_WIDGET_BAR_COUNT);
            if (showLabel)
            {
                int labelX = endX + (20 * cos(radian)) - 5; // TODO: adjust centering
                int labelY = endY - (20 * sin(radian));
                this->parentDisplayPtr->setTextColor(TEXT_COLOR_ACTIVE);
                this->parentDisplayPtr->setTextSize(1);
                this->parentDisplayPtr->setCursor(labelX, labelY);
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
                this->parentDisplayPtr->printf("%s", label);
            }
        }
    }
    return (changed);
}
