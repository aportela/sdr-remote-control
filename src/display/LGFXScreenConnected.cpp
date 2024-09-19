#include <Arduino.h>
#include "LGFXScreenConnected.hpp"
#include "../DisplayConfiguration.hpp"

#ifdef DEBUG_FPS
#include "../utils/FPS.hpp"
#endif

#ifdef DISPLAY_LOVYANN_ILI9488_480x320

#include "ILI9488/ScreenConnectedDefines.hpp"

#elif defined(DISPLAY_LOVYANN_ST7789_240x320)

#include "ST7789/ScreenConnectedDefines.hpp"

#else

#error NO DISPLAY DEFINED

#endif // DISPLAY_LOVYANN_ILI9488_480x320

#define TEXT_COLOR_ACTIVE TFT_WHITE
#define TEXT_COLOR_SECONDARY 0x528A
#define TEXT_COLOR_NOT_ACTIVE 0x18C3
#define TEXT_BACKGROUND TFT_BLACK

using namespace aportela::microcontroller::utils;

LGFXScreenConnected::LGFXScreenConnected(LovyanGFX *display, Transceiver *trx) : LGFXScreen(display), trx(trx)
{
    if (display != nullptr)
    {
        this->vfoWidget = new LGFXDualVFOWidget(display, DUAL_VFO_WIDGET_WIDTH, DUAL_VFO_WIDGET_HEIGHT, DUAL_VFO_WIDGET_X_OFFSET, DUAL_VFO_WIDGET_Y_OFFSET, trx);
        //  this->createDigitalSMeter();
        //  this->Refresh(true);
    }
}

LGFXScreenConnected::~LGFXScreenConnected()
{
    delete this->vfoWidget;
    this->vfoWidget = nullptr;
    this->parentDisplay = nullptr;
}

void LGFXScreenConnected::createDigitalSMeter()
{
#ifdef DEBUG_SCREEN_BOUNDS
    this->parentDisplay->drawRect(
        DISPLAY_PADDING,
        (DISPLAY_PADDING * 2) + (DUAL_VFO_WIDGET_SINGLE_VFO_TOTAL_HEIGHT * 2),
        DISPLAY_WIDTH - (DISPLAY_PADDING * 2),
        48,
        TFT_RED);
#endif
    this->parentDisplay->setTextSize(SMETER_FONT_SIZE);
    this->parentDisplay->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND);

    // this->parentDisplay->setCursor(SMETER_VALUES_LABEL_X_OFFSET, SMETER_VALUES_LABEL_Y_OFFSET);
    // this->parentDisplay->print("1  3  5  7  9 +15 +30    +60");
    //  this->parentDisplay->setCursor(DISPLAY_PADDING, 98);
    //  this->parentDisplay->print("S");
    // this->parentDisplay->setCursor(370, 102);
    // this->parentDisplay->print("S9+60dB");
    //  this->parentDisplay->drawFastVLine(23, 94, 22, TFT_WHITE);
    //  this->parentDisplay->drawFastVLine(284, 94, 22, TFT_WHITE);
    /*
    this->parentDisplay->drawFastHLine(23, 116, 262, TFT_WHITE);
    */
    for (int i = 1; i <= SMETER_BAR_COUNT; i++)
    {
        int x = SMETER_BARS_X_OFFSET + (i - 1) * (SMETER_BAR_WIDTH + SMETER_BAR_HORIZONTAL_MARGIN);
        if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 23 || i == 28 || i == 38)
        {
            this->parentDisplay->fillRect(x, SMETER_HIGH_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_HIGH_BAR_HEIGHT, SMETER_BAR_BG_COLOR);
        }
        else
        {
            this->parentDisplay->fillRect(x, SMETER_LOW_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_LOW_BAR_HEIGHT, SMETER_BAR_BG_COLOR);
        }
    }
}

