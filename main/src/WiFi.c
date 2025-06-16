#include "WiFi.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include <stdio.h>
#include <string.h>
#include "esp_sntp.h"
#include <time.h>
#include <sys/time.h>

#define WIFI_CONNECTED_BIT BIT0 
#define WIFI_FAIL_BIT BIT1

static const char* TAG = "WIFI";

static EventGroupHandle_t WifiEventGroup = 0;

static bool s_started = 0;
static bool s_isConnected = 0;
static bool s_apChanged = 0;
static char s_ip[16];
static wifi_ap_record_t ap_records[10];
uint16_t ap_count = 0;
static bool s_scanDone = 0;
static bool s_isScanning = 0;
static esp_netif_t* ap_netif = 0;

#define MAX_AP (10)

static void obtain_time(){
    setenv("TZ", "CET-1CEST,M3.5.0/2,M10.5.0/3", 1);
    tzset();

    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();

    time_t now = 0;
    struct tm timeinfo;
    int retries = 10;
    do{
        time(&now);
        localtime_r(&now, &timeinfo);
        vTaskDelay(pdMS_TO_TICKS(500));
    } while(timeinfo.tm_year < 100 && retries-- > 0);
}
static void event_wifi(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData){
    switch(eventID){
        case WIFI_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case WIFI_EVENT_STA_DISCONNECTED: {
            wifi_event_sta_disconnected_t* disconnected = (wifi_event_sta_disconnected_t*)eventData;
            if(disconnected->reason == WIFI_REASON_AUTH_FAIL){
                printf("Wrong password\n");
            } else {
                printf("idk, %i\n", disconnected->reason);
            }
            ks_wifi_disconnect();
            xEventGroupSetBits(WifiEventGroup, WIFI_FAIL_BIT);
            s_isConnected = 0;
        }   break;
        case WIFI_EVENT_SCAN_DONE:
            esp_wifi_scan_get_ap_num(&ap_count);
            if (ap_count == 0) {
                break;
            }

            if(ap_count > MAX_AP) ap_count = MAX_AP;
            esp_wifi_scan_get_ap_records(&ap_count, ap_records);
            s_scanDone = 1;
            s_isScanning = 0;
            break;
        default:
            break;
    }
}
static void event_ip(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData){
    switch(eventID){
        case IP_EVENT_STA_GOT_IP: {
            ip_event_got_ip_t* event = (ip_event_got_ip_t*)eventData;
            ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
            sprintf(s_ip, IPSTR, IP2STR(&event->ip_info.ip));
            s_isConnected = 1;
            if(!esp_sntp_enabled()){
                obtain_time();
            }
            xEventGroupSetBits(WifiEventGroup, WIFI_CONNECTED_BIT);
        }   break;
    }
}

static void event_handler(void* arg, esp_event_base_t eventBase, int32_t eventID, void* eventData){
    if(eventBase == WIFI_EVENT) event_wifi(arg, eventBase, eventID, eventData);
    else if(eventBase == IP_EVENT) event_ip(arg, eventBase, eventID, eventData);
}
bool ks_wifi_ap_changed(){
    bool b = s_apChanged;
    s_apChanged = 0;
    return b;
}

esp_event_handler_instance_t instanceAnyId = 0;    
esp_event_handler_instance_t instanceGotIp = 0;

wifi_ap_record_t* ks_wifi_get_records(){
    return ap_records;
}
int ks_wifi_records_size(){
    return ap_count;
}
bool ks_wifi_scan_done(){
    bool b = s_scanDone;
    s_scanDone = 0;
    return b;
}

