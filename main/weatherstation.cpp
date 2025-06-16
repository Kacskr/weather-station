#include <iostream>

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "esp_timer.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_ili9341.h"
#include "esp_lcd_touch_xpt2046.h"
#include "esp_heap_caps.h"

extern "C" {
    #include "lvgl.h"
    #include "WiFi.h"
    #include "NVS.h"
}
#include "AP.hpp"

#include "Hardware/I2CBus.hpp"
#include "Hardware/Pins.hpp"

#include "Hardware/SPIBus.hpp"
#include "Hardware/Display/ILI9341.hpp"
#include "Hardware/TouchController/XPT2046.hpp"

#include "LvglManager.hpp"
#include "Screen/Screen.hpp"
#include "Screen/MainScreen.hpp"
#include "Screen/WifiScreen.hpp"
#include "SharedData.hpp"
#include "WeatherServer.hpp"

#include <time.h>
#include <sys/time.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "esp_http_server.h"
#include "esp_spiffs.h"

constexpr int MEASUREMENT_PERIOD = 1'000'000;
constexpr int WEATHER_REFRESH_PERIOD = 300'000'000;
constexpr int INSERT_PERIOD = 60'000'000;
constexpr int PRESS_TIME = 5'000'000;

/*
-Add display dimming at night and lighting up after pressing it. 
-Add option to adjust screen brightness
*/

static void main_task(void*){
    ws::reset_pins();
    ws::init_spi(ws::D_MOSI, ws::D_MISO, ws::D_CLK, SPI2_HOST);
    
    esp_vfs_spiffs_conf_t config2 = {
        .base_path = "/storage",
        .partition_label = 0,
        .max_files = 5,
        .format_if_mount_failed = false,
    };

    esp_vfs_spiffs_register(&config2);

    ks_nvs_init();

    esp_netif_init();
    wifi_init_config_t wifiInitConfig = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifiInitConfig);
    esp_event_loop_create_default();

    ks_wifi_init();

    ks_value_t ssid = ks_nvs_load_string("ssid");
    ks_value_t password = ks_nvs_load_string("password");

    if(ssid.error == 0 && password.error == 0) {
        ks_wifi_connect(ssid.string, password.string);
        free(ssid.string);
        free(password.string);
    }

    if(!ks_wifi_is_connected()){
        ks_wifi_deinit();
    }

    ws::ILI9341 ili;
    ili.init(320, 240, ws::D_DC, ws::D_RST, ws::D_CS, ws::D_LED, SPI2_HOST);
    
    ws::XPT2046 xpt;
    xpt.init(320, 240, ws::T_INT, ws::T_CS, SPI2_HOST);
    
    lv_init();

    ws::LvglManager lvglManager;
    lvglManager.init(320, 240, SPI2_HOST, ili, xpt);

    ws::I2CBus i2cBus;
    i2cBus.init(ws::WS_SDA, ws::WS_SCL);
    
    ws::BME280 bme;
    bme.init(i2cBus);
    
    ws::SharedData data;
    ws::MainScreen screen(data);
    ws::WifiScreen wifiScreen(data);
    
    int64_t lastMeasurement = esp_timer_get_time();
    int64_t lastRefresh = esp_timer_get_time();
    int64_t lastInsert = esp_timer_get_time();
    int64_t pressTimer = esp_timer_get_time();
    bool pressFirstTime = 1;
    bool pressOnce = 1;

    bool lastWifiState = !ks_wifi_is_connected();

    ws::WeatherServer server;

    while(1){
        xpt.read_data();
        
        if(ws::LvglManager::s_pressed){
            if(pressFirstTime) {
                pressTimer = esp_timer_get_time();
                pressFirstTime = 0;
            }

            if(esp_timer_get_time() > pressTimer + PRESS_TIME) {
                if(pressOnce) {
                    screen.show_settings();
                    pressOnce = 0;
                }
            }
        }
        else {
            pressFirstTime = 1;
            pressOnce = 1;
        }

        struct timeval tv;
        gettimeofday(&tv, NULL);
        localtime_r(&tv.tv_sec, &data.timeinfo);

        if(esp_timer_get_time() >= lastRefresh + WEATHER_REFRESH_PERIOD || server.has_location_changed()) {
            screen.refresh();
            lastRefresh = esp_timer_get_time();
        }

        if(esp_timer_get_time() >= lastMeasurement + MEASUREMENT_PERIOD){
            data.temperature = bme.read_temperature();
            data.humidity = bme.read_humidity();
            data.pressure = bme.read_pressure();
            struct timeval tv;
            gettimeofday(&tv, NULL);
            server.set_measurement({tv.tv_sec, data.temperature, data.humidity, data.pressure});
            if(ks_wifi_is_connected()) {
                if(esp_timer_get_time() >= lastInsert + INSERT_PERIOD){
                    server.insert_measurement({tv.tv_sec, data.temperature, data.humidity, data.pressure});
                    lastInsert = esp_timer_get_time();
                }
            }
            lastMeasurement = esp_timer_get_time();
        }
        
        if(lastWifiState != ks_wifi_is_connected()) {
            if(ks_wifi_is_connected()) {
                server.start();
                wifiScreen.unload();
                screen.load();
                screen.refresh();
            } else {
                server.stop();
                wifiScreen.load();
            }

            lastWifiState = ks_wifi_is_connected();
        }

        if(ks_wifi_is_connected()) {
            screen.update();        
        } else {
            wifiScreen.update();
        }

        lv_timer_handler();
        vTaskDelay(1);
    }
}

extern "C" void app_main(void){ 
    xTaskCreatePinnedToCore(main_task, "main_task", 16384, NULL, 0, NULL, 1);
}
