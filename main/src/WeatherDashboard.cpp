#include "WeatherDashboard.hpp"
#include <sstream>

namespace ws {
    static void close_button_callback(lv_event_t* e) {
        WeatherDashboard* wd = (WeatherDashboard*)lv_event_get_user_data(e);
        wd->set_visibility(0);
    }

    static const char* s_days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    auto WeatherDashboard::create(lv_obj_t* src) -> void {
        m_main = lv_obj_create(src);
        lv_obj_set_style_bg_color(m_main, lv_color_hex(0x333333), 0);
        lv_obj_set_style_border_width(m_main, 0, 0);
        lv_obj_set_style_shadow_width(m_main, 0, 0);
        lv_obj_set_style_radius(m_main, 0, 0);
        lv_obj_set_scrollbar_mode(m_main, LV_SCROLLBAR_MODE_OFF);
        lv_obj_clear_flag(m_main, LV_OBJ_FLAG_SCROLLABLE);

        m_button = lv_button_create(m_main);
        m_buttonLabel = lv_label_create(m_button);

        lv_obj_add_event_cb(m_button, close_button_callback, LV_EVENT_CLICKED, this);
        lv_label_set_text(m_buttonLabel, "Close");

        m_label = lv_label_create(m_main);
        lv_obj_set_style_text_color(m_label, lv_color_hex(0x9a9a9a), 0);

        m_table = lv_table_create(m_main);
        lv_obj_set_style_bg_color(m_table, lv_color_hex(0x333333), 0);
        lv_obj_set_style_text_color(m_table, lv_color_hex(0x9a9a9a), 0);
        lv_obj_set_style_border_width(m_table, 0, 0);
        
        lv_obj_set_style_bg_opa(m_table, LV_OPA_COVER, LV_PART_ITEMS);
        lv_obj_set_style_bg_color(m_table, lv_color_hex(0x555555), LV_PART_ITEMS);
        lv_obj_set_style_text_color(m_table, lv_color_hex(0xc4c4c4), LV_PART_ITEMS);
        lv_obj_set_style_border_color(m_table, lv_color_hex(0x222222), LV_PART_ITEMS);
    }

    auto WeatherDashboard::update(extended_weather_data_t& weatherData, uint8_t day) -> void {
        std::stringstream ss;
        ss << (weatherData.month < 10 ? "0" : "") << +weatherData.month << "." << (weatherData.day < 10 ? "0" : "") << +weatherData.day;
        ss << " " << s_days[day];
        lv_label_set_text(m_label, ss.str().c_str());

        lv_obj_align_to(m_label, m_button, LV_ALIGN_OUT_RIGHT_MID, 20, 0);
        
        lv_table_set_cell_value(m_table, 0, 0, "Time");
        lv_table_set_cell_value(m_table, 0, 1, "Temp.");
        lv_table_set_cell_value(m_table, 0, 2, "Rain");

        for(int i = 0; i < 24; i++) {
            const int index = (i + 8) % 24;
            ss.str("");
            ss << index << ":00";
            lv_table_set_cell_value(m_table, i + 1, 0, ss.str().c_str());

            ss.str("");
            ss << weatherData.temperature[index] / 10 << "." << weatherData.temperature[index] % 10 << "\xC2\xB0" "C";
            lv_table_set_cell_value(m_table, i + 1, 1, ss.str().c_str());

            ss.str("");
            ss << weatherData.precipitationProbability[index] << "%";
            lv_table_set_cell_value(m_table, i + 1, 2, ss.str().c_str());
        }
        lv_obj_scroll_to(m_table, 0, 0, LV_ANIM_OFF);
        lv_obj_set_width(m_table, 240);
        lv_obj_set_height(m_table, 275);
        lv_obj_align(m_table, LV_ALIGN_TOP_MID, 0, 45);
        lv_table_set_column_width(m_table, 0, 80);
        lv_table_set_column_width(m_table, 1, 80);
        lv_table_set_column_width(m_table, 2, 80);
    }

    auto WeatherDashboard::realign() -> void {
        lv_obj_set_pos(m_main, m_x, m_y);
    }
}