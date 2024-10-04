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

#define _DIGITAL_SMETER_WIDGET_BAR_DISABLED_BACKGROUND_COLOR 0x8410

#define LABEL_FPS_X_OFFSET DISPLAY_WIDTH - 54
#define LABEL_FPS_Y_OFFSET 70

using namespace aportela::microcontroller::utils;

LGFXScreenConnected::LGFXScreenConnected(LovyanGFX *display, const TransceiverStatus *currentTransceiverStatusPtr) : LGFXScreen(display)
{
    if (display != nullptr)
    {
        this->parentDisplay->setTextDatum(TL_DATUM);
        this->vfoWidget = new LGFXDualVFOWidget(display, DUAL_VFO_WIDGET_WIDTH, DUAL_VFO_WIDGET_HEIGHT, DUAL_VFO_WIDGET_X_OFFSET, DUAL_VFO_WIDGET_Y_OFFSET, DUAL_VFO_WIDGET_PADDING, currentTransceiverStatusPtr);
        this->digitalSMeterWidget = new LGFXDigitalSMeterWidget(display, DIGITAL_SMETER_WIDGET_WIDTH, DIGITAL_SMETER_WIDGET_HEIGHT, DIGITAL_SMETER_WIDGET_X_OFFSET, DIGITAL_SMETER_WIDGET_Y_OFFSET, DIGITAL_SMETER_WIDGET_PADDING, currentTransceiverStatusPtr);
        this->volumeSquelchWidget = new LGFXVolumeSquelchWidget(display, VOLUME_SQUELCH_WIDGET_WIDTH, VOLUME_SQUELCH_WIDGET_HEIGHT, VOLUME_SQUELCH_WIDGET_X_OFFSET, VOLUME_SQUELCH_WIDGET_Y_OFFSET, VOLUME_SQUELCH_WIDGET_PADDING, currentTransceiverStatusPtr);
        this->filterWidget = new LGFXFilterWidget(display, FILTER_WIDGET_WIDTH, FILTER_WIDGET_HEIGHT, FILTER_WIDGET_X_OFFSET, FILTER_WIDGET_Y_OFFSET, FILTER_WIDGET_PADDING, currentTransceiverStatusPtr);
        this->menuWidget = new LGFXMenu(display, MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, MENU_WIDGET_X_OFFSET, MENU_WIDGET_Y_OFFSET, MENU_WIDGET_PADDING);
        if (currentTransceiverStatusPtr != nullptr)
        {
            this->Refresh(true, currentTransceiverStatusPtr);
        }
    }
}

LGFXScreenConnected::~LGFXScreenConnected()
{
    delete this->vfoWidget;
    this->vfoWidget = nullptr;
    delete this->digitalSMeterWidget;
    this->digitalSMeterWidget = nullptr;
    delete this->volumeSquelchWidget;
    this->volumeSquelchWidget = nullptr;
    delete this->filterWidget;
    this->filterWidget = nullptr;
    delete this->menuWidget;
    this->menuWidget = nullptr;
    this->parentDisplay = nullptr;
}

