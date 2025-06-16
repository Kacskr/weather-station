#pragma once

namespace ws {
    // Weather sensor
    constexpr int WS_SDA = 5;
    constexpr int WS_SCL = 4;

    // Display
    constexpr int D_MISO = 41;
    constexpr int D_MOSI = 40;
    constexpr int D_CLK = 39;
    constexpr int D_LED = 42;
    constexpr int D_DC = 38;
    constexpr int D_CS = 47;
    constexpr int D_RST = 48;

    constexpr int T_CS = 2;
    constexpr int T_INT = 1;
    
    constexpr int SD_CS = 21;


    extern auto reset_pins() -> void;
}