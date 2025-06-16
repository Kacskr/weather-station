#pragma once

#include "Config.hpp"

extern "C"{ 
    #include "lvgl.h"
}

#include "SharedData.hpp"

namespace ws {
    class Screen {
    public:
        Screen(SharedData& data)
            :   m_sharedData(data) {
            m_screen = lv_obj_create(0);
            lv_obj_set_style_bg_color(m_screen, lv_color_hex(BACKGROUND_COLOR), LV_STATE_DEFAULT);
            lv_obj_set_scrollbar_mode(m_screen, LV_SCROLLBAR_MODE_OFF);
            lv_obj_clear_flag(m_screen, LV_OBJ_FLAG_SCROLLABLE);
        }
        virtual ~Screen() = default;

        virtual auto update() -> void {};
        
        virtual auto load() -> void {
            lv_scr_load(m_screen);
        }
        virtual auto unload() -> void {};

        auto get() -> lv_obj_t* {
            return m_screen;
        }

    protected:
        auto get_data() -> SharedData& {
            return m_sharedData;
        }

    private:
        lv_obj_t* m_screen{};
        SharedData& m_sharedData;
    };
}