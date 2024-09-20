
#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H

/*
    NOTE:
    INTERNAL (PRIVATE) WIDGET CONSTANTS HAVE PREFIX "_"
*/

#define _DISPLAY_PADDING 4 // real draw starts/ends contained in this horizontal/vertical (pixel) padding

#define WIDGETS_VERTICAL_MARGIN 4

/* DUAL VFO WIDGET START */

#define _DUAL_VFO_WIDGET_FONT_SIZE 3          // common font size for all VFO fields (index, freq, mode)
#define _DUAL_VFO_WIDGET_FONT_PIXEL_HEIGHT 20 // total pixel height for current font size

// frequency step field margins
#define _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN 2 // frequency step indicator horizontal margin
#define _DUAL_VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN 4   // frequency step indicator vertical margin
#define _DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH 16            // vfo frequency step indicator width
#define _DUAL_VFO_WIDGET_FREQUENCY_STEP_HEIGHT 3            // vfo frequency step indicator height

// field offsets
#define _DUAL_VFO_WIDGET_FREQUENCY_X_OFFSET 87                                       // frequency field x offset
#define _DUAL_VFO_WIDGET_MODE_X_OFFSET 419                                           // mode field x offset
#define _DUAL_VFO_WIDGET_FREQUENCY_STEP_X_OFFSET _DUAL_VFO_WIDGET_FREQUENCY_X_OFFSET // frequency step field x offset
#define _DUAL_VFO_WIDGET_PRIMARY_FREQUENCY_STEP_Y_OFFSET _DUAL_VFO_WIDGET_FONT_PIXEL_HEIGHT + _DUAL_VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN
#define _DUAL_VFO_WIDGET_SECONDARY_FREQUENCY_STEP_Y_OFFSET _DUAL_VFO_WIDGET_SINGLE_LINE_HEIGHT + _DUAL_VFO_WIDGET_FONT_PIXEL_HEIGHT + _DUAL_VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN

// other
#define _DUAL_VFO_WIDGET_SINGLE_LINE_HEIGHT (_DUAL_VFO_WIDGET_FONT_PIXEL_HEIGHT + _DUAL_VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HEIGHT + _DUAL_VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN) // single VFO line (frequency step indicator included) height

// public widget constants
#define DUAL_VFO_WIDGET_WIDTH DISPLAY_WIDTH - (_DISPLAY_PADDING * 2)
#define DUAL_VFO_WIDGET_HEIGHT _DUAL_VFO_WIDGET_SINGLE_LINE_HEIGHT * 2
#define DUAL_VFO_WIDGET_X_OFFSET _DISPLAY_PADDING
#define DUAL_VFO_WIDGET_Y_OFFSET _DISPLAY_PADDING
#define DUAL_VFO_WIDGET_PADDING 1
#define DUAL_VFO_WIDGET_SECONDARY_VFO_Y_OFFSET _DUAL_VFO_WIDGET_SINGLE_LINE_HEIGHT

/* DUAL VFO WIDGET END */

/* SMETER BLOCK START */

#define _DIGITAL_SMETER_FONT_SIZE 2

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
#define _DIGITAL_SMETER_WIDGET_S_LABEL_FONT_SIZE 4
#define _DIGITAL_SMETER_WIDGET_S_LABEL_X_OFFSET _DIGITAL_SMETER_WIDGET_RIGHT_VERTICAL_LINE_X_OFFSET + 16
#define _DIGITAL_SMETER_WIDGET_S_LABEL_Y_OFFSET _DIGITAL_SMETER_WIDGET_VERTICAL_LINES_Y_OFFSET
#define _DIGITAL_SMETER_WIDGET_S_SUB_LABEL_FONT_SIZE 3

// public widget constants
#define DIGITAL_SMETER_WIDGET_WIDTH DISPLAY_WIDTH - (_DISPLAY_PADDING * 2)
#define DIGITAL_SMETER_WIDGET_HEIGHT 57
#define DIGITAL_SMETER_WIDGET_X_OFFSET _DISPLAY_PADDING
#define DIGITAL_SMETER_WIDGET_Y_OFFSET _DISPLAY_PADDING + DUAL_VFO_WIDGET_HEIGHT + WIDGETS_VERTICAL_MARGIN
#define DIGITAL_SMETER_WIDGET_PADDING 1

/* SMETER BLOCK END */

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H
