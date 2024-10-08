#include <cstdint>

// this settings are copied from SDRRadio max BW modes

#define MAX_BW_MODE_AM 24000     // amplitude modulation => 24 kHz
#define MAX_BW_MODE_SAM 24000    // synchronous amplitude modulation => 24 kHz
#define MAX_BW_MODE_ECSS_L 24000 // exalted carrier single sideband (lower sideband) => 24 kHz
#define MAX_BW_MODE_ECSS_U 24000 // exalted carrier single sideband (upper sideband) => 24 kHz
#define MAX_BW_MODE_CW_U 4000    //  carrier wave (morse code), upper sideband => 4 kHz
#define MAX_BW_MODE_CW_L 4000    //  carrier wave (morse code), lower sideband => 4 kHz
#define MAX_BW_MODE_BC_FM 350000 //  => broadcast FM (mono, stereo, RDS), 50µs or 75µs de-emphasis => 350 kHz
#define MAX_BW_MODE_N_FM 16000   //  => narrow FM (voice), 5300µs de-emphasis, 300-3000 Hz bandpass filter => 16 kHz
#define MAX_BW_MODE_W_FM 48000   //  => wide FM (data), no de-emphasis, no high pass filtering => 48 kHz
#define MAX_BW_MODE_LSB 4000     //  lower sideband => 4 kHz
#define MAX_BW_MODE_USB 4000     //  upper sideband => 4 kHz
#define MAX_BW_MODE_WIDE_L 24000 //  lower sideband (wide mode) => 24 kHz
#define MAX_BW_MODE_WIDE_U 24000 //  upper sideband (wide mode) => 24 kHz
#define MAX_BW_MODE_DSB 8000     //  double sideband => 8 kHz

#define AM_FILTER_WIDTH_COUNT 10
#define CW_FILTER_WIDTH_COUNT 7
#define BFM_FILTER_WIDTH_COUNT 12
#define NFM_FILTER_WIDTH_COUNT 6
#define SSB_FILTER_WIDTH_COUNT 8
#define WSSB_FILTER_WIDTH_COUNT 13

class Bandwidth
{
private:
    /*
        ATTENTION: the low/high filter values specified below are the actual values with respect to the carrier. Since the CAT protocol does not support negative numbers, they may vary (SL = 0, SH = value * 2)

        VALUE1 is start of low pass filter viewed in relation to the carrier frequency
        VALUE2 is start of high pass filter viewed in relation to the carrier frequency
        VALUE3 is carrier center

        NOTE that carrier is centered on some modes (AM/FM) but on anothers USB/LSB/CW_U/CW_L the filter operates below/above the carrier frequency
    */

    // this is a "simetric filter" (carrier is centered)
    const uint16_t AM_FILTER_WIDTHS[AM_FILTER_WIDTH_COUNT][3] = {
        {-2500, 2500, 0},
        {-3000, 3000, 0},
        {-3500, 3500, 0},
        {-4000, 4000, 0},
        {-4500, 4500, 0},
        {-5000, 5000, 0},
        {-5500, 5500, 0},
        {-6000, 6000, 0},
        {-7000, 7000, 0},
        {-8000, 8000, 0},
    };

    // this is a "non simetric filter" (carrier is below/above depending mode CW_L/CW_U)
    const uint16_t CW_FILTER_WIDTHS[CW_FILTER_WIDTH_COUNT][3] = {
        {475, 525, 500},
        {450, 550, 500},
        {400, 600, 500},
        {300, 700, 500},
        {200, 800, 500},
        {100, 900, 500},
        {0, 1000, 500},
    };

    // this is a "simetric filter" (carrier is centered)
    const uint16_t BFM_FILTER_WIDTHS[BFM_FILTER_WIDTH_COUNT][3] = {
        {-50000, 50000, 0},
        {-62500, 62500, 0},
        {-75000, 75000, 0},
        {-87500, 87500, 0},
        {-96000, 96000, 0},
        {-100000, 100000, 0},
        {-112500, 112500, 0},
        {-125000, 125000, 0},
        {-137500, 137500, 0},
        {-150000, 150000, 0},
        {-162500, 162500, 0},
        {-175000, 175000, 0},
    };

    // this is a "simetric filter" (carrier is centered)
    const uint16_t NFM_FILTER_WIDTHS[NFM_FILTER_WIDTH_COUNT][3] = {
        {-4000, 4000, 0},
        {-5000, 5000, 0},
        {-6000, 6000, 0},
        {-12000, 12000, 0},
        {-15000, 15000, 0},
        {-20000, 20000, 0},
    };

    // this is a "non simetric filter" (carrier is below/above depending mode LSB/USB)
    const uint16_t SSB_FILTER_WIDTHS[SSB_FILTER_WIDTH_COUNT][3] = {
        {200, 2400},
        {200, 2600},
        {100, 2700},
        {100, 2900},
        {100, 3100},
        {100, 3300},
        {100, 3500},
        {100, 3700},
    };

    // this is a "non simetric filter" (carrier is below/above depending mode Wide-LSB/Wide-USB)
    const uint16_t WIDE_SSB_FILTER_WIDTHS[WSSB_FILTER_WIDTH_COUNT][3] = {
        {200, 2400},
        {200, 2600},
        {100, 2700},
        {100, 2900},
        {100, 3100},
        {100, 3300},
        {100, 3500},
        {100, 3700},
        {0, 5000},
        {0, 10000},
        {0, 15000},
        {0, 20000},
        {0, 24000},
    };
};
