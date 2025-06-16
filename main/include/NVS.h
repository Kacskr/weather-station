#ifndef NVS_H_INCLUDED
#define NVS_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>
#include "esp_err.h"

typedef struct{
    bool error;
    int32_t value;
    char* string;
} ks_value_t;

extern esp_err_t ks_nvs_init();
extern void ks_nvs_save_int32(const char* name, const int32_t x);
extern ks_value_t ks_nvs_load_int32(const char* name);
extern void ks_nvs_save_string(const char* name, const char* string);
extern ks_value_t ks_nvs_load_string(const char* name);

#endif // NVS_H_INCLUDED