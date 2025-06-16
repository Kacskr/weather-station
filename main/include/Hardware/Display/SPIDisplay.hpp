#pragma once

#include "driver/spi_master.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

#include "Display.hpp"

namespace ws {
    class SPIDisplay : public Display {
    public:
        SPIDisplay() = default;
        virtual ~SPIDisplay() = default;
        
        virtual auto init(const int width, const int height, const int dcPin, const int rstPin, const int csPin, const int ledPin, spi_host_device_t device) -> void = 0;
    
        auto get_io_handle() -> esp_lcd_panel_io_handle_t& {
            return m_ioHandle;
        }
        auto get_panel_handle() -> esp_lcd_panel_handle_t& {
            return m_panelHandle;
        }

    private:
        esp_lcd_panel_io_handle_t m_ioHandle{};
        esp_lcd_panel_handle_t m_panelHandle{};
    };
}