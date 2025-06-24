#include "WeatherDashboard.hpp"

namespace ws {
    static void close_button_callback(lv_event_t* e) {
        WeatherDashboard* wd = (WeatherDashboard*)lv_event_get_user_data(e);
        wd->set_visibility(0);
    }

    auto WeatherDashboard::create(lv_obj_t* src) -> void {
        m_main = lv_obj_create(src);
        lv_obj_set_style_bg_color(m_main, lv_color_hex(0x333333), 0);
        lv_obj_set_style_border_width(m_main, 0, 0);
        lv_obj_set_style_shadow_width(m_main, 0, 0);
        lv_obj_set_style_radius(m_main, 0, 0);

        m_button = lv_button_create(m_main);
        m_buttonLabel = lv_label_create(m_button);

        lv_obj_add_event_cb(m_button, close_button_callback, LV_EVENT_CLICKED, this);
        lv_label_set_text(m_buttonLabel, "Close");
    }

    auto WeatherDashboard::update(extended_weather_data_t& weatherData) -> void {
        
    }

    auto WeatherDashboard::realign() -> void {
        lv_obj_set_pos(m_main, m_x, m_y);
    }
}