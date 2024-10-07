#include "LGFXTransceiverStatusWidget.hpp"

LGFXTransceiverStatusWidget::LGFXTransceiverStatusWidget(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding), currentTransceiverStatusPtr(currentTransceiverStatusPtr)
{
}

LGFXTransceiverStatusWidget::~LGFXTransceiverStatusWidget()
{
    this->currentTransceiverStatusPtr = nullptr;
}
