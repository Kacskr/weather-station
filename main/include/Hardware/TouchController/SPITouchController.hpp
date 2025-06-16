#pragma once

#include "TouchController.hpp"

#include "esp_lcd_touch.h"

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"

namespace ws {
    class SPITouchController : public TouchController{
    public:
        SPITouchController() = default;
        virtual ~SPITouchController() = default;

        virtual auto init(const int width, const int height, const int intPin, const int csPin, spi_host_device_t device) -> void = 0;

        auto get_io_handle() -> esp_lcd_panel_io_handle_t& {
            return m_tpIoHandle;
        }
        auto get_handle () -> esp_lcd_touch_handle_t& {
            return m_handle;
        }

    private:
        esp_lcd_panel_io_handle_t m_tpIoHandle{};
        esp_lcd_touch_handle_t m_handle{};
    };
}