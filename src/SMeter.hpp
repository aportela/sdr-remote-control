
#ifndef SDR_REMOTE_CONTROL_SMETER_H
#define SDR_REMOTE_CONTROL_SMETER_H

#define SMETER_MIN_DB -54 // S0
#define SMETER_MAX_DB 60  // S9+60

#define SMETER_SMOOTH_INCREASE_MS_INTERVAL 3 // 2
#define SMETER_SMOOTH_DECREASE_MS_INTERVAL 8 // 5

#include <cstdint>
#include <cstddef>

class SMeter
{
private:
    int8_t dB;
    int8_t dBSMoothed;
    uint64_t lastSmoothTimestamp = 0;

public:
    SMeter(int8_t currentdB = SMETER_MIN_DB);
    ~SMeter();
    void set(int8_t currentdB);
    int8_t get(bool smooth = false);
};

#endif // SDR_REMOTE_CONTROL_SMETER_H
