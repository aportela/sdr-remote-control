#include <Arduino.h>
#include "FPS.hpp"

namespace aportela::microcontroller::utils
{
  uint64_t FPS::frameCount = 0;
  uint64_t FPS::lastTime = 0;
  uint16_t FPS::fps = 0;

  void FPS::Init(void)
  {
    FPS::frameCount = 0;
    FPS::lastTime = 0;
    FPS::fps = 0;
  }

  void FPS::Loop(uint16_t limit)
  {
    uint64_t currentTime = millis();
    uint64_t elapsedTime = currentTime - FPS::lastTime;
    if (elapsedTime >= 1000)
    {
      FPS::fps = static_cast<uint16_t>(static_cast<float>(FPS::frameCount) / (static_cast<float>(elapsedTime) / 1000.0));
      FPS::frameCount = 0;
      FPS::lastTime = currentTime;
    }
    if (limit == 0 || FPS::frameCount < limit)
    {
      FPS::frameCount++;
    }
  }

  uint16_t FPS::GetFPS(void)
  {
    return (FPS::fps);
  }
}
