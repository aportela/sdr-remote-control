#include "LGFXScreenConnectedILI9488.hpp"
#include "../../DisplayConfiguration.hpp"

#ifdef DEBUG_FPS
#include "../../utils/FPS.hpp"
#endif

#define WIDGETS_VERTICAL_MARGIN 8

/* VFO BLOCK START */

#define VFO_WIDGET_WIDTH 464                                                                                                                                    // widget width (without padding)
#define VFO_WIDGET_HEIGHT 72                                                                                                                                    // widget height (without padding)
#define VFO_WIDGET_PADDING 8                                                                                                                                    // real draw starts at this padding
#define VFO_WIDGET_FONT_SIZE 3                                                                                                                                  // font size (setTextSize())
#define VFO_WIDGET_FONT_WIDTH 20                                                                                                                                // font size (total pixel width)
#define VFO_WIDGET_FONT_HEIGHT 20                                                                                                                               // font size (total pixel height)
#define VFO_WIDGET_START_X_COORDINATE VFO_WIDGET_PADDING                                                                                                        // draw (x) starts here
#define VFO_WIDGET_START_Y_COORDINATE VFO_WIDGET_PADDING                                                                                                        // draw (y) starts here
#define VFO_WIDGET_STEP_HORIZONTAL_MARGIN 2                                                                                                                     // frequency step indicator top/bottom margin
#define VFO_WIDGET_STEP_VERTICAL_MARGIN 4                                                                                                                       // frequency step indicator top/bottom margin
#define VFO_WIDGET_STEP_WIDTH 16                                                                                                                                // frequency step indicator width
#define VFO_WIDGET_STEP_HEIGHT 3                                                                                                                                // frequency step indicator height
#define VFO_WIDGET_SINGLE_VFO_LINE_HEIGHT (VFO_WIDGET_FONT_HEIGHT + VFO_WIDGET_STEP_VERTICAL_MARGIN + VFO_WIDGET_STEP_HEIGHT + VFO_WIDGET_STEP_VERTICAL_MARGIN) // single VFO line (frequency step indicator included) height

// first VFO x/y coordinates
#define VFO_WIDGET_PRIMARY_START_X_COORDINATE VFO_WIDGET_START_X_COORDINATE
#define VFO_WIDGET_PRIMARY_START_Y_COORDINATE VFO_WIDGET_START_Y_COORDINATE
// first VFO freq step indicator x/y coordinates
#define VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE 90
#define VFO_WIDGET_PRIMARY_STEP_START_Y_COORDINATE VFO_WIDGET_PRIMARY_START_Y_COORDINATE + VFO_WIDGET_FONT_HEIGHT + VFO_WIDGET_STEP_VERTICAL_MARGIN
// secondary VFO x/y coordinates
#define VFO_WIDGET_SECONDARY_START_X_COORDINATE VFO_WIDGET_START_X_COORDINATE
#define VFO_WIDGET_SECONDARY_START_Y_COORDINATE VFO_WIDGET_START_Y_COORDINATE + VFO_WIDGET_SINGLE_VFO_LINE_HEIGHT
// secondary VFO freq step indicator x/y coordinates
#define VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE 90
#define VFO_WIDGET_SECONDARY_STEP_START_Y_COORDINATE VFO_WIDGET_SECONDARY_START_Y_COORDINATE + VFO_WIDGET_FONT_HEIGHT + VFO_WIDGET_STEP_VERTICAL_MARGIN

/* VFO BLOCK END */

/* SMETER BLOCK START */

#define SMETER_WIDGET_WIDTH 240  // widget width (without padding)
#define SMETER_WIDGET_HEIGHT 120 // widget height (without padding)

// first VFO x/y coordinates
#define SMETER_WIDGET_PRIMARY_START_X_COORDINATE 0
#define SMETER_WIDGET_PRIMARY_START_Y_COORDINATE 72

/* SMETER BLOCK END */

#define SMETER_PARTS 42
#define SMETER_PART_WIDTH 3
#define SMETER_PART_SPACE_WIDTH 2
#define SMETER_PARTH_HEIGHT 18
#define SMETER_PARTH_HEIGHT_SEPARATOR 20
#define SMETER_PARTH_BG_COLOR 0x8410

