
#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H

#define DEFAULT_FONT nullptr
/*
    NOTE:
    INTERNAL (PRIVATE) WIDGET CONSTANTS HAVE PREFIX "_"
*/

#define _DISPLAY_PADDING 4 // real draw starts/ends contained in this horizontal/vertical (pixel) padding

#define WIDGETS_VERTICAL_MARGIN 4

#define DEFAULT_FONT_SIZE_4_WIDTH 24
#define DEFAULT_FONT_SIZE_4_HEIGHT 28

/* SINGLE VFO WIDGET START */

#define _VFO_WIDGET_FONT_SIZE 3          // common font size for all VFO fields (index, freq, mode)
#define _VFO_WIDGET_FONT_PIXEL_HEIGHT 20 // total pixel height for current font size

// field offsets
#define _VFO_WIDGET_FREQUENCY_X_OFFSET 87                                  // frequency field x offset
#define _VFO_WIDGET_MODE_X_OFFSET 419                                      // mode field x offset
#define _VFO_WIDGET_FREQUENCY_STEP_X_OFFSET _VFO_WIDGET_FREQUENCY_X_OFFSET // frequency step field x offset
#define _VFO_WIDGET_FREQUENCY_STEP_Y_OFFSET (_VFO_WIDGET_FONT_PIXEL_HEIGHT + _VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN)

// frequency step field margins
#define _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN 2 // frequency step indicator horizontal margin
#define _VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN 4   // frequency step indicator vertical margin
#define _VFO_WIDGET_FREQUENCY_STEP_WIDTH 16            // vfo frequency step indicator width
#define _VFO_WIDGET_FREQUENCY_STEP_HEIGHT 3            // vfo frequency step indicator height

// other
#define _VFO_WIDGET_SINGLE_LINE_HEIGHT (_VFO_WIDGET_FONT_PIXEL_HEIGHT + _VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN + _VFO_WIDGET_FREQUENCY_STEP_HEIGHT + 2) // single VFO line (frequency step indicator included) height

// public widget constants
#define VFO_WIDGET_WIDTH (DISPLAY_WIDTH - (_DISPLAY_PADDING * 2))
#define VFO_WIDGET_HEIGHT _VFO_WIDGET_SINGLE_LINE_HEIGHT
#define VFO_WIDGET_X_OFFSET _DISPLAY_PADDING
#define VFO_PRIMARY_WIDGET_Y_OFFSET _DISPLAY_PADDING
#define VFO_SECONDARY_WIDGET_Y_OFFSET (VFO_PRIMARY_WIDGET_Y_OFFSET + VFO_WIDGET_HEIGHT + WIDGETS_VERTICAL_MARGIN)
#define VFO_WIDGET_PADDING 1

/* SINGLE VFO WIDGET END */

/* SEMETER COMMON BLOCK START */

#define _SMETER_WIDGET_SIGNAL_BASE_FONT_SIZE 4                                 // default label/value font size
#define _SMETER_WIDGET_SIGNAL_BASE_FONT_SIZE_WIDTH DEFAULT_FONT_SIZE_4_WIDTH   // total pixel width for current font size
#define _SMETER_WIDGET_SIGNAL_BASE_FONT_SIZE_HEIGHT DEFAULT_FONT_SIZE_4_HEIGHT // total pixel height for current font size
#define _SMETER_WIDGET_SIGNAL_EXPONENT_FONT_SIZE 2                             // default label value font size

// signal meter exponent value "xy" (Sn+xydB) sprite
#define _SMETER_WIDGET_SIGNAL_EXPONENT_SPRITE_WIDTH 60
#define _SMETER_WIDGET_SIGNAL_EXPONENT_SPRITE_HEIGHT 28

// field offsets

#define _SMETER_WIDGET_SIGNAL_VALUE_LABEL_Y_OFFSET SMETER_WIDGET_HEIGHT - _SMETER_WIDGET_SIGNAL_BASE_FONT_SIZE_HEIGHT - 4 // signal meter value label (Snn+xydB) y offset

// signal meter "S" (Sn) label x/y offsets
#define _SMETER_WIDGET_SIGNAL_BASE_LABEL_X_OFFSET _DIGITAL_SMETER_WIDGET_RIGHT_VERTICAL_LINE_X_OFFSET + 16
#define _SMETER_WIDGET_SIGNAL_BASE_LABEL_Y_OFFSET _SMETER_WIDGET_SIGNAL_VALUE_LABEL_Y_OFFSET

