#include "Menu.hpp"

Menu::Menu(const char **labels, const availableMenuActions menuActions[TOTAL_MENU_ITEMS])
{
    for (uint8_t i = 0; i < TOTAL_MENU_ITEMS; i++)
    {
        this->items[i] = new MenuItem(labels[i], menuActions[i]);
    }
}

Menu::~Menu()
{
    for (uint8_t i = 0; i < TOTAL_MENU_ITEMS; i++)
    {
        delete this->items[i];
        this->items[i] = nullptr;
    }
}

void Menu::getLabel(uint8_t index, char *buf, size_t count)
{
    this->items[index]->getLabel(buf, count);
}

void Menu::setActive(uint8_t index, bool flag, bool unsetOthers)
{
    if (index >= 0 && index < TOTAL_MENU_ITEMS)
    {
        this->items[index]->setActive(flag);
        if (unsetOthers)
        {
            for (uint8_t i = 0; i < TOTAL_MENU_ITEMS; i++)
            {
                if (i != index)
                {
                    this->items[index]->setActive(false);
                }
            }
        }
    }
}

void Menu::click(uint8_t index)
{
    if (index >= 0 && index < TOTAL_MENU_ITEMS)
    {
        this->items[index]->click();
    }
}

bool Menu::isActive(uint8_t index)
{
    if (index >= 0 && index < TOTAL_MENU_ITEMS)
    {
        return (this->items[index]->isActive());
    }
    else
    {
        return (false);
    }
}

bool Menu::isBlinking(uint8_t index)
{
    if (index >= 0 && index < TOTAL_MENU_ITEMS)
    {
        return (this->items[index]->isBlinking());
    }
    else
    {
        return (false);
    }
}

uint8_t Menu::getCurrentPage(void)
{
    return (this->currentPage);
}

uint8_t Menu::getTotalPages(void)
{
    return (this->totalPages);
}

bool Menu::nextPage(void)
{
    if (this->currentPage <= this->totalPages)
    {
        this->currentPage++;
        return (true);
    }
    else
    {
        return (false);
    }
}

bool Menu::previousPage(void)
{
    if (this->currentPage > 1)
    {
        this->currentPage--;
        return (true);
    }
    else
    {
        return (false);
    }
}

bool Menu::setPage(uint8_t index)
{
    if (index != this->currentPage && index > 0 && index <= this->totalPages)
    {
        this->currentPage = index;
        return (true);
    }
    else
    {
        return (false);
    }
}
