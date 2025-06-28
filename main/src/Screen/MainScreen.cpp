#include "Screen/MainScreen.hpp"

#include <sstream>

extern "C" {
    #include "WiFi.h"
    #include "NVS.h"
}

static const char* s_daysAb[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};

namespace ws {
    static void close_button_callback(lv_event_t* e) {
        lv_obj_t* main = (lv_obj_t*)lv_event_get_user_data(e);
        lv_obj_add_flag(main, LV_OBJ_FLAG_HIDDEN);
    }
    static void disconnect_button_callback(lv_event_t* e) {
        if(ks_wifi_is_connected()){
            ks_wifi_disconnect();

            ks_nvs_save_string("ssid", "");
            ks_nvs_save_string("password", "");

            lv_obj_t* main = (lv_obj_t*)lv_event_get_user_data(e);
            lv_obj_add_flag(main, LV_OBJ_FLAG_HIDDEN);
        }
    }

    MainScreen::MainScreen(SharedData& data)
            :   Screen(data) {
        m_timeWidget.create(get());
        
        for(int i = 0; i < 3; i++){
            m_panels[i].create(get());
            m_panels[i].set_size(70, 110);
            m_panels[i].set_position(7.5f + (7.5f + 70.0f) * i, 63);
        }

        m_weatherWidget.create(get());
        m_weatherWidget.set_position(7, 180);
        m_weatherWidget.set_size(148, 132);

        m_homeWidget.create(get());
        m_homeWidget.set_position(162, 180);
        m_homeWidget.set_size(70, 132);

        m_settingsMain = lv_obj_create(get());
        m_settingsIPLabel = lv_label_create(m_settingsMain);
        m_settingsCloseButton = lv_button_create(m_settingsMain);
        m_settingsCloseButtonLabel = lv_label_create(m_settingsCloseButton);
        m_settingsDisconnectButton = lv_button_create(m_settingsMain);
        m_settingsDisconnectButtonLabel = lv_label_create(m_settingsDisconnectButton);

        lv_obj_set_size(m_settingsMain, 240, 320);
        lv_obj_set_style_bg_color(m_settingsMain, lv_color_hex(0x333333), 0);
        lv_obj_set_style_border_width(m_settingsMain, 0, 0);
        lv_obj_set_style_shadow_width(m_settingsMain, 0, 0);
        lv_obj_set_style_radius(m_settingsMain, 0, 0);

        lv_obj_set_style_text_color(m_settingsIPLabel, lv_color_hex(0x9a9a9a), 0);

        lv_obj_add_event_cb(m_settingsCloseButton, close_button_callback, LV_EVENT_CLICKED, m_settingsMain);
        lv_label_set_text(m_settingsCloseButtonLabel, "Close");

        lv_obj_add_event_cb(m_settingsDisconnectButton, disconnect_button_callback, LV_EVENT_CLICKED, m_settingsMain);
        lv_label_set_text(m_settingsDisconnectButtonLabel, "Disconnect");

        lv_obj_align(m_settingsIPLabel, LV_ALIGN_TOP_MID, 0, 10);
        lv_obj_align_to(m_settingsDisconnectButton, m_settingsIPLabel, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
        lv_obj_align(m_settingsCloseButton, LV_ALIGN_BOTTOM_MID, 0, -10);

        lv_obj_add_flag(m_settingsMain, LV_OBJ_FLAG_HIDDEN);

        m_dashboard.create(get());
        m_dashboard.set_visibility(0);
        m_dashboard.set_size(240, 320);

        refresh();
    }

    auto MainScreen::update() -> void {
        m_timeWidget.update(get_data().timeinfo);
        m_weatherWidget.update(ks_weather_get_current_weather());
        m_homeWidget.update(get_data().temperature, get_data().humidity, get_data().pressure);

        if(m_extendedForecast.error == 0){
            for(int i = 0; i < 3; i++){
                if(m_panels[i].was_clicked() && lv_obj_has_flag(m_settingsMain, LV_OBJ_FLAG_HIDDEN)){
                    m_dashboard.update(m_extendedForecast.weatherData[i], m_forecast.weatherData[i].wday);
                    m_dashboard.set_visibility(1);
                }
            }
        }

        if(ks_wifi_is_connected()){
            if(m_lastWifi == 0){
                m_lastWifi = 1;

                wifi_ap_record_t apInfo;
                if(esp_wifi_sta_get_ap_info(&apInfo) == ESP_OK){
                    std::stringstream ss;
                    ss << "IP: " << ks_wifi_get_ip();
                    lv_label_set_text(m_settingsIPLabel, ss.str().c_str());
                } else {
                    lv_label_set_text(m_settingsIPLabel, "");
                }
            }
        } else {
            m_lastWifi = 0;
        }
    }

    auto MainScreen::refresh() -> void {
        if(ks_wifi_is_connected()){
            ks_weather_update_weather();
            m_forecast = ks_weather_get_forecast();
            m_extendedForecast = ks_weather_get_extended_forecast();
            for(int i = 0; i < 3; i++){
                m_panels[i].set_day_label(s_daysAb[m_forecast.weatherData[i].wday]);
                m_panels[i].set_temperature_label(m_forecast.weatherData[i].minTemperature / 10, m_forecast.weatherData[i].maxTemperature / 10);
                m_panels[i].set_weather_icon(ks_symbols_get_weather_icon_neutral(m_forecast.weatherData[i].weatherCode));
            }
        }

        if(m_forecast.error == 1 || !ks_wifi_is_connected()) {
            for(int i = 0; i < 3; i++){
                m_panels[i].set_day_label("");
                m_panels[i].set_temperature_label(0, 0);
                m_panels[i].set_weather_icon("");
            }
        }
    }
    auto MainScreen::show_settings() -> void {
        lv_obj_clear_flag(m_settingsMain, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(m_settingsMain);
    }
}