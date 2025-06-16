#pragma once

extern "C" {
    #include "lvgl.h"
}

namespace ws {
    class HomeWidget {
    public:
        HomeWidget() = default;
        ~HomeWidget() = default;

        auto create(lv_obj_t* src) -> void;

        auto update(const float temperature, const float humidity, const float pressure) -> void;

        auto set_position(const int x, const int y) -> void {
            m_x = x;
            m_y = y;
            realign();
        }
        auto set_size(const int width, const int height) -> void {
            lv_obj_set_size(m_main, width, height);
            realign();
        }

    private:
        auto realign() -> void;
        
    private:
        int m_x{};
        int m_y{};

        lv_obj_t* m_main{};
        lv_obj_t* m_icon{};
        lv_obj_t* m_temperatureLabel{};
        lv_obj_t* m_humidityLabel{};
        lv_obj_t* m_pressureLabel{};
    };
}