// signal meter "n" (Sn) value x/y offsets
#define _SMETER_WIDGET_SIGNAL_BASE_VALUE_BASE_X_OFFSET (_SMETER_WIDGET_SIGNAL_BASE_LABEL_X_OFFSET + _SMETER_WIDGET_SIGNAL_BASE_FONT_SIZE_WIDTH) // base number x offset
#define _SMETER_WIDGET_SIGNAL_BASE_VALUE_BASE_Y_OFFSET _SMETER_WIDGET_SIGNAL_VALUE_LABEL_Y_OFFSET                                               // label value y offset

// signal meter exponent value "xy" (Sn+xydB) x/y offsets
#define _SMETER_WIDGET_SIGNAL_EXPONENT_VALUE_BASE_X_OFFSET (_SMETER_WIDGET_SIGNAL_BASE_VALUE_BASE_X_OFFSET + _SMETER_WIDGET_SIGNAL_BASE_FONT_SIZE_WIDTH) // base number x offset
#define _SMETER_WIDGET_SIGNAL_EXPONENT_VALUE_BASE_Y_OFFSET _SMETER_WIDGET_SIGNAL_VALUE_LABEL_Y_OFFSET                                                    // label value y offset

// public widget constants
#define SMETER_WIDGET_WIDTH DISPLAY_WIDTH - (_DISPLAY_PADDING * 2)
#define SMETER_WIDGET_HEIGHT 57
#define SMETER_WIDGET_X_OFFSET _DISPLAY_PADDING
#define SMETER_WIDGET_Y_OFFSET (VFO_SECONDARY_WIDGET_Y_OFFSET + VFO_WIDGET_HEIGHT + WIDGETS_VERTICAL_MARGIN)
#define SMETER_WIDGET_PADDING 1

/* SEMETER COMMON BLOCK END */

/* DIGITAL SMETER BLOCK START */

#define _DIGITAL_SMETER_WIDGET_SUNITS_RANGE_FONT_SIZE 2

// bars
#define _DIGITAL_SMETER_WIDGET_BAR_COUNT 38                                                     // total smeter bars
#define _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN 4                                          // horizontal margin (separation) between bars
#define _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET (1 + _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN) // bars start x offset
#define _DIGITAL_SMETER_WIDGET_BAR_WIDTH 5                                                      // bar width
#define _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT 24                                                // low bar height
#define _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT (_DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT + 8)      // high bar height

// bars container lines (|_____________|)
#define _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_LENGTH 32      // vertical line length
#define _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_Y_OFFSET 22    // vertical lines y offset
#define _DIGITAL_SMETER_WIDGET_LEFT_VERTICAL_LINE_X_OFFSET 0 // left vertical line x offset
#define _DIGITAL_SMETER_BAR_WITH_MARGIN_WIDTH (_DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN + _DIGITAL_SMETER_WIDGET_BAR_WIDTH)
#define _DIGITAL_SMETER_WIDGET_RIGHT_VERTICAL_LINE_X_OFFSET (_DIGITAL_SMETER_WIDGET_LEFT_VERTICAL_LINE_X_OFFSET + _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN + (_DIGITAL_SMETER_WIDGET_BAR_COUNT * _DIGITAL_SMETER_BAR_WITH_MARGIN_WIDTH) + 1) // right vertical line y offset
#define _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_X_OFFSET _DIGITAL_SMETER_WIDGET_LEFT_VERTICAL_LINE_X_OFFSET                                                                                                                                       // horizontal line x offset
#define _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_Y_OFFSET (_DIGITAL_SMETER_WIDGET_VERTICAL_LINES_Y_OFFSET + _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_LENGTH)                                                                                          // horizontal line y offset
#define _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_LENGTH (_DIGITAL_SMETER_WIDGET_RIGHT_VERTICAL_LINE_X_OFFSET + 1)                                                                                                                                  // horizontal line length

// top value range-labels (numeric S units)
#define _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S1_X_OFFSET 6          // "S1" x offset
#define _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S3_X_OFFSET 46         // "S3" x offset
#define _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S5_X_OFFSET 83         // "S5" x offset
#define _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S7_X_OFFSET 119        // "S7" x offset
#define _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_X_OFFSET 154        // "S9" x offset
#define _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS15_X_OFFSET 185 // "S9+15" x offset
#define _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS30_X_OFFSET 230 // "S9+30" x offset
#define _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_S9_PLUS60_X_OFFSET 320 // "S9+60" x offset
#define _DIGITAL_SMETER_WIDGET_TOP_RANGE_LABELS_Y_OFFSET 0             // y offset