void LGFXScreenConnected::refreshDigitalSMeter(uint8_t newSignal)
{
    this->oldSignal = this->currentSignal;
    // signal changed
    if (this->oldSignal != newSignal)
    {
        if (newSignal >= this->oldSignal)
        {
            // signal increased
            this->currentSignal = newSignal;
            if (newSignal > this->oldSignal)
            {
                this->peakSignal = newSignal;
                this->lastPeakChange = millis();
            }
        }
        else if (newSignal < this->oldSignal)
        {
            // this->peakSignal = this->oldSignal;
            long current = millis();
            if (current - this->lastPeakChange > 200)
            {
                this->peakSignal--;
                this->lastPeakChange = current;
            }
            this->currentSignal = this->oldSignal - 1;
        }
        if (this->currentSignal < 0)
        {
            this->currentSignal = 0;
        }
        else if (this->currentSignal > 42)
        {
            this->currentSignal = 42;
        }
        int start = 0;
        if (this->oldSignal < this->currentSignal)
        {
            start = this->oldSignal;
        }
        else if (this->oldSignal > this->currentSignal)
        {
            start = this->currentSignal;
        }
        else
        {
            start = 0;
        }
        if (this->peakSignal < 0)
        {
            this->peakSignal = 0;
        }

        /*
        this->parentDisplay->setCursor(20, 160);
        this->parentDisplay->setTextSize(2);
        this->parentDisplay->setTextColor(TFT_WHITE, TFT_BLACK);
        this->parentDisplay->printf("Current: %u", this->currentSignal);
        this->parentDisplay->setCursor(20, 180);
        this->parentDisplay->setTextSize(2);
        this->parentDisplay->setTextColor(TFT_WHITE, TFT_BLACK);
        this->parentDisplay->printf("Old: %u", this->oldSignal);

        this->parentDisplay->setCursor(20, 200);
        this->parentDisplay->setTextSize(2);
        this->parentDisplay->setTextColor(TFT_WHITE, TFT_BLACK);
        this->parentDisplay->printf("Peak: %u", this->peakSignal);
        */

        // BG
        for (int i = start; i <= SMETER_BAR_COUNT; i++)
        {
            int x = SMETER_BARS_X_OFFSET + i * (SMETER_BAR_WIDTH + SMETER_BAR_HORIZONTAL_MARGIN);
            if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
            {
                this->parentDisplay->fillRect(x, SMETER_HIGH_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_HIGH_BAR_HEIGHT, SMETER_BAR_BG_COLOR);
            }
            else
            {
                this->parentDisplay->fillRect(x, SMETER_LOW_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_LOW_BAR_HEIGHT, SMETER_BAR_BG_COLOR);
            }
        }

        // PEAK
        for (int i = 0; i <= SMETER_BAR_COUNT; i++)
        {
            if (i == this->peakSignal)
            {
                int x = SMETER_BARS_X_OFFSET + i * (SMETER_BAR_WIDTH + SMETER_BAR_HORIZONTAL_MARGIN);
                // set more height on marked values
                if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
                {
                    this->parentDisplay->fillRect(x, SMETER_HIGH_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_HIGH_BAR_HEIGHT, TFT_WHITE);
                }
                else
                {
                    this->parentDisplay->fillRect(x, SMETER_LOW_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_LOW_BAR_HEIGHT, TFT_WHITE);
                }
            }
        }

        // SIGNAL
        for (int i = 0; i <= SMETER_BAR_COUNT; i++)
        {
            // real value draw
            if (i < this->currentSignal)
            {
                int x = SMETER_BARS_X_OFFSET + i * (SMETER_BAR_WIDTH + SMETER_BAR_HORIZONTAL_MARGIN);
                // set more height on marked values
                if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
                {
                    this->parentDisplay->fillRect(x, SMETER_HIGH_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_HIGH_BAR_HEIGHT, i <= 18 ? TFT_GREEN : TFT_RED);
                }
                else
                {
                    this->parentDisplay->fillRect(x, SMETER_LOW_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_LOW_BAR_HEIGHT, i <= 18 ? TFT_GREEN : TFT_RED);
                }
            }
        }
    }
    else
    {
        if (this->peakSignal > this->currentSignal)
        {
            long current = millis();
            if (current - this->lastPeakChange > 200)
            {
                this->peakSignal--;
                if (this->peakSignal > 0)
                {
                    this->peakSignal--;
                    this->lastPeakChange = current;

                    for (int i = this->peakSignal + 1; i <= SMETER_BAR_COUNT; i++)
                    {
                        int x = SMETER_BARS_X_OFFSET + i * (SMETER_BAR_WIDTH + SMETER_BAR_HORIZONTAL_MARGIN);
                        if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
                        {
                            this->parentDisplay->fillRect(x, SMETER_HIGH_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_HIGH_BAR_HEIGHT, SMETER_BAR_BG_COLOR);
                        }
                        else
                        {
                            this->parentDisplay->fillRect(x, SMETER_LOW_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_LOW_BAR_HEIGHT, SMETER_BAR_BG_COLOR);
                        }
                    }

                    for (int i = 0; i <= SMETER_BAR_COUNT; i++)
                    {
                        if (i == this->peakSignal)
                        {
                            int x = SMETER_BARS_X_OFFSET + i * (SMETER_BAR_WIDTH + SMETER_BAR_HORIZONTAL_MARGIN);
                            // set more height on marked values
                            if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
                            {
                                this->parentDisplay->fillRect(x, SMETER_HIGH_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_HIGH_BAR_HEIGHT, TFT_WHITE);
                            }
                            else
                            {
                                this->parentDisplay->fillRect(x, SMETER_LOW_BARS_Y_OFFSET, SMETER_BAR_WIDTH, SMETER_LOW_BAR_HEIGHT, TFT_WHITE);
                            }
                        }
                    }
                }
            }
        }
    }
}

