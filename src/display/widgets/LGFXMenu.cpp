#include "LGFXMenu.hpp"

#define DEFAULT_COLOR 0xFFFF
#define DEFAULT_BG 0x0000

// TODO: ADJUST WIDH & MARGIN
#define BUTTON_MARGIN_X 10
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

LGFXMenu::LGFXMenu(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding) : Menu()
{
    if (displayPtr != nullptr)
    {
        this->parentDisplayPtr = displayPtr;
#ifdef DEBUG_SCREEN_WIDGETS_BOUNDS
        this->parentDisplayPtr->drawRect(xOffset, yOffset, width, height, TFT_WHITE);
#endif
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

LGFXMenu::~LGFXMenu()
{
    for (uint8_t i = 0; i < TOTAL_MENU_BUTTONS; i++)
    {
        delete (this->buttons[i]);
    }
    this->parentDisplayPtr = nullptr;
}

void LGFXMenu::initMenu(void)
{
    for (uint8_t i = 0, f = 1, row = 0, col = 0; i < TOTAL_MENU_BUTTONS; i++, f++)
    {
        char topLabel[24];
        if (f > BUTTONS_PER_PAGE)
        {
            f = 1;
        }
        sprintf(topLabel, "F%d", f);
        char mainLabel[24];
        sprintf(mainLabel, buttonLabels[i]);
        this->buttons[i] = new LGFXMenuButton(
            this->parentDisplayPtr,
            i,                                                         // index
            (col * (BUTTON_WIDTH + BUTTON_MARGIN_X)) + this->xOffset,  // x
            (row * (BUTTON_HEIGHT + BUTTON_MARGIN_Y)) + this->yOffset, // y
            topLabel,                                                  // top label
            mainLabel,                                                 // bottom label
            i == 0,                                                    // active
            buttonCallbacks[i]                                         // callback
        );
        col++;
        if (col == COLS_PER_PAGE)
        {
            col = 0;
            row++;
        }
        if (row == ROWS_PER_PAGE)
        {
            row = 0;
        }
    }
}

void LGFXMenu::refresh(bool force)
{
    uint8_t startIndex = this->currentPage * BUTTONS_PER_PAGE;
    uint8_t endIndex = startIndex + BUTTONS_PER_PAGE;
    for (uint8_t i = startIndex; i < endIndex; i++)
    {
        if (force)
        {
            this->buttons[i]->onChange();
        }
        // TODO: save/restore active flag on page changes ???
        this->buttons[i]->draw();
    }
}

void LGFXMenu::setButtonEnabled(uint8_t btnIndex)
{
    if (btnIndex < BUTTONS_PER_PAGE)
    {
        this->buttons[btnIndex]->setActive(true);
    }
}

void LGFXMenu::setButtonDisabled(uint8_t btnIndex)
{
    if (btnIndex < BUTTONS_PER_PAGE)
    {
        this->buttons[btnIndex]->setActive(false);
    }
}

void LGFXMenu::previousPage(void)
{
    if (this->currentPage > 0)
    {
        this->currentPage--;
        this->refresh(true);
    }
}

void LGFXMenu::nextPage(void)
{
    if (this->currentPage < this->totalPages)
    {
        this->currentPage++;
        this->refresh(true);
    }
}