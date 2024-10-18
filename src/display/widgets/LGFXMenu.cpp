#include "LGFXMenu.hpp"

#define DEFAULT_COLOR 0xFFFF
#define DEFAULT_BG 0x0000

const char *buttonLabels[TOTAL_MENU_ITEMS] = {
    "  TUNE  ",
    " VOLUME ",
    " FILTER ",
    "SETTINGS",
    "VFO  A/B",
    "VFO STEP",
    "VFO MODE",
    ">>>>>>>>",

    "VFO A=>B",
    "VFO B=>A",
    "+++ BAND",
    "--- BAND",
    "SETTINGS",
    "        ",
    "        ",
    "<<<<<<<<",
};

void defaultCallBack(void)
{
}

const ButtonCallback buttonCallbacks[TOTAL_MENU_ITEMS] = {
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

LGFXMenu::LGFXMenu(LovyanGFX *displayPtr, uint16_t width, uint16_t height, uint16_t xOffset, uint16_t yOffset, uint8_t padding, const Menu *menuPtr) : LGFXWidget(displayPtr, width, height, xOffset, yOffset, padding), menuPtr(menuPtr)
{
    if (displayPtr != nullptr)
    {
        this->totalPages = TOTAL_MENU_PAGES;
        this->initMenu();
        this->refresh(true);

        //   1: define buttons (index, labels, callbacks)
        //   2: set default menu start/end indexes
        //   3: refresh (draw from start to end)
    }
}

LGFXMenu::~LGFXMenu()
{
    for (uint8_t i = 0; i < TOTAL_MENU_ITEMS; i++)
    {
        delete (this->buttons[i]);
        this->buttons[i] = nullptr;
    }
    this->parentDisplayPtr = nullptr;
    this->menuPtr = nullptr;
}

void LGFXMenu::initMenu(void)
{
    for (uint8_t i = 0, f = 1, row = 0, col = 0; i < TOTAL_MENU_ITEMS; i++, f++)
    {
        char topLabel[8];
        if (f > MENU_ITEMS_PER_PAGE)
        {
            f = 1;
        }
        sprintf(topLabel, "F%d", f);
        char mainLabel[24];
        sprintf(mainLabel, buttonLabels[i]);
        this->buttons[i] = new LGFXMenuButton(
            this->parentDisplayPtr,
            MENU_BUTTON_WIDGET_WIDTH,                                                                                   // width
            MENU_BUTTON_WIDGET_HEIGHT,                                                                                  // height
            (col * (MENU_BUTTON_WIDGET_WIDTH + _MENU_BUTTON_WIDGET_HORIZONTAL_MARGIN)) + this->xOffset + this->padding, // x offset
            (row * (MENU_BUTTON_WIDGET_HEIGHT + _MENU_BUTTON_WIDGET_VERTICAL_MARGIN)) + this->yOffset + this->padding,  // y offset
            MENU_BUTTON_WIDGET_PADDING,                                                                                 // padding
            i,                                                                                                          // index
            topLabel,                                                                                                   // top label
            mainLabel,                                                                                                  // bottom label
            i == 0,                                                                                                     // active
            buttonCallbacks[i]                                                                                          // callback
        );
        col++;
        if (col == MENU_ITEMS_COLS_PER_PAGE)
        {
            col = 0;
            row++;
        }
        if (row == MENU_ITEMS_ROWS_PER_PAGE)
        {
            row = 0;
        }
    }
}

bool LGFXMenu::refresh(bool force)
{
    bool changed = force;
    uint8_t startIndex = this->currentPage * MENU_ITEMS_PER_PAGE;
    uint8_t endIndex = startIndex + MENU_ITEMS_PER_PAGE;
    for (uint8_t i = startIndex; i < endIndex; i++)
    {
        if (force)
        {
            this->buttons[i]->onChange();
        }
        // TODO: save/restore active flag on page changes ???
        this->buttons[i]->refresh(force);
    }
    return (changed);
}

void LGFXMenu::setButtonEnabled(uint8_t btnIndex)
{
    if (btnIndex < MENU_ITEMS_PER_PAGE)
    {
        this->buttons[btnIndex]->setActive(true);
    }
}

void LGFXMenu::setButtonDisabled(uint8_t btnIndex)
{
    if (btnIndex < MENU_ITEMS_PER_PAGE)
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