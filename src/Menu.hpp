#ifndef SDR_REMOTE_CONTROL_MENU_H
#define SDR_REMOTE_CONTROL_MENU_H

#include <cstdint>
#include <cstddef>
#include "MenuConfiguration.hpp"
#include "MenuItem.hpp"

#define MENU_LABEL_LENGTH 8

class Menu
{
private:
    uint8_t currentPage = 1;
    uint8_t totalPages = 0;
    MenuItem *items[TOTAL_MENU_ITEMS] = {};

public:
    Menu(const char **labels);
    ~Menu();
    void getLabel(uint8_t index, char *buf, size_t count);
    void setActive(uint8_t index, bool flag, bool unsetOthers = true);
    void click(uint8_t index);
    bool isActive(uint8_t index);
    bool isBlinking(uint8_t index);
    uint8_t getCurrentPage(void);
    uint8_t getTotalPages(void);
    bool nextPage(void);
    bool previousPage(void);
    bool setPage(uint8_t index);
};

#endif // SDR_REMOTE_CONTROL_MENU_H
