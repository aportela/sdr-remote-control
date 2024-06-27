#include "Menu-ILI9498.hpp"

#define DEFAULT_COLOR 0xFFFF
#define DEFAULT_BG 0x0000

// TODO: ADJUST WIDH & MARGIN
#define BUTTON_MARGIN_X 12
#define BUTTON_MARGIN_Y 4

#define BUTTON_WIDTH 111
#define BUTTON_HEIGHT 48

// TWO DIGITS TOP LABEL (Fx)
#define TOP_BUTTON_LABEL_X 42
#define TOP_BUTTON_LABEL_Y 5

// MAIN LABEL (Fn DESCRIPTION)
#define MAIN_BUTTON_LABEL_X 9
#define MAIN_BUTTON_LABEL_Y 28

#define COLS_PER_PAGE 4
#define ROWS_PER_PAGE 2

#define BUTTONS_PER_PAGE (COLS_PER_PAGE * ROWS_PER_PAGE)

#define TOTAL_MENU_PAGES 2

void defaultCallBack(void)
{
}

const char *buttonLabels[] = {
    "  TUNE  ",
    " VOLUME ",
    " FILTER ",
    "SETTINGS",
    "VFO  A/B",
    "VFO STEP",
    "VFO MODE",
    ">>>>>>>>",

    "+++ BAND",
    "--- BAND",
    "SETTINGS",
    "        ",
    "        ",
    "        ",
    "        ",
    "<<<<<<<<",
};

const ButtonCallback buttonCallbacks[] = {
    defaultCallBack,
    defaultCallBack,
    defaultCallBack,
    defaultCallBack,
    defaultCallBack,
    defaultCallBack,
    defaultCallBack,
    defaultCallBack,

    defaultCallBack,
    defaultCallBack,
    defaultCallBack,
    defaultCallBack,
    defaultCallBack,
    defaultCallBack,
    defaultCallBack,
    defaultCallBack,
};

MenuILI9488::MenuILI9488(LGFX *existingDisplay, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset) : Menu()
{
    if (existingDisplay != nullptr)
    {
        this->display = existingDisplay;
        this->width = width;
        this->height = height;
        this->xOffset = xOffset;
        this->yOffset = yOffset;
        this->totalPages = TOTAL_MENU_PAGES;
        this->initMenu();
        this->refresh(true);
    }
    //   1: define buttons (index, labels, callbacks)
    //   2: set default menu start/end indexes
    //   3: refresh (draw from start to end)
}

MenuILI9488::~MenuILI9488()
{
    for (uint8_t i = 0; i < TOTAL_MENU_BUTTONS; i++)
    {
        delete (this->buttons[i]);
    }
    this->display = nullptr;
}

void MenuILI9488::initMenu(void)
{
    for (uint8_t i = 0, f = 1; i < TOTAL_MENU_BUTTONS; i++, f++)
    {
        char topLabel[9];
        if (f > BUTTONS_PER_PAGE)
        {
            f = 1;
        }
        sprintf(topLabel, "F%d", f);
        char mainLabel[9];
        sprintf(mainLabel, buttonLabels[i]);
        this->buttons[i] = new MenuButtonILI9488(
            this->display,
            i,                                                       // index
            (i * (BUTTON_WIDTH + BUTTON_MARGIN_X)) + this->xOffset,  // x
            (0 * (BUTTON_HEIGHT + BUTTON_MARGIN_Y)) + this->yOffset, // y
            topLabel,                                                // top label
            mainLabel,                                               // bottom label
            i == 0,                                                  // active
            buttonCallbacks[i]                                       // callback
        );
    }
}

void MenuILI9488::refresh(bool forceAll)
{
    uint8_t startIndex = this->currentPage * BUTTONS_PER_PAGE;
    uint8_t endIndex = startIndex + BUTTONS_PER_PAGE;
    for (uint8_t i = startIndex; i < BUTTONS_PER_PAGE; i++)
    {
        if (forceAll)
        {
            this->buttons[i]->onChange();
        }
        // TODO: save/restore active flag on page changes ???
        this->buttons[i]->draw();
    }
}

void MenuILI9488::previousPage(void)
{
    if (this->currentPage > 0)
    {
        this->currentPage--;
        this->refresh(true);
    }
}

void MenuILI9488::nextPage(void)
{
    if (this->currentPage < this->totalPages)
    {
        this->currentPage++;
        this->refresh(true);
    }
}