#define VFO_BLOCK_START_X_COORDINATE 8
#define VFO_PRIMARY_BLOCK_START_Y_COORDINATE 8
#define VFO_SECONDARY_BLOCK_START_Y_COORDINATE 44

#define COLOR_BG TFT_BLACK
#define COLOR_NOT_ACTIVE 0x18C3
#define COLOR_ACTIVE 0xFFFF
#define COLOR_SECONDARY 0x528A
#define COLOR_WARNING TFT_RED

using namespace aportela::microcontroller::utils;

LGFXScreenConnectedILI9488::LGFXScreenConnectedILI9488(LovyanGFX *display, Transceiver *trx) : LGFXScreen(display), trx(trx)
{
    if (display != nullptr)
    {
        this->Refresh(true);
    }
}

LGFXScreenConnectedILI9488::~LGFXScreenConnectedILI9488()
{
    this->parentDisplay = nullptr;
}

void LGFXScreenConnectedILI9488::refreshVFOIndex(uint8_t number, bool isActive)
{
    this->parentDisplay->setTextColor(isActive ? COLOR_ACTIVE : COLOR_SECONDARY, COLOR_BG);
    if (number == 0)
    {
        this->parentDisplay->setCursor(VFO_WIDGET_PRIMARY_START_X_COORDINATE, VFO_WIDGET_PRIMARY_START_Y_COORDINATE);
    }
    else
    {
        this->parentDisplay->setCursor(VFO_WIDGET_SECONDARY_START_X_COORDINATE, VFO_WIDGET_SECONDARY_START_Y_COORDINATE);
    }
    this->parentDisplay->setTextSize(VFO_WIDGET_FONT_SIZE);
    this->parentDisplay->print(number == 0 ? "VFOA" : "VFOB");
}

void LGFXScreenConnectedILI9488::refreshVFOFreq(uint8_t number, bool isActive, uint64_t frequency)
{
    char formattedFrequency[16];
    char nformattedFrequency[12];
    // test if this method is more optimized than div operators
    uint8_t resultIndex = 0;
    sprintf(nformattedFrequency, "%012llu", frequency);
    for (uint8_t i = 0; i < 12; ++i)
    {
        formattedFrequency[resultIndex++] = nformattedFrequency[i];
        if ((i + 1) % 3 == 0 && i < 11)
        {
            formattedFrequency[resultIndex++] = '.';
        }
    }
    formattedFrequency[resultIndex] = '\0';
    this->parentDisplay->setCursor(90, number == 0 ? VFO_WIDGET_PRIMARY_START_Y_COORDINATE : VFO_WIDGET_SECONDARY_START_Y_COORDINATE);
    this->parentDisplay->setTextSize(VFO_WIDGET_FONT_SIZE);
    if (isActive)
    {
        uint8_t firstNonZeroIndex = 0;
        while ((formattedFrequency[firstNonZeroIndex] == '0' || formattedFrequency[firstNonZeroIndex] == '.') && firstNonZeroIndex < 12)
        {
            firstNonZeroIndex++;
        }
        this->parentDisplay->setTextColor(COLOR_NOT_ACTIVE, TFT_BLACK);
        for (uint8_t i = 0; i < firstNonZeroIndex; i++)
        {
            this->parentDisplay->print(formattedFrequency[i]);
        }
        this->parentDisplay->setTextColor(COLOR_ACTIVE, TFT_BLACK);
        this->parentDisplay->print(&formattedFrequency[firstNonZeroIndex]);
    }
    else
    {
        this->parentDisplay->setTextColor(COLOR_SECONDARY, TFT_BLACK);
        this->parentDisplay->print(formattedFrequency);
    }
    if (frequency >= 1000000000)
    {
        this->parentDisplay->print("GHz");
    }
    else if (frequency >= 1000000)
    {
        this->parentDisplay->print("Mhz");
    }
    else if (frequency >= 1000)
    {
        this->parentDisplay->print("Khz");
    }
    else
    {
        this->parentDisplay->print("Hz ");
    }
}

