#include "Hardware/Pins.hpp"

#include "driver/gpio.h"

namespace ws {
    auto reset_pins() -> void {
        gpio_reset_pin((gpio_num_t)ws::WS_SDA);
        gpio_reset_pin((gpio_num_t)ws::WS_SCL);
        gpio_reset_pin((gpio_num_t)ws::D_MISO);
        gpio_reset_pin((gpio_num_t)ws::D_MOSI);
        gpio_reset_pin((gpio_num_t)ws::D_CLK);
        gpio_reset_pin((gpio_num_t)ws::D_LED);
        gpio_reset_pin((gpio_num_t)ws::D_DC);
        gpio_reset_pin((gpio_num_t)ws::D_CS);
        gpio_reset_pin((gpio_num_t)ws::D_RST);
        gpio_reset_pin((gpio_num_t)ws::T_CS);
        gpio_reset_pin((gpio_num_t)ws::T_INT);
        gpio_reset_pin((gpio_num_t)ws::SD_CS);
    }
}