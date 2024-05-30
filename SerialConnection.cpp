
#include "SerialConnection.h"

#define MILLISECONDS_WAITED_AFTER_FLUSH 5
#define MILLISECONDS_WAITED_AFTER_SEND 10

SerialConnection::SerialConnection(HardwareSerial* serialPort, long speed, long timeout) : serial(serialPort) {
  this->serial->setDebugOutput(false);
  this->serial->setTimeout(timeout);
  this->serial->clearWriteError();
  this->serial->begin(speed);
  while (!Serial) {
    yield();
    delay(10);
  }
}

void SerialConnection::flush(void) {
  this->serial->flush();
  delay(MILLISECONDS_WAITED_AFTER_FLUSH);
}

void SerialConnection::send(String str) {
  this->serial->print(str);
  delay(MILLISECONDS_WAITED_AFTER_SEND);
}

bool SerialConnection::tryConnection(void) {
  this->flush();
  this->send("PS;");
  bool connected = false;
  while (this->serial->available() > 0 && !connected) {
    this->lastRXActivity = millis();
    String receivedData = this->serial->readStringUntil(';');
    if (receivedData == "PS1") {
      this->flush();
      connected = true;
      break;
    }
  }
  return (connected);
}

String SerialConnection::loop(void) {
  this->flush();
  this->send("PS;");
  String receivedData;
  while (this->serial->available() > 0) {
    this->lastRXActivity = millis();
    receivedData = this->serial->readStringUntil(';');
  }
  return (receivedData);
}