bool ks_wifi_init(){
    if(ap_netif) return 1;
    ap_netif = esp_netif_create_default_wifi_sta();
    esp_wifi_set_mode(WIFI_MODE_STA);

    if(WifiEventGroup == 0) {
        WifiEventGroup = xEventGroupCreate();
        esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, 0, &instanceAnyId);    
        esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, 0, &instanceGotIp);
    }

    return 1;
}
void ks_wifi_deinit(){
    if(ap_netif) {
        esp_netif_destroy_default_wifi(ap_netif);
        ap_netif = 0;
    }
}
bool ks_wifi_scan(){
    if(!s_started){
        s_started = 1;
        wifi_config_t wifiConfig = {
            .sta = {
                .ssid = "",
                .password = "",
                .threshold.authmode = WIFI_AUTH_WPA2_PSK,
                .pmf_cfg = {
                    .capable = true,
                    .required = false,
                },
                .scan_method = WIFI_ALL_CHANNEL_SCAN,
                .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
            },
        };  
        esp_wifi_set_config(WIFI_IF_STA, &wifiConfig);
        esp_wifi_start();
        esp_wifi_disconnect();
    }

    wifi_scan_config_t scan_config = {
        .ssid = NULL,             // NULL means scan all SSIDs
        .bssid = NULL,            // NULL means scan all BSSIDs
        .channel = 0,             // 0 means scan all channels
        .show_hidden = false,     // Don't show hidden networks
        .scan_type = WIFI_SCAN_TYPE_ACTIVE,
        .scan_time.active = {
            .min = 100,           // Minimum active scan time per channel (ms)
            .max = 500,           // Maximum active scan time per channel (ms)
        },
    };

    s_isScanning = 1;
    return ESP_OK == esp_wifi_scan_start(&scan_config, false);
}
bool ks_wifi_stop_scan(){
    esp_wifi_scan_stop();
    s_isScanning = 0;
    return 1;
}
bool ks_wifi_is_scanning(){
    return s_isScanning;
}

bool ks_wifi_connect(const char* ssid, const char* password){
    bool success = 0;
    s_isConnected = 0;

    wifi_config_t wifiConfig = {
        .sta = {
            .ssid = "",
            .password = "",
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false,
            },
            .scan_method = WIFI_ALL_CHANNEL_SCAN,
            .sort_method = WIFI_CONNECT_AP_BY_SIGNAL,
        },
    };  

    strncpy((char*)wifiConfig.sta.ssid, ssid, sizeof(wifiConfig.sta.ssid) - 1);
    strncpy((char*)wifiConfig.sta.password, password, sizeof(wifiConfig.sta.password) - 1);

    esp_wifi_set_config(WIFI_IF_STA, &wifiConfig);
    if(s_started == 0){
        esp_wifi_start();
        s_started = 1;
    }

    ESP_LOGI(TAG, "Wifi initialization finished.");

    xEventGroupClearBits(WifiEventGroup, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT);
    EventBits_t bits = xEventGroupWaitBits(WifiEventGroup, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, pdMS_TO_TICKS(10000));

    if(bits & WIFI_CONNECTED_BIT){
        ESP_LOGI(TAG, "Connected to WiFi.");
        success = 1;
    }
    else if(bits & WIFI_FAIL_BIT){
        ESP_LOGI(TAG, "Failed to connect to WiFi.");
    } 
    else {
        ESP_LOGI(TAG, "Unexpected event.");
    }

    if(!success){
        ks_wifi_disconnect();
    }

    s_apChanged = 1;

    return success;
}
bool ks_wifi_disconnect(){
    if(esp_sntp_enabled()){
        esp_sntp_stop();
    }
    esp_err_t ret = esp_wifi_disconnect();
    if (ret != ESP_OK) {
        return 0;
    }
    esp_wifi_stop();
    s_started = 0;
    wifi_config_t wifiConfig = {};
    esp_wifi_set_config(WIFI_IF_STA, &wifiConfig);
    ESP_LOGI(TAG, "Disconnected from WiFi.");

    return 1;
}
bool ks_wifi_is_connected(){
    return s_isConnected;
}
char* ks_wifi_get_ip(){
    return s_ip;
}