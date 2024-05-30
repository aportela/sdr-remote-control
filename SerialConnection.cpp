#include <HardwareSerial.h>

#include <string.h>
#include "SerialConnection.h"

SerialConnection::SerialConnection(long speed, long timeout) {
  Serial.setDebugOutput(false);
  Serial.setTimeout(timeout);
  Serial.clearWriteError();
  Serial.begin(speed);
  while (!Serial) {
    yield();
    delay(10);
  }
}

void SerialConnection::flush(void) {
  Serial.flush();
  delay(10);
}

void SerialConnection::send(String str) {
  Serial.print(str);
  delay(10);
}

bool SerialConnection::tryConnection(void) {
  this->flush();
  this->send("PS;");
  bool connected = false;
  while (Serial.available() > 0 && !connected) {
    this->lastRXActivity = millis();
    String receivedData = Serial.readStringUntil(';');
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
  while (Serial.available() > 0) {
    this->lastRXActivity = millis();
    receivedData = Serial.readStringUntil(';');
  }
  delay(100);
  return (receivedData);
}