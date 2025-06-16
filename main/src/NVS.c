#include "NVS.h"
#include "nvs_flash.h"

esp_err_t ks_nvs_init(){
    esp_err_t ret = nvs_flash_init();
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND){
        nvs_flash_erase();
        nvs_flash_init();
    }
    
    return ret;
}
void ks_nvs_save_int32(const char* name, const int32_t x){
    nvs_handle_t nvsHandle;
    esp_err_t err = nvs_open("ks_settings", NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK) {
        return;
    }

    err = nvs_set_i32(nvsHandle, name, x);
    if(err != ESP_OK){
        nvs_close(nvsHandle);
        return;
    }

    nvs_commit(nvsHandle);

    nvs_close(nvsHandle);
}
ks_value_t ks_nvs_load_int32(const char* name){
    ks_value_t ret;
    ret.error = 1;
    ret.value = 0;

    nvs_handle_t nvsHandle;
    esp_err_t err = nvs_open("ks_settings", NVS_READONLY, &nvsHandle);
    if (err != ESP_OK) {
        return ret;
    }

    int32_t val = 0;
    err = nvs_get_i32(nvsHandle, name, &val);

    if(err == ESP_OK){
        ret.value = val;
        ret.error = 0;
    }

    nvs_close(nvsHandle);
    return ret;
}
void ks_nvs_save_string(const char* name, const char* string) {
    nvs_handle_t nvsHandle;
    esp_err_t err = nvs_open("ks_settings", NVS_READWRITE, &nvsHandle);
    if (err != ESP_OK) {
        return;
    }

    err = nvs_set_str(nvsHandle, name, string);
    if(err != ESP_OK){
        nvs_close(nvsHandle);
        return;
    }

    nvs_commit(nvsHandle);

    nvs_close(nvsHandle);
}
ks_value_t ks_nvs_load_string(const char* name) {
    ks_value_t ret;
    ret.error = 1;
    ret.string = 0;

    nvs_handle_t nvsHandle;
    esp_err_t err = nvs_open("ks_settings", NVS_READONLY, &nvsHandle);
    if (err != ESP_OK) {
        return ret;
    }

    size_t size = 0;
    err = nvs_get_str(nvsHandle, name, 0, &size);

    if(err == ESP_OK){
        ret.string = malloc(size);
        err = nvs_get_str(nvsHandle, name, ret.string, &size);
        if(err == ESP_OK){
            ret.error = 0;
        } else {
            free(ret.string);
        }
    }

    nvs_close(nvsHandle);
    return ret;
}