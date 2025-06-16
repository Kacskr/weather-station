#pragma once

#include <string>

struct Credentials {
    char ssid[32];
    char password[64];
};

extern void ks_ap_start();
extern void ks_ap_stop();
extern bool ks_ap_entered_credentials();
extern Credentials ks_ap_get_credentials();
extern std::string ks_ap_get_ip();
