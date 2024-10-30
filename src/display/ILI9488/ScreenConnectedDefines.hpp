
#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H

/*
    NOTE:
    INTERNAL (PRIVATE) WIDGET CONSTANTS HAVE PREFIX "_"
*/

#define _DISPLAY_PADDING 4 // real draw starts/ends contained in this horizontal/vertical (pixel) padding

#define WIDGETS_VERTICAL_MARGIN 4

/* SINGLE VFO WIDGET START */

#define _VFO_WIDGET_FONT_SIZE 3          // common font size for all VFO fields (index, freq, mode)
#define _VFO_WIDGET_FONT_PIXEL_HEIGHT 20 // total pixel height for current font size

// field offsets
#define _VFO_WIDGET_FREQUENCY_X_OFFSET 87                                  // frequency field x offset
#define _VFO_WIDGET_MODE_X_OFFSET 419                                      // mode field x offset
#define _VFO_WIDGET_FREQUENCY_STEP_X_OFFSET _VFO_WIDGET_FREQUENCY_X_OFFSET // frequency step field x offset
#define _VFO_WIDGET_FREQUENCY_STEP_Y_OFFSET _VFO_WIDGET_FONT_PIXEL_HEIGHT + _VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN

// frequency step field margins
#define _VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN 2 // frequency step indicator horizontal margin
#define _VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN 4   // frequency step indicator vertical margin
#define _VFO_WIDGET_FREQUENCY_STEP_WIDTH 16            // vfo frequency step indicator width
#define _VFO_WIDGET_FREQUENCY_STEP_HEIGHT 3            // vfo frequency step indicator height

// other
#define _VFO_WIDGET_SINGLE_LINE_HEIGHT (_VFO_WIDGET_FONT_PIXEL_HEIGHT + _VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN + _VFO_WIDGET_FREQUENCY_STEP_HEIGHT + _VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN) // single VFO line (frequency step indicator included) height

// public widget constants
#define VFO_WIDGET_WIDTH DISPLAY_WIDTH - (_DISPLAY_PADDING * 2)
#define VFO_WIDGET_HEIGHT _VFO_WIDGET_SINGLE_LINE_HEIGHT
#define VFO_WIDGET_X_OFFSET _DISPLAY_PADDING
#define VFO_PRIMARY_WIDGET_Y_OFFSET _DISPLAY_PADDING
#define VFO_SECONDARY_WIDGET_Y_OFFSET VFO_WIDGET_HEIGHT + WIDGETS_VERTICAL_MARGIN
#define VFO_WIDGET_PADDING 1

/* SINGLE VFO WIDGET END */

/* SMETER BLOCK START */

#define _DIGITAL_SMETER_WIDGET_FONT_SIZE 2

// bars
#define _DIGITAL_SMETER_WIDGET_BAR_COUNT 38                                                    // total SMeter bars
#define _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN 4                                         // horizontal margin (separation) between bars
#define _DIGITAL_SMETER_WIDGET_BARS_X_OFFSET _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN + 1  // bars start x offset
#define _DIGITAL_SMETER_WIDGET_BAR_WIDTH 5                                                     // bar width
#define _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT 24                                               // low bar height
#define _DIGITAL_SMETER_WIDGET_HIGH_BAR_HEIGHT _DIGITAL_SMETER_WIDGET_LOW_BAR_HEIGHT + 4       // high bar height
#define _DIGITAL_SMETER_WIDGET_HIGH_BARS_Y_OFFSET 94                                           // high bar y offset
#define _DIGITAL_SMETER_WIDGET_LOW_BARS_Y_OFFSET _DIGITAL_SMETER_WIDGET_HIGH_BARS_Y_OFFSET + 4 // low bar y offset

