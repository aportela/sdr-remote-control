#include <Arduino.h>

#include "RotaryControl.hpp"

#define MAIN_VFO_ROTARY_ENCODER_PIN_A 25
#define MAIN_VFO_ROTARY_ENCODER_PIN_B 26
#define MAIN_VFO_ROTARY_ENCODER_BUTTON_PIN -1
#define MAIN_VFO_ROTARY_ENCODER_VCC_PIN -1 // put -1 of Rotary encoder Vcc is connected directly to 3,3V
#define MAIN_VFO_ROTARY_ENCODER_STEPS 4    // depending on your encoder - try 1,2 or 4 to get expected behaviour

#define MAIN_VFO_ROTARY_ENCODER_MIN_VALUE 0
#define MAIN_VFO_ROTARY_ENCODER_MAX_VALUE 9999
#define MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE 5000
#define MAIN_VFO_ROTARY_ENCODER_ACCELERATION_VALUE 100

void IRAM_ATTR readBigEncoderISRWithoutArgs()
{
    // mainVFORotaryEncoder.readEncoder_ISR();
}

static void readBigEncoderISRWithArgs(void *arg)
{
    RotaryControl *instance = static_cast<RotaryControl *>(arg);
    instance->encoder->readEncoder_ISR();

    instance->onUpdate();
}

RotaryControl::RotaryControl(int pinA, int pinB, uint8_t steps, uint8_t acceleration)
{
    this->encoder = new AiEsp32RotaryEncoder(MAIN_VFO_ROTARY_ENCODER_PIN_A, MAIN_VFO_ROTARY_ENCODER_PIN_B, MAIN_VFO_ROTARY_ENCODER_BUTTON_PIN, MAIN_VFO_ROTARY_ENCODER_VCC_PIN, MAIN_VFO_ROTARY_ENCODER_STEPS);
    this->encoder->begin();

    // this->encoder->setup(readBigEncoderISRWithoutArgs);
    attachInterruptArg(digitalPinToInterrupt(MAIN_VFO_ROTARY_ENCODER_PIN_A), readBigEncoderISRWithArgs, this, CHANGE);
    attachInterruptArg(digitalPinToInterrupt(MAIN_VFO_ROTARY_ENCODER_PIN_B), readBigEncoderISRWithArgs, this, CHANGE);
    this->encoder->setBoundaries(MAIN_VFO_ROTARY_ENCODER_MIN_VALUE, MAIN_VFO_ROTARY_ENCODER_MAX_VALUE, true);
    this->encoder->setAcceleration(MAIN_VFO_ROTARY_ENCODER_ACCELERATION_VALUE);
    this->encoder->setEncoderValue(MAIN_VFO_ROTARY_ENCODER_CENTER_VALUE);
}

RotaryControl::~RotaryControl()
{
    delete this->encoder;
    this->encoder = nullptr;
}

void RotaryControl::onUpdate(void)
{
    int16_t delta = this->encoder->encoderChanged();
    if (delta > 0)
    {
        int32_t newEncoderValue = this->encoder->readEncoder();
        int32_t hzIncrement = 0;
        if (newEncoderValue > 5030)
        {
            hzIncrement = 1000;
        }
        else if (newEncoderValue > 5025)
        {
            hzIncrement = 100;
        }
        else if (newEncoderValue > 5015)
        {
            hzIncrement = 100;
        }
        else
        {
            hzIncrement = 1;
        }
        this->onChange(hzIncrement);
    }
    else if (delta < 0)
    {
        int32_t newEncoderValue = this->encoder->readEncoder();
        int32_t hzDecrement = 0;
        if (newEncoderValue < 4970)
        {
            hzDecrement = -1000;
        }
        else if (newEncoderValue < 4975)
        {
            hzDecrement = -100;
        }
        else if (newEncoderValue < 4985)
        {
            hzDecrement = -10;
        }
        else
        {
            hzDecrement = -1;
        }
        this->onChange(hzDecrement);
    }
}