#include "Screen/WifiScreen.hpp"

extern "C" {
    #include "WiFi.h"
    #include "NVS.h"
}
#include "AP.hpp"

namespace ws {
    static constexpr int UPDATE_PERIOD = 1'000'000;

    WifiScreen::WifiScreen(SharedData& data)
    :   Screen(data) {
        m_homeWidget.create(get());
        m_homeWidget.set_position(85, 180);
        m_homeWidget.set_size(70, 132);

        m_ipLabel = lv_label_create(get());
        lv_obj_set_style_text_color(m_ipLabel, lv_color_hex(0x9a9a9a), 0);
        lv_obj_align(m_ipLabel, LV_ALIGN_TOP_MID, 0, 40);

        m_lastUpdate = esp_timer_get_time();
    }

    auto WifiScreen::load() -> void {
        Screen::load();
        ks_wifi_deinit();
        ks_ap_start();
    }
    auto WifiScreen::unload() -> void {

    }
    auto WifiScreen::update() -> void {
        m_homeWidget.update(get_data().temperature, get_data().humidity, get_data().pressure);
        if(!ks_wifi_is_connected()){
            if(ks_ap_entered_credentials()){
                ks_ap_stop();
                ks_wifi_init();
                Credentials cred = ks_ap_get_credentials();
                ks_wifi_connect(cred.ssid, cred.password);
                if(!ks_wifi_is_connected()) {
                    ks_wifi_deinit();
                    ks_ap_start();
                }

                ks_nvs_save_string("ssid", cred.ssid);
                ks_nvs_save_string("password", cred.password);
            }
            
            if(esp_timer_get_time() > m_lastUpdate + UPDATE_PERIOD) {
                const std::string ip = ks_ap_get_ip().c_str();
                lv_label_set_text(m_ipLabel, ip.c_str());
            }
        }
    }
}