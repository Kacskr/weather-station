#pragma once

extern "C" {
    #include "lvgl.h"
    #include "Weather.h"
}

namespace ws {
        class WeatherPanel {
    public:
        WeatherPanel() = default;
        ~WeatherPanel() = default;
        
        auto create(lv_obj_t* src) -> void;

        auto set_position(const int x, const int y) -> void {
            m_x = x;
            m_y = y;
            realign();
        }
        auto set_size(const int width, const int height) -> void {
            lv_obj_set_size(m_main, width, height);
            realign();
        }
        auto set_day_label(const char* str) -> void {
            lv_label_set_text(m_dayLabel, str);
            realign();
        }
        auto set_temperature_label(const int minTemp, const int maxTemp) -> void {
            lv_label_set_text_fmt(m_tempLabel, "%i\xC2\xB0  %i\xC2\xB0", minTemp, maxTemp);
            realign();
        }
        auto set_weather_icon(const char* icon) -> void {
            lv_label_set_text(m_icon, icon);
            realign();
        }

    private:
        auto realign() -> void;

    private:
        lv_obj_t* m_main{};
        lv_obj_t* m_dayLabel{};
        lv_obj_t* m_tempLabel{};
        lv_obj_t* m_icon{};

        int m_x{};
        int m_y{};
    };
}