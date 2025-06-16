#pragma once

#include "Screen.hpp"

extern "C" {
    #include "lvgl.h"
    #include "Weather.h"
}

#include "WeatherPanel.hpp"
#include "TimeWidget.hpp"
#include "WeatherWidget.hpp"
#include "HomeWidget.hpp"

namespace ws {
    class MainScreen : public Screen {
    public:
        MainScreen(SharedData& data);
        virtual ~MainScreen() = default;

        virtual auto update() -> void;
        auto refresh() -> void;
        auto show_settings() -> void;

    private:
        TimeWidget m_timeWidget;
        WeatherPanel m_panels[3];
        WeatherWidget m_weatherWidget;
        HomeWidget m_homeWidget;

        ks_weather_forecast_t m_forecast{};

        lv_obj_t* m_settingsMain{};
        lv_obj_t* m_settingsIPLabel{};
        lv_obj_t* m_settingsCloseButton{};
        lv_obj_t* m_settingsCloseButtonLabel{};
        lv_obj_t* m_settingsDisconnectButton{};
        lv_obj_t* m_settingsDisconnectButtonLabel{};

        bool m_lastWifi{};
    };
}