/* DIGITAL SMETER BLOCK END */

/* ANALOG SMETER BLOCK START */

#define _ANALOG_SMETER_WIDGET_BACKGROUND_WIDTH 350
#define _ANALOG_SMETER_WIDGET_BACKGROUND_HEIGHT ANALOG_SMETER_WIDGET_HEIGHT
#define _ANALOG_SMETER_WIDGET_CENTER_HLINE_X_OFFSET 4
#define _ANALOG_SMETER_WIDGET_CENTER_HLINE_Y_OFFSET ANALOG_SMETER_WIDGET_HEIGHT / 2
#define _ANALOG_SMETER_WIDGET_CENTER_HLINE_LENGTH _ANALOG_SMETER_WIDGET_BACKGROUND_WIDTH - 8
#define _ANALOG_SMETER_WIDGET_CENTER_NUMBERS_X_OFFSET 10

#define _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_OFFSET (ANALOG_SMETER_WIDGET_HEIGHT / 2) + 8
#define _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_Y_HEIGHT 20
#define _ANALOG_SMETER_WIDGET_CENTER_METER_VLINE_WIDTH 3

// public widget constants
#define ANALOG_SMETER_WIDGET_WIDTH DISPLAY_WIDTH - (_DISPLAY_PADDING * 2)
#define ANALOG_SMETER_WIDGET_HEIGHT 57
#define ANALOG_SMETER_WIDGET_X_OFFSET _DISPLAY_PADDING
#define ANALOG_SMETER_WIDGET_Y_OFFSET VFO_SECONDARY_WIDGET_Y_OFFSET + VFO_WIDGET_HEIGHT + WIDGETS_VERTICAL_MARGIN
#define ANALOG_SMETER_WIDGET_PADDING 1

/* ANALOG SMETER BLOCK END */

/* VOLUME & SQUELCH START */

#define _VOLUME_SQUELCH_WIDGET_FONT_SIZE 2

// volume / squelch labels

#define _VOLUME_SQUELCH_WIDGET_LABEL_SQUELCH_X_OFFSET 240

#define _VOLUME_SQUELCH_WIDGET_LABEL_VOLUME_VALUE_X_OFFSET 184
#define _VOLUME_SQUELCH_WIDGET_LABEL_SQUELCH_VALUE_X_OFFSET 436

#define _VOLUME_SQUELCH_WIDGET_BARS_WIDTH 100 + 2
#define _VOLUME_SQUELCH_WIDGET_BARS_HEIGHT VOLUME_SQUELCH_WIDGET_HEIGHT - 2

#define _VOLUME_SQUELCH_WIDGET_VOLUME_BAR_X_OFFSET 76
#define _VOLUME_SQUELCH_WIDGET_SQUELCH_BAR_X_OFFSET 328

#define VOLUME_SQUELCH_WIDGET_WIDTH DISPLAY_WIDTH - (_DISPLAY_PADDING * 2)
#define VOLUME_SQUELCH_WIDGET_HEIGHT 16
#define VOLUME_SQUELCH_WIDGET_X_OFFSET _DISPLAY_PADDING
#define VOLUME_SQUELCH_WIDGET_Y_OFFSET SMETER_WIDGET_Y_OFFSET + SMETER_WIDGET_HEIGHT + WIDGETS_VERTICAL_MARGIN
#define VOLUME_SQUELCH_WIDGET_PADDING 1

/* VOLUME & SQUELCH END */

/* FILTER START */

#define _FILTER_WIDGET_TRAPEZIUM_SPRITE_WIDTH 180                                         // trapezium sprite width
#define _FILTER_WIDGET_TRAPEZIUM_TOP_LINE_MAX_LENGTH 120                                  // trapezium top horizontal line max length
#define _FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_LENGTH _FILTER_WIDGET_TRAPEZIUM_SPRITE_WIDTH // trapezium bottom horizontal line length
#define _FILTER_WIDGET_TRAPEZIUM_TOP_LINE_Y_OFFSET 8                                      // trapezium top horizontal line y offset
#define _FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_Y_OFFSET 45                                  // trapezium bottom horizontal line y offset

#define _FILTER_WIDGET_LABELS_SPRITE_WIDTH 150 // labels sprite width