void LGFXScreenConnectedILI9488::refreshVFOMode(uint8_t number, bool isActive, TRXVFOMode mode)
{
    this->parentDisplay->setTextColor(isActive ? TFT_CYAN : COLOR_SECONDARY, TFT_BLACK);
    this->parentDisplay->setCursor(420, number == 0 ? VFO_WIDGET_PRIMARY_START_Y_COORDINATE : VFO_WIDGET_SECONDARY_START_Y_COORDINATE);
    this->parentDisplay->setTextSize(VFO_WIDGET_FONT_SIZE);
    switch (mode)
    {
    case TRX_VFO_MD_DSB:
        this->parentDisplay->print("DSB");
        break;
    case TRX_VFO_MD_LSB:
        this->parentDisplay->print("LSB");
        break;
    case TRX_VFO_MD_USB:
        this->parentDisplay->print("USB");
        break;
    case TRX_VFO_MD_CW_U:
        this->parentDisplay->print("CWU"); // CW (upper sideband)
        break;
    case TRX_VFO_MD_FM:
        this->parentDisplay->print("FM ");
        break;
    case TRX_VFO_MD_SAM:
        this->parentDisplay->print("SAM"); // (synchronous AM, includes ECSS)
        break;
    case TRX_VFO_MD_RESERVED:
        this->parentDisplay->print("  "); // reserved / not used
        break;
    case TRX_VFO_MD_CW_L:
        this->parentDisplay->print("CWL"); // CW (lower sideband)
        break;
    case TRX_VFO_MD_WFM:
        this->parentDisplay->print("WFM");
        break;
    case TRX_VFO_MD_BFM:
        this->parentDisplay->print("BFM");
        break;
    default:
        this->parentDisplay->print("???"); // error / invalid value
        break;
    }
}

void LGFXScreenConnectedILI9488::refreshVFOStep(uint8_t number, bool isActive, uint64_t step)
{
    uint16_t x = 0;
    uint16_t y = 0;
    // clear old step
    if (number == 0)
    {
        x = VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE;
        y = VFO_WIDGET_PRIMARY_STEP_START_Y_COORDINATE;
    }
    else
    {
        x = VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE;
        y = VFO_WIDGET_SECONDARY_STEP_START_Y_COORDINATE;
    }
    this->parentDisplay->fillRect(x, y, 15 * (VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN), VFO_WIDGET_STEP_HEIGHT, TFT_BLACK);
    if (step > 0)
    {
        switch (step)
        {
        case 1: // 1 hz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 14);
            break;
        case 10: // 10 hz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 13);
            break;
        case 100: // 100 hz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 12);
            break;
        case 1000: // 1 Khz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 10);
            break;
        case 10000: // 10 Khz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 9);
            break;
        case 100000: // 100 Khz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 8);
            break;
        case 1000000: // 1 Mhz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 6);
            break;
        case 10000000: // 10 Mhz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 5);
            break;
        case 100000000: // 100 Mhz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 4);
            break;
        case 1000000000: // 1 Ghz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + ((VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN) * 2);
            break;
        case 10000000000: // 10 Ghz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE) + (VFO_WIDGET_STEP_WIDTH + VFO_WIDGET_STEP_HORIZONTAL_MARGIN);
            break;
        case 100000000000: // 100 Ghz
            x = (number == 0 ? VFO_WIDGET_PRIMARY_STEP_START_X_COORDINATE : VFO_WIDGET_SECONDARY_STEP_START_X_COORDINATE);
            break;
        default:
            x = 0;
            break;
        }
        this->parentDisplay->fillRect(x, y, VFO_WIDGET_STEP_WIDTH, VFO_WIDGET_STEP_HEIGHT, isActive ? COLOR_ACTIVE : COLOR_SECONDARY);
    }
}

bool LGFXScreenConnectedILI9488::Refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        bool changed = this->trx->changed > 0;

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
        if (force || (this->trx->changed & TRX_CFLAG_SIGNAL_METER_LEVEL))
        {
            // this->smeterDigitalPtr->refresh(this->trx->signalMeterLevel);
            this->trx->changed &= ~TRX_CFLAG_SIGNAL_METER_LEVEL;
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
            this->parentDisplay->setTextColor(0xF85E, COLOR_BG);
            this->parentDisplay->setCursor(VFO_BLOCK_START_X_COORDINATE, DISPLAY_HEIGHT - 16);
            this->parentDisplay->setTextSize(1);
            this->parentDisplay->printf("%03u FPS", currentFPS);
            changed = true;
        }
#endif
        // this->menuPtr->refresh(false);
        return (changed);
    }
    else
    {
        return (false);
    }
}
