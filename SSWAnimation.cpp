#include "SSWAnimation.h"

// generate random noise data
void SSWAnimation::refreshNoise(void) {
  const uint8_t MAX_VALUE = SSWA_MIN_SIGNAL_VALUE / 2;
  const uint8_t MIN_PEAK_INTERVAL = 10;
  const uint8_t MAX_PEAK_INTERVAL = 30;
  for (uint16_t i = 0; i < SSWA_WIDTH; i++) {
    uint8_t currentValue = random(0, SSWA_MAX_NOISE_VALUE);
    uint8_t nextPeak = random(MIN_PEAK_INTERVAL, MAX_PEAK_INTERVAL);
    if (i % nextPeak == 0) {
      uint8_t peakValue = random(SSWA_MIN_NOISE_VALUE, SSWA_MAX_NOISE_VALUE);
      int8_t direction = random(0, 2) * 2 - 1;
      for (int j = i; j < i + nextPeak && j < SSWA_WIDTH; j++) {
        this->noiseData[j] = max(0, min(255, currentValue + direction * abs(currentValue - peakValue) / nextPeak * (j - i)));
      }
      currentValue = peakValue;
      nextPeak = random(MIN_PEAK_INTERVAL, MAX_PEAK_INTERVAL);
    }
  }
}

// generate start/default signals levels
void SSWAnimation::initSignals(void) {
  // left block are "cw/morse" signals
  // random spacing between signals
  uint16_t currentSignalIndex = random(SSWA_MIN_DIST_BETWEEN_CW_SIGNALS, SSWA_MAX_DIST_BETWEEN_CW_SIGNALS);
  for (uint16_t i = 0; i < SSWA_TOTAL_CW_SIGNALS_BANDWITH; i++) {
    // set by default that this is a "noise" (not real signal)
    this->signalIndexes[i] = 0;
    if (i != 0) {
      // when reach next signal index
      if (i == currentSignalIndex) {
        // generate signal level
        this->signalsData[i] = random(SSWA_MIN_SIGNAL_VALUE, SSWA_MAX_SIGNAL_VALUE);
        // change "noise" to -> "signal"
        this->signalIndexes[i] = 1;
        // generate next signal index
        currentSignalIndex += random(SSWA_MIN_DIST_BETWEEN_CW_SIGNALS, SSWA_MAX_DIST_BETWEEN_CW_SIGNALS);
      } else {
        // no signal at this index => get "noise" data
        this->signalsData[i] = this->noiseData[i];
      }
    } else {
      // first index (0) => get "noise" data
      this->signalsData[i] = this->noiseData[i];
    }
  }

  // right block are "analog voice" signals
  // random spacing between signals
  currentSignalIndex = SSWA_TOTAL_CW_SIGNALS_BANDWITH + random(SSWA_MIN_DIST_BETWEEN_ANALOG_VOICE_SIGNALS, SSWA_MAX_DIST_BETWEEN_ANALOG_VOICE_SIGNALS);
  // generate signal level
  uint8_t currentSignalValue = random(SSWA_MIN_SIGNAL_VALUE, SSWA_MAX_SIGNAL_VALUE);
  // set "noise" (not real signal) by default
  for (uint16_t i = SSWA_TOTAL_CW_SIGNALS_BANDWITH; i < SSWA_WIDTH; i++) {
    this->signalIndexes[i] = 0;
  }
  for (uint16_t i = SSWA_TOTAL_CW_SIGNALS_BANDWITH; i < SSWA_WIDTH; i++) {
    if (i != 0) {
      // when reach next signal index
      if (i == currentSignalIndex) {
        // allow near signal indexes
        if (i > 4 && i < SSWA_WIDTH - 4) {
          // set signal level
          this->signalsData[i] = currentSignalValue;
          // analog voice signal requires more bandwith than "cw/morse" so re-generate also near signal indexes with random values
          this->signalsData[i - 1] = constrain(this->signalsData[i] - random(1, 4), 0, 255);
          this->signalsData[i - 2] = constrain(this->signalsData[i - 1] - random(1, 8), 0, 255);
          this->signalsData[i - 3] = constrain(this->signalsData[i - 2] - random(1, 16), 0, 255);
          this->signalsData[i - 4] = constrain(this->signalsData[i - 3] - random(1, 32), 0, 255);
          this->signalsData[i - 5] = constrain(this->signalsData[i - 4] - random(1, 64), 0, 255);
          this->signalsData[i + 1] = constrain(this->signalsData[i] - random(1, 4), 0, 255);
          this->signalsData[i + 2] = constrain(this->signalsData[i + 1] - random(1, 8), 0, 255);
          this->signalsData[i + 3] = constrain(this->signalsData[i + 2] - random(1, 16), 0, 255);
          this->signalsData[i + 4] = constrain(this->signalsData[i + 3] - random(1, 32), 0, 255);
          this->signalsData[i + 5] = constrain(this->signalsData[i + 4] - random(1, 64), 0, 255);
          // change "noise" to -> "signal start"
          this->signalIndexes[i - 5] = 4;
          // change "noise" to -> "intermediate signal"
          this->signalIndexes[i - 4] = 2;
          this->signalIndexes[i - 3] = 2;
          this->signalIndexes[i - 2] = 2;
          this->signalIndexes[i - 1] = 2;
          // change "noise" to -> "signal" (peak)
          this->signalIndexes[i] = 1;
          // change "noise" to -> "intermediate signal"
          this->signalIndexes[i + 1] = 3;
          this->signalIndexes[i + 2] = 3;
          this->signalIndexes[i + 3] = 3;
          this->signalIndexes[i + 4] = 3;
          // change "noise" to -> "signal end"
          this->signalIndexes[i + 5] = 5;

          // generate next signal index
          currentSignalIndex += random(SSWA_MIN_DIST_BETWEEN_ANALOG_VOICE_SIGNALS, SSWA_MAX_DIST_BETWEEN_ANALOG_VOICE_SIGNALS);
          // regenerate signal level
          currentSignalValue = random(SSWA_MIN_SIGNAL_VALUE, SSWA_MAX_SIGNAL_VALUE);
        } else {
          this->signalsData[i] = this->noiseData[i];
        }
      } else {
        // check (ONLY) for existing noise at this index
        if (this->signalIndexes[i] == 0) {
          // no signal at this index => get "noise" data
          this->signalsData[i] = this->noiseData[i];
        }
      }
    } else {
      // first index (0) => get "noise" data
      this->signalsData[i] = this->noiseData[i];
    }
  }
}

