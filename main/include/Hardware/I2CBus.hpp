#pragma once

extern "C" {
    #include "i2c_bus.h"
}

namespace ws {
    class I2CBus {
    public:
        I2CBus() = default;
        ~I2CBus();

        auto init(const int sda, const int scl, const int speed=400000) -> bool;

        auto get() -> i2c_bus_handle_t& {
            return m_i2cBus;
        }

    private:
        i2c_bus_handle_t m_i2cBus{};
    };
}