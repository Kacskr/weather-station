#pragma once

#include "ILI9341.hpp"

namespace ws {
    class ILI9341Builder {
    public:
        ILI9341Builder() {
            m_display = new ws::ILI9341();
        }
        virtual ~ILI9341Builder() = default;

        auto set_pins(const int dcPin, const int rstPin, const int csPin) -> ILI9341Builder& {
            m_dcPin = dcPin;
            m_rstPin = rstPin;
            m_csPin = csPin;
            return *this;
        }
        auto set_resolution(const int width, const int height) -> ILI9341Builder& {
            m_width = width;
            m_height = height;
            return *this;
        }
        auto set_led(const int pin) -> ILI9341Builder& {
            m_ledPin = pin;
            return *this;
        }

        auto build(spi_host_device_t device) -> ws::ILI9341* {
            ws::ILI9341* m_display{};
            
            m_display->init(m_dcPin, m_rstPin, m_csPin, device);

            return m_display;
        }

    private:
        int m_width{-1};
        int m_height{-1};

        int m_dcPin{-1};
        int m_rstPin{-1};
        int m_csPin{-1};
        int m_ledPin{-1};
    }
}