#ifndef SDR_REMOTE_CONTROL_KEYPAD8_I2C_CONTROL_H
#define SDR_REMOTE_CONTROL_KEYPAD8_I2C_CONTROL_H

#include <stdint.h>
#include <Adafruit_MCP23X17.h>

class Keypad8I2C
{
private:
    Adafruit_MCP23X17 mcp;
    uint8_t *pins = nullptr;
    size_t pinCount = 0;

public:
    Keypad8I2C(uint8_t i2c_addr = MCP23XXX_ADDR, const uint8_t *pinArray = nullptr, const size_t pinArraySize = 0);
    ~Keypad8I2C();
    uint8_t loop(void);
};

#endif // SDR_REMOTE_CONTROL_KEYPAD8_I2C_CONTROL_H