void LGFXScreenConnected::createDigitalSMeter()
{
    this->parentDisplay->setTextSize(_DIGITAL_SMETER_WIDGET_FONT_SIZE);
    this->parentDisplay->setTextColor(TEXT_COLOR_ACTIVE, TEXT_BACKGROUND_COLOR);

    // this->parentDisplay->setCursor(_DIGITAL_SMETER_WIDGET_TOP_LABELS_X_OFFSET, _DIGITAL_SMETER_WIDGET_TOP_LABELS_Y_OFFSET);
    // this->parentDisplay->print("1  3  5  7  9 +15 +30    +60");
    //  this->parentDisplay->setCursor(_DISPLAY_PADDING, 98);
    //  this->parentDisplay->print("S");
    // this->parentDisplay->setCursor(370, 102);
    // this->parentDisplay->print("S9+60dB");
    //  this->parentDisplay->drawFastVLine(23, 94, 22, TFT_WHITE);
    //  this->parentDisplay->drawFastVLine(284, 94, 22, TFT_WHITE);
    /*
    this->parentDisplay->drawFastHLine(23, 116, 262, TFT_WHITE);
    */
    for (int i = 1; i <= _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
    {
        int x = _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET + (i - 1) * (_DIGITAL_SMETER_WIDGET_BAR_WIDTH + _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN);
        if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 23 || i == 28 || i == 38)
        {
            this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_HIGH_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT, _DIGITAL_SMETER_WIDGET_BAR_DISABLED_BACKGROUND_COLOR);
        }
        else
        {
            this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_LOW_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT, _DIGITAL_SMETER_WIDGET_BAR_DISABLED_BACKGROUND_COLOR);
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
        for (int i = start; i <= _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
        {
            int x = _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET + i * (_DIGITAL_SMETER_WIDGET_BAR_WIDTH + _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN);
            if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
            {
                this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_HIGH_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT, _DIGITAL_SMETER_WIDGET_BAR_DISABLED_BACKGROUND_COLOR);
            }
            else
            {
                this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_LOW_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT, _DIGITAL_SMETER_WIDGET_BAR_DISABLED_BACKGROUND_COLOR);
            }
        }

        // PEAK
        for (int i = 0; i <= _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
        {
            if (i == this->peakSignal)
            {
                int x = _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET + i * (_DIGITAL_SMETER_WIDGET_BAR_WIDTH + _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN);
                // set more height on marked values
                if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
                {
                    this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_HIGH_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT, TFT_WHITE);
                }
                else
                {
                    this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_LOW_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT, TFT_WHITE);
                }
            }
        }

        // SIGNAL
        for (int i = 0; i <= _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
        {
            // real value draw
            if (i < this->currentSignal)
            {
                int x = _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET + i * (_DIGITAL_SMETER_WIDGET_BAR_WIDTH + _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN);
                // set more height on marked values
                if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
                {
                    this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_HIGH_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT, i <= 18 ? TFT_GREEN : TFT_RED);
                }
                else
                {
                    this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_LOW_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT, i <= 18 ? TFT_GREEN : TFT_RED);
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

                    for (int i = this->peakSignal + 1; i <= _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
                    {
                        int x = _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET + i * (_DIGITAL_SMETER_WIDGET_BAR_WIDTH + _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN);
                        if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
                        {
                            this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_HIGH_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT, _DIGITAL_SMETER_WIDGET_BAR_DISABLED_BACKGROUND_COLOR);
                        }
                        else
                        {
                            this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_LOW_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT, _DIGITAL_SMETER_WIDGET_BAR_DISABLED_BACKGROUND_COLOR);
                        }
                    }

                    for (int i = 0; i <= _DIGITAL_SMETER_WIDGET_BAR_COUNT; i++)
                    {
                        if (i == this->peakSignal)
                        {
                            int x = _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET + i * (_DIGITAL_SMETER_WIDGET_BAR_WIDTH + _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN);
                            // set more height on marked values
                            if (i == 2 || i == 4 || i == 6 || i == 8 || i == 10 || i == 12 || i == 14 || i == 16 || i == 18 || i == 22 || i == 26 || i == 30 || i == 34 || i == 38 || i == 42)
                            {
                                this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_HIGH_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT, TFT_WHITE);
                            }
                            else
                            {
                                this->parentDisplay->fillRect(x, _DIGITAL_SMETER_WIDGET_LOW_BARS_Y_OFFSET, _DIGITAL_SMETER_WIDGET_BAR_WIDTH, _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT, TFT_WHITE);
                            }
                        }
                    }
                }
            }
        }
    }
}

bool LGFXScreenConnected::Refresh(bool force, const TransceiverStatus *currentTransceiverStatusPtr)
{
    if (this->parentDisplay != nullptr && currentTransceiverStatusPtr != nullptr)
    {
        bool changed = false;
        if (this->vfoWidget->refresh(force, currentTransceiverStatusPtr))
        {
            changed = true;
        }
        if (this->digitalSMeterWidget->refresh(force, currentTransceiverStatusPtr))
        {
            changed = true;
        }
        if (this->volumeSquelchWidget->refresh(force, currentTransceiverStatusPtr))
        {
            changed = true;
        }
        if (this->filterWidget->refresh(force, currentTransceiverStatusPtr))
        {
            changed = true;
        }
        if (this->menuWidget->refresh(force))
        {
            changed = true;
        }
#ifdef DEBUG_FPS
        uint16_t currentFPS = FPS::GetFPS();
        if (force || this->previousFPS != currentFPS)
        {
            char str[10];
            snprintf(str, sizeof(str), "FPS: %03u", currentFPS);
            this->parentDisplay->setTextSize(1);
            this->parentDisplay->drawString(str, LABEL_FPS_X_OFFSET, LABEL_FPS_Y_OFFSET);
            changed = true;
        }
#endif

        return (changed);
    }
    else
    {
        return (false);
    }
}
