#pragma once

extern "C" {
    #include "lvgl.h"
    #include "Weather.h"
}

namespace ws {
    class WeatherWidget {
    public:
        WeatherWidget() = default;
        ~WeatherWidget() = default;

        auto create(lv_obj_t* src) -> void;

        auto update(const ks_weather_t& weather) -> void;
        
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
        lv_obj_t* m_weatherLabel{};
        lv_obj_t* m_weatherIcon{};
        lv_obj_t* m_temperatureLabel{};
        lv_obj_t* m_humidityLabel{};
        lv_obj_t* m_pressureLabel{};
        lv_obj_t* m_windSpeedLabel{};
    };
}