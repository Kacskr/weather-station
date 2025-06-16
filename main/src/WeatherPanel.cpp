#include "WeatherPanel.hpp"

LV_FONT_DECLARE(weather32)

namespace ws {
    auto WeatherPanel::create(lv_obj_t* src) -> void {
        m_main = lv_obj_create(src);
        m_dayLabel = lv_label_create(src);
        m_tempLabel = lv_label_create(src);
        m_icon = lv_label_create(src);

        lv_obj_set_style_bg_color(m_main, lv_color_hex(0x333333), 0);
        lv_obj_set_style_text_color(m_dayLabel, lv_color_hex(0x9a9a9a), 0);
        lv_obj_set_style_text_color(m_tempLabel, lv_color_hex(0x9a9a9a), 0);
        lv_obj_set_style_text_color(m_icon, lv_color_hex(0x9a9a9a), 0);

        lv_obj_set_style_border_width(m_main, 0, 0);
        lv_obj_set_style_shadow_width(m_main, 0, 0);

        lv_obj_set_style_text_font(m_dayLabel, &lv_font_montserrat_18, 0);
        lv_obj_set_style_text_font(m_icon, &weather32, 0);

        lv_label_set_text(m_icon, "");

        set_size(70, 70);
        realign();
    }

    auto WeatherPanel::realign() -> void {
        lv_obj_set_pos(m_main, m_x, m_y);
        lv_obj_align_to(m_dayLabel, m_main, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_align_to(m_icon, m_dayLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(m_tempLabel, m_icon, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    }
}