#pragma once

#include "Screen.hpp"

extern "C" {
    #include "lvgl.h"
}

#include "esp_timer.h"
#include "HomeWidget.hpp"

namespace ws {
    class WifiScreen : public Screen {
    public:
        WifiScreen(SharedData& data);
        virtual ~WifiScreen() = default;

        virtual auto load() -> void override;
        virtual auto unload() -> void override;
        virtual auto update() -> void;

    private:
        lv_obj_t* m_ipLabel{};
        HomeWidget m_homeWidget;

        int64_t m_lastUpdate{};
    };
}