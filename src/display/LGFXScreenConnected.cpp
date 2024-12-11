#include <Arduino.h>
#include "LGFXScreenConnected.hpp"
#include "../DisplayConfiguration.hpp"
#include "../CommonDefines.hpp"

#ifdef DEBUG_FPS

#include "../utils/FPS.hpp"

#endif // DEBUG_FPS

#ifdef DISPLAY_LOVYANGFX_ILI9488_480x320

#include "ILI9488/ScreenConnectedDefines.hpp"

#elif defined(DISPLAY_LOVYANGFX_ST7789_240x320)

#include "ST7789/ScreenConnectedDefines.hpp"

#else

#error NO DISPLAY DEFINED

#endif // DISPLAY_LOVYANGFX_ILI9488_480x320

#define _DIGITAL_SMETER_WIDGET_BAR_DISABLED_BACKGROUND_COLOR 0x8410

using namespace aportela::microcontroller::utils;

LGFXScreenConnected::LGFXScreenConnected(LovyanGFX *display, const TransceiverStatus *currentTransceiverStatusPtr, Menu *menuPtr) : LGFXScreen(display)
{
    if (display != nullptr)
    {
        this->parentDisplay->setTextDatum(TL_DATUM);
        if (currentTransceiverStatusPtr != nullptr)
        {

            this->primaryVFOWidget = new LGFXVFOWidget(display, VFO_WIDGET_WIDTH, VFO_WIDGET_HEIGHT, VFO_WIDGET_X_OFFSET, VFO_PRIMARY_WIDGET_Y_OFFSET, VFO_WIDGET_PADDING, 0, currentTransceiverStatusPtr);
            this->secondaryVFOWidget = new LGFXVFOWidget(display, VFO_WIDGET_WIDTH, VFO_WIDGET_HEIGHT, VFO_WIDGET_X_OFFSET, VFO_SECONDARY_WIDGET_Y_OFFSET, VFO_WIDGET_PADDING, 1, currentTransceiverStatusPtr);
            /*

#ifdef USE_ANALOG_SMETER_WIDGET
            this->analogSMeterWidget = new LGFXAnalogSMeterWidget(display, SMETER_WIDGET_WIDTH, SMETER_WIDGET_HEIGHT, SMETER_WIDGET_X_OFFSET, SMETER_WIDGET_Y_OFFSET, SMETER_WIDGET_PADDING, currentTransceiverStatusPtr);
#else
            this->digitalSMeterWidget = new LGFXDigitalSMeterWidget(display, SMETER_WIDGET_WIDTH, SMETER_WIDGET_HEIGHT, SMETER_WIDGET_X_OFFSET, SMETER_WIDGET_Y_OFFSET, SMETER_WIDGET_PADDING, currentTransceiverStatusPtr);
#endif
            this->volumeSquelchWidget = new LGFXVolumeSquelchWidget(display, VOLUME_SQUELCH_WIDGET_WIDTH, VOLUME_SQUELCH_WIDGET_HEIGHT, VOLUME_SQUELCH_WIDGET_X_OFFSET, VOLUME_SQUELCH_WIDGET_Y_OFFSET, VOLUME_SQUELCH_WIDGET_PADDING, currentTransceiverStatusPtr);
            this->filterWidget = new LGFXFilterWidget(display, FILTER_WIDGET_WIDTH, FILTER_WIDGET_HEIGHT, FILTER_WIDGET_X_OFFSET, FILTER_WIDGET_Y_OFFSET, FILTER_WIDGET_PADDING, currentTransceiverStatusPtr);
            this->bandWidget = new LGFXBandWidget(display, BAND_WIDGET_WIDTH, BAND_WIDGET_HEIGHT, BAND_WIDGET_X_OFFSET, BAND_WIDGET_Y_OFFSET, BAND_WIDGET_PADDING, currentTransceiverStatusPtr);
            this->menuWidget = new LGFXMenu(display, MENU_WIDGET_WIDTH, MENU_WIDGET_HEIGHT, MENU_WIDGET_X_OFFSET, MENU_WIDGET_Y_OFFSET, MENU_WIDGET_PADDING, menuPtr);

            this->parentDisplay->setTextSize(1);
            this->parentDisplay->setCursor(CONNECTED_SCREEN_BOTTOM_INFO_X_OFFSET, CONNECTED_SCREEN_BOTTOM_INFO_Y_OFFSET);
            this->parentDisplay->printf("SDR-REMOTE-CONTROL v%s (%s) - CAT speed: %d baud - FPS: ", SDR_REMOTE_CONTROL_CURRENT_VERSION, SDR_REMOTE_CONTROL_CURRENT_VERSION_DATE, SERIAL_DEFAULT_BAUD_RATE);
            */
            this->Refresh(true);
        }
    }
}

LGFXScreenConnected::~LGFXScreenConnected()
{
    delete this->primaryVFOWidget;
    this->primaryVFOWidget = nullptr;
    delete this->secondaryVFOWidget;
    this->secondaryVFOWidget = nullptr;
#ifdef USE_ANALOG_SMETER_WIDGET
    delete this->analogSMeterWidget;
    this->analogSMeterWidget = nullptr;
#else
    delete this->digitalSMeterWidget;
    this->digitalSMeterWidget = nullptr;
#endif
    delete this->volumeSquelchWidget;
    this->volumeSquelchWidget = nullptr;
    delete this->filterWidget;
    this->filterWidget = nullptr;
    delete this->bandWidget;
    this->bandWidget = nullptr;
    delete this->menuWidget;
    this->menuWidget = nullptr;
    this->parentDisplay = nullptr;
}

bool LGFXScreenConnected::Refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        bool changed = false;

        if (this->primaryVFOWidget->refresh(force))
        {
            changed = true;
        }
        if (this->secondaryVFOWidget->refresh(force))
        {
            changed = true;
        }
        /*
#ifdef USE_ANALOG_SMETER_WIDGET
        if (this->analogSMeterWidget->refresh(force))
        {
            changed = true;
        }
#else
        if (this->digitalSMeterWidget->refresh(force))
        {
            changed = true;
        }
#endif
        if (this->volumeSquelchWidget->refresh(force))
        {
            changed = true;
        }
        if (this->filterWidget->refresh(force))
        {
            changed = true;
        }
        if (this->bandWidget->refresh(force))
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
            char str[4];
            snprintf(str, sizeof(str), "%03u", currentFPS);
            this->parentDisplay->setTextSize(1);
            this->parentDisplay->drawString(str, CONNECTED_SCREEN_BOTTOM_INFO_LABEL_FPS_X_OFFSET, CONNECTED_SCREEN_BOTTOM_INFO_Y_OFFSET);
            changed = true;
        }
#endif // DEBUG_FPS
        */
        return (changed);
    }
    else
    {
        return (false);
    }
}
