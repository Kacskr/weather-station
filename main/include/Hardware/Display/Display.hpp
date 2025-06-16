#pragma once

namespace ws {
    class Display {
    public:
        Display() = default;
        virtual ~Display() = default;

        //auto draw_bitmap(const int x, const int y, const int width, const int height, const uint16_t* pixels) -> void;
    };
}