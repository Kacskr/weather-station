#pragma once

#include "esp_lcd_ili9341.h"

#include "SPIDisplay.hpp"

namespace ws {
    class ILI9341 final : public SPIDisplay {
    public: 
        ILI9341() = default;
        virtual ~ILI9341() = default;

        virtual auto init(const int width, const int height, const int dcPin, const int rstPin, const int csPin, const int ledPin, spi_host_device_t device) -> void;

    private:

    };
}