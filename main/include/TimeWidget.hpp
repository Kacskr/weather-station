#pragma once

extern "C" {
    #include "lvgl.h"
}

#include <time.h>
#include <sys/time.h>

namespace ws {
    class TimeWidget {
    public:
        TimeWidget() = default;
        ~TimeWidget() = default;

        auto create(lv_obj_t* src) -> void;
        auto update(struct tm& timeinfo) -> void;

    private:
        lv_obj_t* m_timeLabel{};
        lv_obj_t* m_dateLabel{};
    };
}