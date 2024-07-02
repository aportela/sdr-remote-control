
#include "SerialConnection.hpp"

SerialConnection::SerialConnection(HardwareSerial *serialPort, long speed, long timeout)
    : serial(serialPort)
{
    this->serial->setDebugOutput(false);
    this->serial->setTimeout(timeout);
    this->serial->clearWriteError();
    this->serial->begin(speed);
    while (!Serial)
    {
        yield();
        delay(10);
    }
}

void SerialConnection::flush(void)
{
    this->serial->flush();
    delay(MILLISECONDS_WAITED_AFTER_FLUSH);
}

void SerialConnection::rxFlush(void)
{
    while (Serial.available() > 0)
    {
        Serial.read();
    }
    delay(MILLISECONDS_WAITED_AFTER_FLUSH);
}

void SerialConnection::send(String str)
{
    this->serial->print(str);
    this->lastTXActivity = millis();
    delay(MILLISECONDS_WAITED_AFTER_SEND);
}

bool SerialConnection::isDisconnectedByTimeout(void)
{
    return (millis() - this->lastRXValidCommand > MILLISECONDS_TO_WAIT_BEFORE_TIMEOUT);
}