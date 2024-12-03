
#ifndef SDR_REMOTE_CONTROL_SMETER_H
#define SDR_REMOTE_CONTROL_SMETER_H

#define MIN_DB -54 // S0
#define MAX_DB 60  // S9+60

#include <cstdint>
#include <cstddef>

class SMeter
{
private:
    int8_t dB;

public:
    SMeter(int8_t currentdB = MIN_DB);
    ~SMeter();
    void set(int8_t currentdB);
    int8_t get(bool smooth = false);
};

#endif // SDR_REMOTE_CONTROL_SMETER_H
