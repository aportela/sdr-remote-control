#include "LGFX.hpp"
#include "../DisplayConfiguration.hpp"
#include "LGFXScreenConnecting.hpp"
#include "LGFXScreenConnected.hpp"

LGFX::LGFX(uint8_t pinSDA, uint8_t pinSCL, uint8_t pinCS, uint8_t pinDC, uint8_t pinRST, uint16_t width, uint16_t height, uint8_t rotation, bool invertColors, Transceiver *trx) : trx(trx)
{
    auto cfg = this->_bus_instance.config();
    cfg.spi_host = VSPI_HOST;  // Utilizar el SPI del hardware VSPI
    cfg.spi_mode = 0;          // Modo SPI (0, 1, 2, 3)
    cfg.freq_write = 40000000; // Frecuencia de escritura
    cfg.freq_read = 16000000;  // Frecuencia de lectura
    cfg.spi_3wire = false;     // No utilizar modo de 3 hilos
    cfg.use_lock = true;       // Utilizar bloqueo durante las transacciones
    cfg.dma_channel = 1;       // Canal DMA (0=Sin uso, 1=Canal 1, 2=Canal 2)
    cfg.pin_sclk = pinSCL;     // Pin de reloj SCK
    cfg.pin_mosi = pinSDA;     // Pin MOSI
    cfg.pin_dc = pinDC;        // Pin de datos/comando DC

    this->_bus_instance.config(cfg);                    // Aplicar configuración
    this->_panel_instance.setBus(&this->_bus_instance); // Asignar el bus al panel

    auto cfg_panel = this->_panel_instance.config();
    cfg_panel.pin_cs = pinCS;   // Pin Chip Select
    cfg_panel.pin_rst = pinRST; // Pin de reinicio
    cfg_panel.pin_busy = -1;    // Pin de ocupado (si no se utiliza, poner -1)

    cfg_panel.panel_width = width;        // Ancho del panel
    cfg_panel.panel_height = height;      // Altura del panel
    cfg_panel.offset_x = 0;               // Desplazamiento en X
    cfg_panel.offset_y = 0;               // Desplazamiento en Y
    cfg_panel.offset_rotation = rotation; // Rotación de la pantalla
    cfg_panel.dummy_read_pixel = 8;       // Lecturas dummy por pixel
    cfg_panel.dummy_read_bits = 1;        // Lecturas dummy por bits
    cfg_panel.readable = true;            // La pantalla puede ser leída
    cfg_panel.invert = invertColors;      // No invertir los colores
    cfg_panel.rgb_order = false;          // Orden de color RGB
    cfg_panel.dlen_16bit = false;         // Longitud de datos de 16 bits
    cfg_panel.bus_shared = true;          // El bus es compartido con otros dispositivos

    this->_panel_instance.config(cfg_panel); // Aplicar configuración

    setPanel(&this->_panel_instance); // Asignar el panel al dispositivo

    this->init();
    this->fillScreen(TFT_BLACK);
}

LGFX::~LGFX()
{
    if (this->currentScreen != nullptr)
    {
        delete this->currentScreen;
        this->currentScreen = nullptr;
    }
}

void LGFX::InitScreen(SCREEN_TYPE screenType)
{
    switch (screenType)
    {
    case SCREEN_TYPE_NOT_CONNECTED:
        if (this->currentScreen == nullptr)
        {
            this->currentScreen = new LGFXScreenConnecting(this);
        }
        this->currentScreenType = screenType;
        break;
    case SCREEN_TYPE_CONNECTED:
        if (this->currentScreen == nullptr)
        {
            TransceiverStatus trxStatus;
            this->trx->getCurrentStatus(&trxStatus);
            this->currentScreen = new LGFXScreenConnected(this, &trxStatus);
        }
        this->currentScreenType = screenType;
        break;
    case SCREEN_TYPE_NONE:
        this->currentScreenType = screenType;
    default:
        break;
    }
}

void LGFX::DeleteCurrentScreen(void)
{
    if (this->currentScreenType != SCREEN_TYPE_NONE)
    {
        if (this->currentScreen != nullptr)
        {
            delete this->currentScreen;
            this->currentScreen = nullptr;
            this->currentScreenType == SCREEN_TYPE_NONE;
        }
    }
}

SCREEN_TYPE LGFX::GetCurrentScreenType(void)
{
    return (this->currentScreenType);
}

bool LGFX::FlipToScreen(SCREEN_TYPE screenType)
{
    if (screenType != this->currentScreenType)
    {
        this->DeleteCurrentScreen();
        this->InitScreen(screenType);
        return (true);
    }
    else
    {
        return (false);
    }
}

bool LGFX::ToggleScreen(void)
{
    bool success = false;
    switch (this->GetCurrentScreenType())
    {
    default:
        break;
    }
    return (success);
}

bool LGFX::Refresh(bool force, const TransceiverStatus *currentTrxStatus)
{
    if (this->currentScreenType != SCREEN_TYPE_NONE)
    {
        if (this->currentScreen != nullptr)
        {
            return (this->currentScreen->Refresh(force, currentTrxStatus));
        }
        else
        {
            return (false);
        }
    }
    else
    {
        return (false);
    }
}
