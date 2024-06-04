
#include "SerialConnection.h"

// TODO: ADJUST
#define MILLISECONDS_BETWEEN_LOOP 10
#define MILLISECONDS_WAITED_AFTER_FLUSH 5
#define MILLISECONDS_WAITED_AFTER_SEND 10

SerialConnection::SerialConnection(HardwareSerial* serialPort, long speed, long timeout)
  : serial(serialPort) {
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
  this->lastTXActivity = millis();
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
      this->lastRXValidCommand = millis();
      this->flush();
      connected = true;
      break;
    }
  }
  return (connected);
}

void SerialConnection::loop(Transceiver* trx) {
  if (millis() - this->lastTXActivity > MILLISECONDS_BETWEEN_LOOP) {
    if (trx != nullptr) {
      if (trx->powerStatus == TRX_PS_ON) {
        // connected => main
        this->send("FA;");
        this->send("MD;");
        while (this->serial->available() > 0) {
          this->lastRXActivity = millis();
          String receivedData = this->serial->readStringUntil(';');
          if (receivedData.startsWith("FA") && receivedData != "FA") {
            this->lastRXValidCommand = millis();
            trx->setVFOFrequency(trx->activeVFOIndex, receivedData.substring(2).toInt());
          } else if (receivedData.startsWith("MD") && receivedData != "MD") {
            this->lastRXValidCommand = millis();
            trx->setVFOMode(trx->activeVFOIndex, (TRXVFOMode) receivedData.substring(2).toInt());
          }
        }
      }
    }
  }
}