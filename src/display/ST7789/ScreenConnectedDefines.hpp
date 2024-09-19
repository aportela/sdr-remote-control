
#ifndef SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H
#define SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H

#define DISPLAY_PADDING 4 // real draw starts at this padding from (0,0)

/* VFO BLOCK START */

#define VFO_WIDGETS_FONT_SIZE 2              // common font size for all VFO fields (index, freq, mode)
#define VFO_WIDGETS_FONT_PIXEL_HEIGHT 16     // total pixel height for current font size
#define VFO_WIDGETS_X_OFFSET DISPLAY_PADDING // VFO's x offset
#define VFO_WIDGETS_Y_OFFSET DISPLAY_PADDING // VFO's y offset
#define VFO_FREQUENCY_WIDGETS_X_OFFSET 60    // VFO's frequency x offset
#define VFO_MODE_WIDGETS_X_OFFSET 280        // VFO's mode x offset

#define VFO_WIDGETS_STEP_HORIZONTAL_MARGIN 2                     // frequency step indicator horizontal margin
#define VFO_WIDGETS_STEP_VERTICAL_MARGIN 4                       // frequency step indicator vertical margin
#define VFO_WIDGETS_STEP_WIDTH 12                                // vfo frequency step indicator width
#define VFO_WIDGETS_STEP_HEIGHT 2                                // vfo frequency step indicator height
#define VFO_WIDGETS_STEP_X_OFFSET VFO_FREQUENCY_WIDGETS_X_OFFSET // first vfo step frecuency x offset

#define VFO_WIDGETS_SINGLE_VFO_LINE_HEIGHT (VFO_WIDGETS_FONT_PIXEL_HEIGHT + VFO_WIDGETS_STEP_VERTICAL_MARGIN + VFO_WIDGETS_STEP_HEIGHT + VFO_WIDGETS_STEP_VERTICAL_MARGIN) // single VFO line (frequency step indicator included) height

// primary VFO offsets
#define PRIMARY_VFO_WIDGET_X_OFFSET VFO_WIDGETS_X_OFFSET
#define PRIMARY_VFO_WIDGET_Y_OFFSET VFO_WIDGETS_Y_OFFSET
#define PRIMARY_VFO_STEP_Y_OFFSET PRIMARY_VFO_WIDGET_Y_OFFSET + VFO_WIDGETS_FONT_PIXEL_HEIGHT + VFO_WIDGETS_STEP_VERTICAL_MARGIN

// secondary VFO offsets
#define SECONDARY_VFO_WIDGET_X_OFFSET VFO_WIDGETS_X_OFFSET
#define SECONDARY_VFO_WIDGET_Y_OFFSET VFO_WIDGETS_Y_OFFSET + VFO_WIDGETS_SINGLE_VFO_LINE_HEIGHT
#define SECONDARY_VFO_STEP_Y_OFFSET SECONDARY_VFO_WIDGET_Y_OFFSET + VFO_WIDGETS_FONT_PIXEL_HEIGHT + VFO_WIDGETS_STEP_VERTICAL_MARGIN

/* VFO BLOCK END */

/* SMETER BLOCK START */

#define SMETER_BAR_COUNT 42
#define SMETER_BAR_WIDTH 3
#define SMETER_BAR_HORIZONTAL_MARGIN 2
#define SMETER_BAR_HEIGHT 18
#define SMETER_BAR_BG_COLOR 0x8410

/* SMETER BLOCK END */

#endif // SDR_REMOTE_CONTROL_LGFX_SCREEN_CONNECTED_DEFINES_H
