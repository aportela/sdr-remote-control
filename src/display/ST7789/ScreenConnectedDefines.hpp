
#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H

#define _DISPLAY_PADDING 4 // real draw starts at this padding from (0,0)

/* VFO BLOCK START */

#define _DUAL_VFO_WIDGET_FONT_SIZE 2           // common font size for all VFO fields (index, freq, mode)
#define _DUAL_VFO_WIDGET_FONT_PIXEL_HEIGHT 16  // total pixel height for current font size
#define VFO_WIDGETS_X_OFFSET _DISPLAY_PADDING  // VFO's x offset
#define VFO_WIDGETS_Y_OFFSET _DISPLAY_PADDING  // VFO's y offset
#define _DUAL_VFO_WIDGET_FREQUENCY_X_OFFSET 60 // VFO's frequency x offset
#define _DUAL_VFO_WIDGET_MODE_X_OFFSET 280     // VFO's mode x offset

#define _DUAL_VFO_WIDGET_FREQUENCY_STEP_HORIZONTAL_MARGIN 2                          // frequency step indicator horizontal margin
#define _DUAL_VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN 4                            // frequency step indicator vertical margin
#define _DUAL_VFO_WIDGET_FREQUENCY_STEP_WIDTH 12                                     // vfo frequency step indicator width
#define _DUAL_VFO_WIDGET_FREQUENCY_STEP_HEIGHT 2                                     // vfo frequency step indicator height
#define _DUAL_VFO_WIDGET_FREQUENCY_STEP_X_OFFSET _DUAL_VFO_WIDGET_FREQUENCY_X_OFFSET // first vfo step frecuency x offset

#define _DUAL_VFO_WIDGET_SINGLE_LINE_HEIGHT (_DUAL_VFO_WIDGET_FONT_PIXEL_HEIGHT + _DUAL_VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN + _DUAL_VFO_WIDGET_FREQUENCY_STEP_HEIGHT + _DUAL_VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN) // single VFO line (frequency step indicator included) height

// primary VFO offsets
#define PRIMARY_VFO_WIDGET_X_OFFSET VFO_WIDGETS_X_OFFSET
#define PRIMARY_VFO_WIDGET_Y_OFFSET VFO_WIDGETS_Y_OFFSET
#define _DUAL_VFO_WIDGET_PRIMARY_FREQUENCY_STEP_Y_OFFSET PRIMARY_VFO_WIDGET_Y_OFFSET + _DUAL_VFO_WIDGET_FONT_PIXEL_HEIGHT + _DUAL_VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN

// secondary VFO offsets
#define SECONDARY_VFO_WIDGET_X_OFFSET VFO_WIDGETS_X_OFFSET
#define SECONDARY_VFO_WIDGET_Y_OFFSET VFO_WIDGETS_Y_OFFSET + _DUAL_VFO_WIDGET_SINGLE_LINE_HEIGHT
#define _DUAL_VFO_WIDGET_SECONDARY_FREQUENCY_STEP_Y_OFFSET SECONDARY_VFO_WIDGET_Y_OFFSET + _DUAL_VFO_WIDGET_FONT_PIXEL_HEIGHT + _DUAL_VFO_WIDGET_FREQUENCY_STEP_VERTICAL_MARGIN

/* VFO BLOCK END */

/* SMETER BLOCK START */

#define _DIGITAL_SMETER_WIDGET_BAR_COUNT 42
#define _DIGITAL_SMETER_WIDGET_BAR_WIDTH 3
#define _DIGITAL_SMETER_WIDGET_BAR_HORIZONTAL_MARGIN 2
#define SMETER_BAR_HEIGHT 18
#define _DIGITAL_SMETER_WIDGET_BAR_DISABLED_BACKGROUND_COLOR 0x8410

/* SMETER BLOCK END */

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H
