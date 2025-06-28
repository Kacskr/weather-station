#pragma once


extern "C" {
    #include "Weather.h"
    #include "lvgl.h"
}

namespace ws {
    class WeatherDashboard {
    public:
        WeatherDashboard() = default;
        ~WeatherDashboard() = default;

        auto create(lv_obj_t* src) -> void;

        auto update(extended_weather_data_t& weatherData, uint8_t day) -> void;

        auto set_position(const int x, const int y) -> void {
            m_x = x;
            m_y = y;
            realign();
        }
        auto set_size(const int width, const int height) -> void {
            lv_obj_set_size(m_main, width, height);
            realign();
        }

        auto set_visibility(const bool visible) -> void {
            if(visible) {
                lv_obj_clear_flag(m_main, LV_OBJ_FLAG_HIDDEN);
                lv_obj_move_foreground(m_main);
            }
            else {
                lv_obj_add_flag(m_main, LV_OBJ_FLAG_HIDDEN);
            }
        }

    private:
        auto realign() -> void;
        
    private:
        int m_x{};
        int m_y{};

        lv_obj_t* m_main{};
        lv_obj_t* m_button{};
        lv_obj_t* m_buttonLabel{};
        lv_obj_t* m_label{};

        lv_obj_t * m_table;
    };
}