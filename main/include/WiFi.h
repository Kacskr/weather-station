#ifndef WIFI_H_INCLUDED
#define WIFI_H_INCLUDED

#include <stdbool.h>

#include "esp_wifi.h"

extern bool ks_wifi_init();
extern void ks_wifi_deinit();
extern bool ks_wifi_scan();
extern bool ks_wifi_stop_scan();
extern bool ks_wifi_connect(const char* ssid, const char* password);
extern bool ks_wifi_disconnect();
extern bool ks_wifi_is_connected();
extern bool ks_wifi_ap_changed();
extern wifi_ap_record_t* ks_wifi_get_records();
extern int ks_wifi_records_size();
extern bool ks_wifi_scan_done();
extern bool ks_wifi_is_scanning();
extern char* ks_wifi_get_ip();

#endif // WIFI_H_INCLUDED