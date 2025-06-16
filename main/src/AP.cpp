#include "AP.hpp"
#include "Util.hpp"

#include "esp_http_server.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "lwip/ip4_addr.h"

#include <string>

#define ESP_WIFI_SSID      "Weather Station"
#define ESP_WIFI_PASS      "12345678"
#define MAX_STA_CONN       4

static bool s_enteredCredentials = 0;
static Credentials s_credentials;
static esp_netif_t* s_apNetif = 0;
static httpd_handle_t s_server = 0;

static esp_err_t root_get_handler(httpd_req_t *req) {
    std::string page = ws::load_page("/storage/wifi.html");
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, page.c_str(), HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t connect_post_handler(httpd_req_t *req) {
    char buf[101];
    int ret = httpd_req_recv(req, buf, std::min(req->content_len, sizeof(buf)));
    buf[ret] = '\0';

    char ssid[32] = {0}, pass[64] = {0};
    sscanf(buf, "ssid=%31[^&]&password=%63s", ssid, pass);
    ESP_LOGI("ap", "Received SSID: %s, Password: %s", ssid, pass);

    memcpy(s_credentials.ssid, ssid, 32);
    memcpy(s_credentials.password, pass, 64);
    s_enteredCredentials = 1;
    
    return ESP_OK;
}

void ks_ap_start() {
    if(s_apNetif) return;
    s_apNetif = esp_netif_create_default_wifi_ap();

    wifi_config_t wifi_config = {};

    strncpy((char *)wifi_config.ap.ssid, ESP_WIFI_SSID, sizeof(wifi_config.ap.ssid));
    strncpy((char *)wifi_config.ap.password, ESP_WIFI_PASS, sizeof(wifi_config.ap.password));
    wifi_config.ap.ssid_len = strlen(ESP_WIFI_SSID);
    wifi_config.ap.max_connection = MAX_STA_CONN;
    wifi_config.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;

    if (strlen(ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    esp_wifi_set_mode(WIFI_MODE_AP);
    esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    esp_wifi_start();

    ESP_LOGI("ap", "SoftAP started. SSID: %s", ESP_WIFI_SSID);

    // start webs_server

    if(s_server == 0) {
        httpd_config_t config = HTTPD_DEFAULT_CONFIG();
        config.stack_size = 16384;
        httpd_start(&s_server, &config);

        httpd_uri_t root = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = root_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(s_server, &root);

        httpd_uri_t connect = {
            .uri       = "/connect",
            .method    = HTTP_POST,
            .handler   = connect_post_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(s_server, &connect);
    }
}
void ks_ap_stop() {
    httpd_stop(s_server);
    s_server = 0;

    esp_wifi_stop();

    if(s_apNetif) {
        esp_netif_destroy_default_wifi(s_apNetif);
        s_apNetif = 0;
    }
}
bool ks_ap_entered_credentials() {
    const bool b = s_enteredCredentials;
    s_enteredCredentials = 0;
    return b;
}
Credentials ks_ap_get_credentials() {
    return s_credentials;
}
std::string ks_ap_get_ip() {
    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(s_apNetif, &ip_info);
    
    char ip_str[16];
    memset(ip_str, 0, sizeof(ip_str));
    ip4addr_ntoa_r((const ip4_addr_t*)&ip_info.ip, ip_str, sizeof(ip_str));

    return std::string(ip_str);
}