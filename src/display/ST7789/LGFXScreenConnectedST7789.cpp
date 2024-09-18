#include "LGFXScreenConnectedST7789.hpp"

LGFXScreenConnectedST7789::LGFXScreenConnectedST7789(LovyanGFX *display) : LGFXScreen(display)
{
    if (display != nullptr)
    {
    }
}

LGFXScreenConnectedST7789::~LGFXScreenConnectedST7789()
{
    this->parentDisplay = nullptr;
}

bool LGFXScreenConnectedST7789::Refresh(bool force)
{
    if (this->parentDisplay != nullptr)
    {
        bool changed = true;
        return (changed);
    }
    else
    {
        return (false);
    }
}