// regenerate signals levels
void SSWAnimation::refreshSignals(void) {
  // re/generate "noise"
  this->refreshNoise();
  // left block are "cw/morse" signals
  for (uint16_t i = 0; i < SSWA_TOTAL_CW_SIGNALS_BANDWITH; i++) {
    // ignore existing signal and change only to new "noise" level
    if (this->signalIndexes[i] == 0) {
      // no signal at this index => get "noise" data
      this->signalsData[i] = this->noiseData[i];
    }
  }
  // right block are "analog voice" signals
  for (uint16_t i = SSWA_TOTAL_CW_SIGNALS_BANDWITH; i < SSWA_WIDTH; i++) {
    // ignore noise, only eval real signals (peak & near data)
    if (this->signalIndexes[i] > 0) {
      // check signal existence on current index
      if (this->signalIndexes[i] == 1) {
        // re-generate near signal indexes with random values
        this->signalsData[i - 1] = constrain(this->signalsData[i] - random(4, 8), 0, 255);
        this->signalsData[i - 2] = constrain(this->signalsData[i - 1] - random(4, 32), 0, 255);
        this->signalsData[i - 3] = constrain(this->signalsData[i - 2] + random(4, 40), 0, 255);
        this->signalsData[i - 4] = constrain(this->signalsData[i - 3] - random(4, 8), 0, 255);
        this->signalsData[i - 5] = constrain(this->signalsData[i - 4] - random(4, 16), 0, 255);
        this->signalsData[i + 1] = constrain(this->signalsData[i] - random(4, 8), 0, 255);
        this->signalsData[i + 2] = constrain(this->signalsData[i + 1] - random(4, 32), 0, 255);
        this->signalsData[i + 3] = constrain(this->signalsData[i + 2] + random(4, 40), 0, 255);
        this->signalsData[i + 4] = constrain(this->signalsData[i + 3] - random(4, 8), 0, 255);
        this->signalsData[i + 5] = constrain(this->signalsData[i + 4] - random(4, 16), 0, 255);
      }
    } else {
      // first index (0) => get "noise" data
      this->signalsData[i] = this->noiseData[i];
    }
  }
}
