#include "WeatherWidget.hpp"

#include <unordered_map>
#include <string>

#include "lvgl.h"
LV_FONT_DECLARE(weather48)

static std::unordered_map<int, std::string> s_weatherCodeMap = {
    {0, "Clear sky"},
    {1, "Mainly clear"},
    {2, "Partly cloudy"},
    {3, "Overcast"},
    {45, "Fog"},
    {48, "Fog"},
    {51, "Drizzle"},
    {53, "Drizzle"},
    {55, "Drizzle"},
    {56, "Freezing Drizzle"},
    {57, "Freezing Drizzle"},
    {61, "Rain"},
    {63, "Rain"},
    {65, "Rain"},
    {66, "Freezing Rain"},
    {67, "Freezing Rain"},
    {71, "Snow fall"},
    {73, "Snow fall"},
    {75, "Snow fall"},
    {77, "Snow grains"},
    {80, "Rain showers"},
    {81, "Rain showers"},
    {82, "Rain showers"},
    {85, "Snow showers"},
    {86, "Snow showers"},
    {95, "Thunderstorm"},
    {96, "Thunderstorm"},
    {99, "Thunderstorm"}
};

namespace ws {
    auto WeatherWidget::create(lv_obj_t* src) -> void {
        m_main = lv_obj_create(src);
        m_weatherLabel = lv_label_create(src);
        m_weatherIcon = lv_label_create(src);
        m_temperatureLabel = lv_label_create(src);
        m_humidityLabel = lv_label_create(src);
        m_pressureLabel = lv_label_create(src);
        m_windSpeedLabel = lv_label_create(src);

        lv_obj_set_style_bg_color(m_main, lv_color_hex(0x333333), 0);
        lv_obj_set_style_border_width(m_main, 0, 0);
        lv_obj_set_style_shadow_width(m_main, 0, 0);

        lv_obj_set_style_text_color(m_weatherLabel, lv_color_hex(0x9a9a9a), 0);
        lv_obj_set_style_text_color(m_weatherIcon, lv_color_hex(0x9a9a9a), 0);
        lv_obj_set_style_text_color(m_temperatureLabel, lv_color_hex(0x9a9a9a), 0);
        lv_obj_set_style_text_color(m_humidityLabel, lv_color_hex(0x9a9a9a), 0);
        lv_obj_set_style_text_color(m_pressureLabel, lv_color_hex(0x9a9a9a), 0);
        lv_obj_set_style_text_color(m_windSpeedLabel, lv_color_hex(0x9a9a9a), 0);

        lv_obj_set_style_text_font(m_temperatureLabel, &lv_font_montserrat_20, 0);

        lv_obj_set_style_text_font(m_weatherIcon, &weather48, 0);

        lv_label_set_text(m_weatherLabel, "");
        lv_label_set_text(m_weatherIcon, "");

        realign();
    }

    auto WeatherWidget::update(const ks_weather_t& weather) -> void {
        if(s_weatherCodeMap.contains(weather.weatherCode)){
            lv_label_set_text(m_weatherLabel, s_weatherCodeMap[weather.weatherCode].c_str());
        } else {
            lv_label_set_text(m_weatherLabel, "");
        }
        
        lv_label_set_text(m_weatherIcon, weather.isDay ? ks_symbols_get_weather_icon_day(&weather) : ks_symbols_get_weather_icon_night(&weather));

        lv_label_set_text_fmt(m_temperatureLabel, "%i.%i\xC2\xB0", weather.temperature / 10, weather.temperature % 10);
        lv_label_set_text_fmt(m_humidityLabel, "%i%%", weather.humidity);
        lv_label_set_text_fmt(m_pressureLabel, "%ihPa", weather.surfacePressure / 10);
        lv_label_set_text_fmt(m_windSpeedLabel, "%ikm/h", weather.windSpeed / 10);

        realign();
    }
    
    auto WeatherWidget::realign() -> void {
        lv_obj_set_pos(m_main, m_x, m_y);
        lv_obj_align_to(m_weatherLabel, m_main, LV_ALIGN_TOP_MID, 0, 0);
        lv_obj_align_to(m_weatherIcon, m_main, LV_ALIGN_TOP_LEFT, 0, 20);
        lv_obj_align_to(m_temperatureLabel, m_main, LV_ALIGN_TOP_RIGHT, 0, 20);
        lv_obj_align_to(m_humidityLabel, m_temperatureLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(m_pressureLabel, m_humidityLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align_to(m_windSpeedLabel, m_pressureLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    }
}