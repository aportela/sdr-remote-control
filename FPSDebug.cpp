#include <Arduino.h>
#include "FPSDebug.h"

uint16_t FPSDebug::getFPS(void) {
  return (this->fps);
}

void FPSDebug::loop(void) {
  uint64_t currentTime = millis();
  uint64_t elapsedTime = currentTime - lastTime;
  if (elapsedTime >= 1000) {
    this->fps = static_cast<uint16_t>(static_cast<float>(this->frameCount) / (static_cast<float>(elapsedTime) / 1000.0));
    this->frameCount = 0;
    this->lastTime = currentTime;
  }
  this->frameCount++;
}