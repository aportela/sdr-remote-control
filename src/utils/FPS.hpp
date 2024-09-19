#ifndef APORTELA_MICROCONTROLLER_UTILS_FPS_H
#define APORTELA_MICROCONTROLLER_UTILS_FPS_H

#include <cstdint>

namespace aportela::microcontroller::utils
{
  /*
    WARNING: THIS IS NOT A "REAL FPS DATA", ONLY AN APROXIMATTION DATA FOR BENCH & OPTIMIZE CODE FOR MAX SCREEN UPDATES
  */
  class FPS
  {
  private:
    static uint64_t frameCount;
    static uint64_t lastTime;
    static uint16_t fps;

  public:
    static void Init(void);
    static void Loop(uint16_t limit = 0);
    static uint16_t GetFPS(void);
  };
}

#endif // APORTELA_MICROCONTROLLER_UTILS_FPS_H
