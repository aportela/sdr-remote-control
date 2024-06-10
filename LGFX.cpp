#include "LGFX.hpp"

LGFX::LGFX(void) {
  {
    auto cfg = _bus_instance.config();
    cfg.spi_host = VSPI_HOST;   // Utilizar el SPI del hardware VSPI
    cfg.spi_mode = 0;           // Modo SPI (0, 1, 2, 3)
    cfg.freq_write = 40000000;  // Frecuencia de escritura
    cfg.freq_read = 16000000;   // Frecuencia de lectura
    cfg.spi_3wire = false;      // No utilizar modo de 3 hilos
    cfg.use_lock = true;        // Utilizar bloqueo durante las transacciones
    cfg.dma_channel = 1;        // Canal DMA (0=Sin uso, 1=Canal 1, 2=Canal 2)
    cfg.pin_sclk = 18;          // Pin de reloj SCK
    cfg.pin_mosi = 23;          // Pin MOSI
    cfg.pin_miso = 19;          // Pin MISO
    cfg.pin_dc = 2;             // Pin de datos/comando DC

    _bus_instance.config(cfg);               // Aplicar configuración
    _panel_instance.setBus(&_bus_instance);  // Asignar el bus al panel
  }

  {  // Configuración del panel
    auto cfg = _panel_instance.config();
    cfg.pin_cs = 15;    // Pin Chip Select
    cfg.pin_rst = 4;    // Pin de reinicio
    cfg.pin_busy = -1;  // Pin de ocupado (si no se utiliza, poner -1)

    cfg.panel_width = 320;     // Ancho del panel
    cfg.panel_height = 480;    // Altura del panel
    cfg.offset_x = 0;          // Desplazamiento en X
    cfg.offset_y = 0;          // Desplazamiento en Y
    cfg.offset_rotation = 1;   // Rotación de la pantalla
    cfg.dummy_read_pixel = 8;  // Lecturas dummy por pixel
    cfg.dummy_read_bits = 1;   // Lecturas dummy por bits
    cfg.readable = true;       // La pantalla puede ser leída
    cfg.invert = true;         // No invertir los colores
    cfg.rgb_order = false;     // Orden de color RGB
    cfg.dlen_16bit = false;    // Longitud de datos de 16 bits
    cfg.bus_shared = true;     // El bus es compartido con otros dispositivos

    _panel_instance.config(cfg);  // Aplicar configuración
  }
  setPanel(&_panel_instance);  // Asignar el panel al dispositivo
};