#define _FILTER_WIDGET_TRAPEZIUM_LR_LINES_HORIZONTAL_OFFSET (_FILTER_WIDGET_TRAPEZIUM_BOTTOM_LINE_LENGTH - _FILTER_WIDGET_TRAPEZIUM_TOP_LINE_MAX_LENGTH) / 4 // trapecium x offsets between left/right lines on top/bottom horizontal lines

#define _FILTER_WIDGET_TRAPEZIUM_CENTER_V_LINE_LENGTH 44 // trapecium "center" (low/hi cut) separator vertical line length

#define _FILTER_WIDGET_FONT_SIZE 2

#define _FILTER_WIDGET_LEFT_LABELS_X_OFFSET (DISPLAY_WIDTH / 2) - 40
#define _FILTER_WIDGET_LABELS_FONT_PIXEL_HEIGHT 17 // total pixel height for current font size

#define FILTER_WIDGET_WIDTH DISPLAY_WIDTH - (_DISPLAY_PADDING * 2)
#define FILTER_WIDGET_HEIGHT 50
#define FILTER_WIDGET_X_OFFSET _DISPLAY_PADDING
#define FILTER_WIDGET_Y_OFFSET VOLUME_SQUELCH_WIDGET_Y_OFFSET + VOLUME_SQUELCH_WIDGET_HEIGHT + WIDGETS_VERTICAL_MARGIN
#define FILTER_WIDGET_PADDING 1

/* FILTER END */

/* BAND START */

#define _BAND_WIDGET_MAIN_LABEL_FONT_SIZE 3
#define _BAND_WIDGET_SUB_LABEL_FONT_SIZE 2
#define _BAND_WIDGET_BACKGROUND_RECT_RADIUS 5

#define _BAND_WIDGET_MAIN_LABEL_Y_OFFSET 6
#define _BAND_WIDGET_SUB_LABEL_Y_OFFSET 30

#define BAND_WIDGET_WIDTH 114
#define BAND_WIDGET_HEIGHT 50
#define BAND_WIDGET_X_OFFSET DISPLAY_WIDTH - (_DISPLAY_PADDING * 2) - BAND_WIDGET_WIDTH
#define BAND_WIDGET_Y_OFFSET VOLUME_SQUELCH_WIDGET_Y_OFFSET + VOLUME_SQUELCH_WIDGET_HEIGHT + WIDGETS_VERTICAL_MARGIN
#define BAND_WIDGET_PADDING 1

/* BAND END */

/* MENU BUTTON START */

#define _MENU_BUTTON_WIDGET_HORIZONTAL_MARGIN 4
#define _MENU_BUTTON_WIDGET_VERTICAL_MARGIN 4

// button border radius
#define _MENU_BUTTON_WIDGET_ROUND_BORDER_RADIUS 5

// TWO DIGITS TOP LABEL (Fx)
#define _MENU_BUTTON_WIDGET_TOP_LABEL_Y_OFFSET 5

// MAIN LABEL (Fn DESCRIPTION)
#define _MENU_BUTTON_WIDGET_MAIN_LABEL_Y_OFFSET 26

#define MENU_BUTTON_WIDGET_WIDTH 114
#define MENU_BUTTON_WIDGET_HEIGHT 48
#define MENU_BUTTON_WIDGET_X_OFFSET _DISPLAY_PADDING
#define MENU_BUTTON_WIDGET_Y_OFFSET DISPLAY_HEIGHT - MENU_WIDGET_HEIGHT
#define MENU_BUTTON_WIDGET_PADDING 1

/* MENU BUTTON END */

/* MENU START */

#define MENU_WIDGET_WIDTH DISPLAY_WIDTH - (_DISPLAY_PADDING * 2)
#define MENU_WIDGET_HEIGHT 104
#define MENU_WIDGET_X_OFFSET _DISPLAY_PADDING
#define MENU_WIDGET_Y_OFFSET FILTER_WIDGET_Y_OFFSET + FILTER_WIDGET_HEIGHT + WIDGETS_VERTICAL_MARGIN
#define MENU_WIDGET_PADDING 1

/* MENU END */

/* CONNECTED SCREEN BOTTOM INFO BEGIN */

#define CONNECTED_SCREEN_BOTTOM_INFO_X_OFFSET 6
#define CONNECTED_SCREEN_BOTTOM_INFO_Y_OFFSET 313
#define CONNECTED_SCREEN_BOTTOM_INFO_LABEL_FPS_X_OFFSET 420

/* CONNECTED SCREEN BOTTOM INFO END */

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H
