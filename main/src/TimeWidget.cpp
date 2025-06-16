#include "TimeWidget.hpp"

static const char* s_days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

namespace ws {
    auto TimeWidget::create(lv_obj_t* src) -> void {
        m_dateLabel = lv_label_create(src);
        m_timeLabel = lv_label_create(src);

        lv_obj_set_style_text_font(m_dateLabel, &lv_font_montserrat_20, 0);
        lv_obj_set_style_text_font(m_timeLabel, &lv_font_montserrat_20, 0);

        lv_obj_align(m_dateLabel, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_set_style_text_color(m_dateLabel, lv_color_hex(0x9a9a9a), 0);

        lv_obj_set_style_text_color(m_timeLabel, lv_color_hex(0x9a9a9a), 0);
    }

    auto TimeWidget::update(struct tm& timeinfo) -> void {
        lv_label_set_text_fmt(m_dateLabel, "%s, %02i.%02i.%i", s_days[timeinfo.tm_wday], timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);

        char timeStr[8];
        strftime(timeStr, sizeof(timeStr), "%H:%M", &timeinfo);
        lv_label_set_text_fmt(m_timeLabel, timeStr);
    
        lv_obj_align(m_dateLabel, LV_ALIGN_TOP_MID, 0, 10);
        lv_obj_align_to(m_timeLabel, m_dateLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    }
}