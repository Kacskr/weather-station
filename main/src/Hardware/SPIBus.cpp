#include "Hardware/SPIBus.hpp"

#include "esp_log.h"

namespace ws {
    auto init_spi(const int mosi, const int miso, const int clk, spi_host_device_t device) -> bool {
        spi_bus_config_t buscfg = {
            .mosi_io_num = mosi,
            .miso_io_num = miso,
            .sclk_io_num = clk,
            .quadwp_io_num = -1,
            .quadhd_io_num = -1,
            .max_transfer_sz = 240 * 80 * sizeof(uint16_t),
        };

        esp_err_t err = spi_bus_initialize(device, &buscfg, SPI_DMA_CH_AUTO);
        if(err != ESP_OK) {
            ESP_LOGE("SPIBus", "Failed to initialize SPI bus");
            return 0;
        }
        return 1;
    }
}