// smeter container lines
#define _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_LENGTH 32                                                                                                                                                                               // vertical line length
#define _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_Y_OFFSET 22                                                                                                                                                                             // vertical lines y offset
#define _DIGITAL_SMETER_WIDGET_LEFT_VERTICAL_LINE_X_OFFSET 0                                                                                                                                                                          // left vertical line x offset
#define _DIGITAL_SMETER_WIDGET_RIGHT_VERTICAL_LINE_X_OFFSET 1 + _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN + (_DIGITAL_SMETER_WIDGET_BAR_COUNT * (_DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN + _DIGITAL_SMETER_WIDGET_BAR_WIDTH)) // right vertical line y offset
#define _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_X_OFFSET _DIGITAL_SMETER_WIDGET_LEFT_VERTICAL_LINE_X_OFFSET                                                                                                                            // horizontal line x offset
#define _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_Y_OFFSET _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_Y_OFFSET + _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_LENGTH                                                                                 // horizontal line y offset
#define _DIGITAL_SMETER_WIDGET_HORIZONTAL_LINE_LENGTH _DIGITAL_SMETER_WIDGET_RIGHT_VERTICAL_LINE_X_OFFSET                                                                                                                             // horizontal line length

// top value labels (numeric S units)
#define _DIGITAL_SMETER_WIDGET_TOP_LABELS_X_OFFSET 10 // x offset
#define _DIGITAL_SMETER_WIDGET_TOP_LABELS_Y_OFFSET 0  // y offset

// SMeter right label value (ex: S9+30dB)
#define _DIGITAL_SMETER_WIDGET_S_LABEL_FONT_SIZE 4                                                                                                           // default label value font size
#define _DIGITAL_SMETER_WIDGET_S_LABEL_FONT_SIZE_WIDTH 24                                                                                                    // total pixel width for current font size
#define _DIGITAL_SMETER_WIDGET_S_LABEL_X_OFFSET _DIGITAL_SMETER_WIDGET_RIGHT_VERTICAL_LINE_X_OFFSET + 16                                                     // "S" x offset
#define _DIGITAL_SMETER_WIDGET_S_LABEL_BASE_NUMBER_X_OFFSET _DIGITAL_SMETER_WIDGET_S_LABEL_X_OFFSET + _DIGITAL_SMETER_WIDGET_S_LABEL_FONT_SIZE_WIDTH         // base number x offset
#define _DIGITAL_SMETER_WIDGET_S_LABEL_BASE_DB_X_OFFSET _DIGITAL_SMETER_WIDGET_S_LABEL_BASE_NUMBER_X_OFFSET + _DIGITAL_SMETER_WIDGET_S_LABEL_FONT_SIZE_WIDTH // dB (base) label (widthout exp) x offset
#define _DIGITAL_SMETER_WIDGET_S_LABEL_EXP_X_OFFSET _DIGITAL_SMETER_WIDGET_S_LABEL_BASE_NUMBER_X_OFFSET + _DIGITAL_SMETER_WIDGET_S_LABEL_FONT_SIZE_WIDTH     // dB (exp) label x offset
#define _DIGITAL_SMETER_WIDGET_S_LABEL_Y_OFFSET _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_Y_OFFSET                                                               // label value y offset
#define _DIGITAL_SMETER_WIDGET_S_SUB_LABEL_FONT_SIZE 2                                                                                                       // +xxdB exp font size

// public widget constants
#define DIGITAL_SMETER_WIDGET_WIDTH DISPLAY_WIDTH - (_DISPLAY_PADDING * 2)
#define DIGITAL_SMETER_WIDGET_HEIGHT 57
#define DIGITAL_SMETER_WIDGET_X_OFFSET _DISPLAY_PADDING
#define DIGITAL_SMETER_WIDGET_Y_OFFSET VFO_SECONDARY_WIDGET_Y_OFFSET + VFO_WIDGET_HEIGHT + WIDGETS_VERTICAL_MARGIN
#define DIGITAL_SMETER_WIDGET_PADDING 1

/* SMETER BLOCK END */

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
#define VOLUME_SQUELCH_WIDGET_Y_OFFSET DIGITAL_SMETER_WIDGET_Y_OFFSET + DIGITAL_SMETER_WIDGET_HEIGHT + WIDGETS_VERTICAL_MARGIN
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
