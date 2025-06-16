#pragma once

extern "C" {
    #include "lvgl.h"
}

#include "Hardware/Display/SPIDisplay.hpp"
#include "Hardware/TouchController/SPITouchController.hpp"

#include "driver/spi_master.h"
#include "esp_timer.h"

#define EXAMPLE_LCD_H_RES              240
#define EXAMPLE_LCD_V_RES              320

#define EXAMPLE_LVGL_DRAW_BUF_LINES    20 // number of display lines in each draw buffer
#define EXAMPLE_LVGL_TICK_PERIOD_MS    2
#define EXAMPLE_LVGL_TASK_MAX_DELAY_MS 500
#define EXAMPLE_LVGL_TASK_MIN_DELAY_MS 1000 / CONFIG_FREERTOS_HZ

namespace ws {
    class LvglManager {
    public:
        LvglManager() = default;
        ~LvglManager() = default;

        auto init(const int width, const int height, spi_host_device_t device, SPIDisplay& display, SPITouchController& tc) -> void {
            m_display = lv_display_create(height, width);

            size_t draw_buffer_sz = height * 20 * sizeof(lv_color16_t);
            void *buf1 = spi_bus_dma_memory_alloc(device, draw_buffer_sz, 0);
            void *buf2 = spi_bus_dma_memory_alloc(device, draw_buffer_sz, 0);

            lv_display_set_buffers(m_display, buf1, buf2, draw_buffer_sz, LV_DISPLAY_RENDER_MODE_PARTIAL);
            lv_display_set_user_data(m_display, display.get_panel_handle());
            lv_display_set_color_format(m_display, LV_COLOR_FORMAT_RGB565);
            lv_display_set_flush_cb(m_display, example_lvgl_flush_cb);

            const esp_timer_create_args_t lvgl_tick_timer_args = {
                .callback = &example_increase_lvgl_tick,
                .name = "lvgl_tick"
            };

            esp_timer_create(&lvgl_tick_timer_args, &m_lvglTickTimer);
            esp_timer_start_periodic(m_lvglTickTimer, 2000);

            const esp_lcd_panel_io_callbacks_t cbs = {
                .on_color_trans_done = example_notify_lvgl_flush_ready,
            };

            esp_lcd_panel_io_register_event_callbacks(display.get_io_handle(), &cbs, m_display);

            lv_indev_t *indev;
            indev = lv_indev_create();
            lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
            lv_indev_set_display(indev, m_display);
            lv_indev_set_user_data(indev, tc.get_handle());
            lv_indev_set_read_cb(indev, example_lvgl_touch_cb);
        }

        auto get_display() -> lv_display_t* {
            return m_display;
        }

    private:
        static bool example_notify_lvgl_flush_ready(esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t *edata, void *user_ctx)
        {
            lv_display_t *disp = (lv_display_t *)user_ctx;
            lv_display_flush_ready(disp);
            return false;
        }
        static void example_increase_lvgl_tick(void *arg)
        {
            /* Tell LVGL how many milliseconds has elapsed */
            lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
        }

        static void example_lvgl_port_update_callback(lv_display_t *disp)
        {
            esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)lv_display_get_user_data(disp);
            lv_display_rotation_t rotation = (lv_display_rotation_t)lv_display_get_rotation(disp);

            switch (rotation) {
            case LV_DISPLAY_ROTATION_0:
                // Rotate LCD display
                esp_lcd_panel_swap_xy(panel_handle, false);
                esp_lcd_panel_mirror(panel_handle, true, false);
                break;
            case LV_DISPLAY_ROTATION_90:
                // Rotate LCD display
                esp_lcd_panel_swap_xy(panel_handle, true);
                esp_lcd_panel_mirror(panel_handle, true, true);
                break;
            case LV_DISPLAY_ROTATION_180:
                // Rotate LCD display
                esp_lcd_panel_swap_xy(panel_handle, false);
                esp_lcd_panel_mirror(panel_handle, false, true);
                break;
            case LV_DISPLAY_ROTATION_270:
                // Rotate LCD display
                esp_lcd_panel_swap_xy(panel_handle, true);
                esp_lcd_panel_mirror(panel_handle, false, false);
                break;
            }
        }
        static void example_lvgl_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
        {
            example_lvgl_port_update_callback(disp);
            esp_lcd_panel_handle_t panel_handle = (esp_lcd_panel_handle_t)lv_display_get_user_data(disp);
            int offsetx1 = area->x1;
            int offsetx2 = area->x2;
            int offsety1 = area->y1;
            int offsety2 = area->y2;
            // because SPI LCD is big-endian, we need to swap the RGB bytes order
            lv_draw_sw_rgb565_swap(px_map, (offsetx2 + 1 - offsetx1) * (offsety2 + 1 - offsety1));
            // copy a buffer's content to a specific area of the display
            esp_lcd_panel_draw_bitmap(panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, px_map);
        }

        static void example_lvgl_touch_cb(lv_indev_t *indev, lv_indev_data_t *data)
        {
            uint16_t touchpad_x[1] = {0};
            uint16_t touchpad_y[1] = {0};
            uint8_t touchpad_cnt = 0;

            esp_lcd_touch_handle_t touch_pad = (esp_lcd_touch_handle_t)lv_indev_get_user_data(indev);
            esp_lcd_touch_read_data(touch_pad);
            /* Get coordinates */
            bool touchpad_pressed = esp_lcd_touch_get_coordinates(touch_pad, touchpad_x, touchpad_y, NULL, &touchpad_cnt, 1);

            if (touchpad_pressed && touchpad_cnt > 0) {
                data->point.x = (int16_t)((touchpad_x[0] - 25.0f) * 320.0f / 280.0f);
                data->point.y = (int16_t)((touchpad_y[0] - 10.0f) * 240.0f / 210.0f);
                data->state = LV_INDEV_STATE_PRESSED;
                s_pressed = 1;
            } else {
                data->state = LV_INDEV_STATE_RELEASED;
                s_pressed = 0;
            }
        }

    public:
        static bool s_pressed;

    private:
        lv_display_t* m_display{};
        esp_timer_handle_t m_lvglTickTimer{};
    };
}