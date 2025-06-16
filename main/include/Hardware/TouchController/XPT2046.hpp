#pragma once

#include "SPITouchController.hpp"

#include "driver/gpio.h"
#include "esp_lcd_touch_xpt2046.h"

namespace ws {
    class XPT2046 final : public SPITouchController {
    public:
        XPT2046() = default;
        virtual ~XPT2046() = default;

        auto init(const int width, const int height, const int intPin, const int csPin, spi_host_device_t device) -> void {
            esp_lcd_panel_io_spi_config_t tp_io_config = ESP_LCD_TOUCH_IO_SPI_XPT2046_CONFIG(csPin);

            // Attach the TOUCH to the SPI bus
            esp_lcd_new_panel_io_spi(device, &tp_io_config, &get_io_handle());

            esp_lcd_touch_config_t tp_cfg = {
                .x_max = (uint16_t)height,
                .y_max = (uint16_t)width,
                .rst_gpio_num = GPIO_NUM_NC,
                .int_gpio_num = static_cast<gpio_num_t>(intPin),
                .flags = {
                    .swap_xy = 0,
                    .mirror_x = 0,
                    .mirror_y = 1,
                },
            };

            // Initialize touch controller XPT2046
            esp_lcd_touch_new_spi_xpt2046(get_io_handle(), &tp_cfg, &get_handle());
        }

        auto read_data() -> void {
            esp_lcd_touch_read_data(get_handle());
        }

    private:
    
    };
}