bool LGFXScreenConnected::Refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        bool changed = this->trx->changed > 0;
        this->vfoWidget->refresh(force);
        /*


        // TODO: only refresh active VFO
        if (this->trx->changed & TRX_CFLAG_TRANSMIT_RECEIVE_POWER_STATUS)
        {
            // this->refreshTransmitStatus(false);
            this->trx->changed &= ~TRX_CFLAG_TRANSMIT_RECEIVE_POWER_STATUS;
        }
        if (force || (this->trx->changed & TRX_CFLAG_VFO_INDEX))
        {
            this->refreshVFOIndex(0, this->trx->activeVFOIndex == 0);
            this->refreshVFOIndex(1, this->trx->activeVFOIndex == 1);
            this->trx->changed &= ~TRX_CFLAG_VFO_INDEX;
        }
        if (force || (this->trx->changed & TRX_CFLAG_ACTIVE_VFO_FREQUENCY))
        {
            this->refreshVFOFreq(0, this->trx->activeVFOIndex == 0, this->trx->VFO[0].frequency);
            this->trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_FREQUENCY;
        }
        if (force || (this->trx->changed & TRX_CFLAG_ACTIVE_VFO_MODE))
        {
            this->refreshVFOMode(0, this->trx->activeVFOIndex == 0, this->trx->VFO[0].mode);
            this->trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_MODE;
        }
        if (force || (this->trx->changed & TRX_CFLAG_ACTIVE_VFO_STEP))
        {
            this->refreshVFOStep(0, this->trx->activeVFOIndex == 0, this->trx->VFO[0].customStep);
            this->trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_STEP;
        }
        if (force || (this->trx->changed & TRX_CFLAG_ACTIVE_VFO_FILTER_LOW))
        {
            // this->refreshPassBandFilter(this->trx->VFO[this->trx->activeVFOIndex].LF, this->trx->VFO[this->trx->activeVFOIndex].HF, this->trx->VFO[this->trx->activeVFOIndex].BW);
            this->trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_FILTER_LOW;
        }
        if (force || (this->trx->changed & TRX_CFLAG_ACTIVE_VFO_FILTER_HIGH))
        {
            // this->refreshPassBandFilter(this->trx->VFO[this->trx->activeVFOIndex].LF, this->trx->VFO[this->trx->activeVFOIndex].HF, this->trx->VFO[this->trx->activeVFOIndex].BW);
            this->trx->changed &= ~TRX_CFLAG_ACTIVE_VFO_FILTER_HIGH;
        }
        if (force || (this->trx->changed & TRX_CFLAG_SECONDARY_VFO_FREQUENCY))
        {
            this->refreshVFOFreq(1, this->trx->activeVFOIndex == 1, this->trx->VFO[1].frequency);
            this->trx->changed &= ~TRX_CFLAG_SECONDARY_VFO_FREQUENCY;
        }
        if (force || (this->trx->changed & TRX_CFLAG_SECONDARY_VFO_MODE))
        {
            this->refreshVFOMode(1, this->trx->activeVFOIndex == 1, this->trx->VFO[1].mode);
            this->trx->changed &= ~TRX_CFLAG_SECONDARY_VFO_MODE;
        }
        if (force || (this->trx->changed & TRX_CFLAG_SECONDARY_VFO_STEP))
        {
            this->refreshVFOStep(1, this->trx->activeVFOIndex == 1, this->trx->VFO[1].customStep);
            this->trx->changed &= ~TRX_CFLAG_SECONDARY_VFO_STEP;
        }
        if (force || (this->trx->changed & TRX_CFLAG_SECONDARY_VFO_FILTER_LOW))
        {
            // this->refreshPassBandFilter(this->trx->VFO[this->trx->activeVFOIndex].LF, this->trx->VFO[this->trx->activeVFOIndex].HF, this->trx->VFO[this->trx->activeVFOIndex].BW);
            this->trx->changed &= ~TRX_CFLAG_SECONDARY_VFO_FILTER_LOW;
        }
        if (force || (this->trx->changed & TRX_CFLAG_SECONDARY_VFO_FILTER_HIGH))
        {
            // this->refreshPassBandFilter(this->trx->VFO[this->trx->activeVFOIndex].LF, this->trx->VFO[this->trx->activeVFOIndex].HF, this->trx->VFO[this->trx->activeVFOIndex].BW);
            this->trx->changed &= ~TRX_CFLAG_SECONDARY_VFO_FILTER_HIGH;
        }
        if (force || (this->trx->changed & TRX_CFLAG_SIGNAL_METER_DB_LEVEL))
        {
            // this->smeterDigitalPtr->refresh(this->trx->signalMeterLevel);
            this->trx->changed &= ~TRX_CFLAG_SIGNAL_METER_DB_LEVEL;
        }
        if (force || ((this->trx->changed & TRX_CFLAG_AF_GAIN) || (this->trx->changed & TRX_CFLAG_AUDIO_MUTE)))
        {
            // this->refreshVolume(this->trx->AFGain, this->trx->audioMuted);
            this->trx->changed &= ~TRX_CFLAG_AF_GAIN;
            this->trx->changed &= ~TRX_CFLAG_AUDIO_MUTE;
        }
#ifdef DEBUG_FPS
        uint16_t currentFPS = FPS::GetFPS();
        if (force || this->previousFPS != currentFPS)
        {
            this->parentDisplay->setTextColor(0xF85E, TEXT_BACKGROUND);
            this->parentDisplay->setCursor(DISPLAY_PADDING, DISPLAY_HEIGHT - 16);
            this->parentDisplay->setTextSize(1);
            this->parentDisplay->printf("%03u FPS", currentFPS);
            changed = true;
        }
#endif
        // this->menuPtr->refresh(false);
        */
        return (changed);
    }
    else
    {
        return (false);
    }
}
