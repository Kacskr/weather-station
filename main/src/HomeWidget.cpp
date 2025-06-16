#include "HomeWidget.hpp"

namespace ws {
    auto HomeWidget::create(lv_obj_t* src) -> void {
        m_main = lv_obj_create(src);
        m_icon = lv_label_create(src);
        m_temperatureLabel = lv_label_create(src);
        m_humidityLabel = lv_label_create(src);
        m_pressureLabel = lv_label_create(src);

        lv_obj_set_style_bg_color(m_main, lv_color_hex(0x333333), 0);
        lv_obj_set_style_border_width(m_main, 0, 0);
        lv_obj_set_style_shadow_width(m_main, 0, 0);
        
        lv_obj_set_style_text_color(m_icon, lv_color_hex(0x9a9a9a), 0);
        lv_obj_set_style_text_color(m_temperatureLabel, lv_color_hex(0x9a9a9a), 0);
        lv_obj_set_style_text_color(m_humidityLabel, lv_color_hex(0x9a9a9a), 0);
        lv_obj_set_style_text_color(m_pressureLabel, lv_color_hex(0x9a9a9a), 0);
        
        lv_obj_set_style_text_font(m_icon, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_font(m_temperatureLabel, &lv_font_montserrat_20, 0);
        lv_label_set_text(m_icon, LV_SYMBOL_HOME);
        
        realign();
    }

    auto HomeWidget::update(const float temperature, const float humidity, const float pressure) -> void { 
        lv_label_set_text_fmt(m_temperatureLabel, "%i.%i\xC2\xB0", int(temperature), (int(temperature * 10.0f) % 10));
        lv_label_set_text_fmt(m_humidityLabel, "%i%%", int(humidity));
        lv_label_set_text_fmt(m_pressureLabel, "%ihPa", int(pressure));

        realign();
    }

    auto HomeWidget::realign() -> void {
        lv_obj_set_pos(m_main, m_x, m_y);
        lv_obj_align_to(m_icon, m_main, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_align_to(m_temperatureLabel, m_icon, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
        lv_obj_align_to(m_humidityLabel, m_temperatureLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(m_pressureLabel, m_humidityLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    }
}