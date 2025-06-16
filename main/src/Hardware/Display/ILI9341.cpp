#include "Hardware/Display/ILI9341.hpp"

#include "driver/gpio.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_ili9341.h"

namespace ws {
    auto ILI9341::init(const int width, const int height, const int dcPin, const int rstPin, const int csPin, const int ledPin, spi_host_device_t device) -> void {
        if(ledPin != -1) {
            gpio_set_direction((gpio_num_t)ledPin, GPIO_MODE_OUTPUT);
            gpio_set_level((gpio_num_t)ledPin, 0);
        }

        // Install panel IO
        esp_lcd_panel_io_spi_config_t io_config = {
            .cs_gpio_num = csPin,
            .dc_gpio_num = dcPin,
            .spi_mode = 0,
            .pclk_hz = 40000000,
            .trans_queue_depth = 10,
            .lcd_cmd_bits = 8,
            .lcd_param_bits = 8,
        };

        // Attach the LCD to the SPI bus
        ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(device, &io_config, &get_io_handle()));

        esp_lcd_panel_dev_config_t panel_config = {
            .reset_gpio_num = rstPin,
            .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
            .bits_per_pixel = 16,
        };

        // Install ILI9341 panel driver
        esp_lcd_new_panel_ili9341(get_io_handle(), &panel_config, &get_panel_handle());

        esp_lcd_panel_reset(get_panel_handle());
        esp_lcd_panel_init(get_panel_handle());
        esp_lcd_panel_mirror(get_panel_handle(), true, false);

        esp_lcd_panel_disp_on_off(get_panel_handle(), true);

        if(ledPin != -1) {
            gpio_set_level((gpio_num_t)ledPin, 1);
        }
    }
}