#pragma once

#include "driver/spi_master.h"

namespace ws {
    extern auto init_spi(const int mosi, const int miso, const int clk, spi_host_device_t device) -> bool;
}