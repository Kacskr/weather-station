#include "Weather.h"
#include "cJSON.h"
#include "esp_http_client.h"
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "NVS.h"

#define WINDY 300

#define MIN(a, b) (a > b ? a : b)

const char* ks_days[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

static esp_http_client_handle_t s_httpClient = 0;
static esp_http_client_handle_t s_httpForecastClient = 0;
static esp_http_client_handle_t s_httpExtendedClient = 0;
static ks_weather_t s_currentWeather;

ks_weather_t ks_weather_update_weather(){
    ks_weather_t retWeather;
    memset(&retWeather, 0, sizeof(ks_weather_t));
    retWeather.error = 1;

    ks_value_t x = ks_nvs_load_int32("lon");
    ks_value_t y = ks_nvs_load_int32("lat");

    char url[512] = {};
    float lon = 0.0f, lat = 0.0f;

    if(x.error == 0 && y.error == 0){
        memcpy(&lon, &x.value, sizeof(float));
        memcpy(&lat, &y.value, sizeof(float));
    }

    snprintf(url, sizeof(url), "https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current=temperature_2m,relative_humidity_2m,apparent_temperature,is_day,weather_code,surface_pressure,wind_speed_10m,wind_direction_10m&timezone=Europe%%2FBerlin", lat, lon);

    if(s_httpClient == 0){
        esp_http_client_config_t clientConfig = {
            .url = url,
            .method = HTTP_METHOD_GET,
            .keep_alive_enable = 1,
            .buffer_size = 0,
            .timeout_ms = 5000,
        };
        s_httpClient = esp_http_client_init(&clientConfig);
    } else {
        esp_http_client_set_url(s_httpClient, url);
    }

    if(esp_http_client_open(s_httpClient, 0) != ESP_OK){
        printf("UpdateWeather: open error\n");
        return retWeather;
    }
    if(esp_http_client_fetch_headers(s_httpClient) < 0){
        printf("UpdateWeather: fetch error\n");
        esp_http_client_close(s_httpClient);
        return retWeather;
    }

    char* buffer = calloc(1536 + 1, 1);
    const int len = esp_http_client_read(s_httpClient, buffer, 1536);
    if(len == -1){
        free(buffer);
        printf("UpdateWeather: read error\n");
        esp_http_client_close(s_httpClient);
        return retWeather;
    }
    buffer[len] = '\0';
    if(esp_http_client_close(s_httpClient) != ESP_OK){
        free(buffer);
        printf("UpdateWeather: close error\n");
        esp_http_client_close(s_httpClient);
        return retWeather;
    }

    cJSON* root = cJSON_Parse(buffer);
    if (root == NULL) {
        free(buffer);
        printf("UpdateWeather: error parsing JSON\n");
        return retWeather;
    }

    cJSON* currentWeather = cJSON_GetObjectItem(root, "current");
    if(currentWeather == NULL){
        free(buffer);
        printf("UpdateWeather: error getting current weather\n");
        return retWeather;
    }

    cJSON* temperature = cJSON_GetObjectItem(currentWeather, "temperature_2m");
    if(!cJSON_IsNumber(temperature)){
        free(buffer);
        printf("UpdateWeather: temperature is not a number\n");
        return retWeather;
    }
    cJSON* weatherCode = cJSON_GetObjectItem(currentWeather, "weather_code");
    if(!cJSON_IsNumber(weatherCode)){
        free(buffer);
        printf("UpdateWeather: weatherCode is not a number\n");
        return retWeather;
    }
    cJSON* windSpeed = cJSON_GetObjectItem(currentWeather, "wind_speed_10m");
    if(!cJSON_IsNumber(windSpeed)){
        free(buffer);
        printf("UpdateWeather: windSpeed is not a number\n");
        return retWeather;
    }
    cJSON* isDay = cJSON_GetObjectItem(currentWeather, "is_day");
    if(!cJSON_IsNumber(isDay)){
        free(buffer);
        printf("UpdateWeather: isDay is not a number\n");
        return retWeather;
    }
    cJSON* humidity = cJSON_GetObjectItem(currentWeather, "relative_humidity_2m");
    if(!cJSON_IsNumber(humidity)){
        free(buffer);
        printf("UpdateWeather: humidity");
        return retWeather;
    }
    cJSON* apparentTemperature = cJSON_GetObjectItem(currentWeather, "apparent_temperature");
    if(!cJSON_IsNumber(apparentTemperature)){
        free(buffer);
        printf("UpdateWeather: apparentTemperature");
        return retWeather;
    }
    cJSON* surfacePressure = cJSON_GetObjectItem(currentWeather, "surface_pressure");
    if(!cJSON_IsNumber(surfacePressure)){
        free(buffer);
        printf("UpdateWeather: surfacePressure");
        return retWeather;
    }
    cJSON* windDirection = cJSON_GetObjectItem(currentWeather, "wind_direction_10m");
    if(!cJSON_IsNumber(windDirection)){
        free(buffer);
        printf("UpdateWeather: windDirection");
        return retWeather;
    }

    retWeather.temperature = (int)(temperature->valuedouble * 10.0);
    retWeather.weatherCode = weatherCode->valueint;
    retWeather.windSpeed = (int)(windSpeed->valuedouble * 10.0);
    retWeather.isDay = (bool)(isDay->valueint);
    retWeather.humidity = humidity->valueint;
    retWeather.apparentTemperature = (int)(apparentTemperature->valuedouble * 10.0);
    retWeather.surfacePressure = (int)(surfacePressure->valuedouble * 10.0);
    retWeather.windDirection = windDirection->valueint;

    s_currentWeather = retWeather;

    cJSON_Delete(root);
    retWeather.error = 0;
    free(buffer);
    return retWeather;
}
ks_weather_t ks_weather_get_current_weather(){
    return s_currentWeather;
}
ks_weather_forecast_t ks_weather_get_forecast(){
    ks_weather_forecast_t retForecast;
    memset(retForecast.weatherData, 0, sizeof(retForecast.weatherData));
    retForecast.error = 1;

    ks_value_t x = ks_nvs_load_int32("lon");
    ks_value_t y = ks_nvs_load_int32("lat");

    char url[512] = {};
    float lon = 0.0f, lat = 0.0f;

    if(x.error == 0 && y.error == 0){
        memcpy(&lon, &x.value, sizeof(float));
        memcpy(&lat, &y.value, sizeof(float));
    }

    snprintf(url, sizeof(url), "https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&daily=weather_code,temperature_2m_max,temperature_2m_min,sunrise,sunset&timezone=Europe%%2FBerlin", lat, lon);

    if(s_httpForecastClient == 0){
        esp_http_client_config_t clientConfig = {
            .url = url,
            .method = HTTP_METHOD_GET,
            .keep_alive_enable = 1,
            .buffer_size = 0,
            .timeout_ms = 5000,
        };
        s_httpForecastClient = esp_http_client_init(&clientConfig);
    } else {
        esp_http_client_set_url(s_httpForecastClient, url);
    }

    if(esp_http_client_open(s_httpForecastClient, 0) != ESP_OK){
        printf("Forecast: open error\n");
        return retForecast;
    }
    if(esp_http_client_fetch_headers(s_httpForecastClient) < 0){
        printf("Forecast: fetch error\n");
        esp_http_client_close(s_httpForecastClient);
        return retForecast;
    }

    char* buffer = calloc(1536 + 1, 1);
    const int len = esp_http_client_read(s_httpForecastClient, buffer, 1536);
    if(len == -1){
        free(buffer);
        printf("Forecast: read error\n");
        esp_http_client_close(s_httpForecastClient);
        return retForecast;
    }
    buffer[len] = '\0';
    if(esp_http_client_close(s_httpForecastClient) != ESP_OK){
        free(buffer);
        printf("Forecast: close error\n");
        return retForecast;
    }

    cJSON* root = cJSON_Parse(buffer);
    if(root == NULL){
        free(buffer);
        printf("Forecast: error parsing JSON\n");
        return retForecast;
    }

    cJSON* daily = cJSON_GetObjectItem(root, "daily");
    if(daily == NULL){
        free(buffer);
        printf("Forecast: daily error");
        return retForecast;
    }

    cJSON* weatherCode = cJSON_GetObjectItem(daily, "weather_code");
    if(weatherCode == NULL || !cJSON_IsArray(weatherCode) || cJSON_GetArraySize(weatherCode) < 4){
        free(buffer);
        printf("Forecast: weatherCode error");
        return retForecast;
    }

    cJSON* maxTemperature = cJSON_GetObjectItem(daily, "temperature_2m_max");
    if(maxTemperature == NULL || !cJSON_IsArray(maxTemperature) || cJSON_GetArraySize(maxTemperature) < 4){
        free(buffer);
        printf("Forecast: maxTemperature error");
        return retForecast;
    }

    cJSON* minTemperature = cJSON_GetObjectItem(daily, "temperature_2m_min");
    if(minTemperature == NULL || !cJSON_IsArray(minTemperature) || cJSON_GetArraySize(minTemperature) < 4){
        free(buffer);
        printf("Forecast: minTemperature error");
        return retForecast;
    }

    cJSON* sunrise = cJSON_GetObjectItem(daily, "sunrise");
    if(sunrise == NULL || !cJSON_IsArray(sunrise) || cJSON_GetArraySize(sunrise) < 4){
        free(buffer);
        printf("Forecast: sunrise error");
        return retForecast;
    }

    cJSON* sunset = cJSON_GetObjectItem(daily, "sunset");
    if(sunset == NULL || !cJSON_IsArray(sunset)){
        free(buffer);
        printf("Forecast: sunset error");
        return retForecast;
    }

    time_t now;
    struct tm timeinfo;
    time(&now);
    localtime_r(&now, &timeinfo);

    for (int i = 0; i < 4; i++) {
        cJSON* wc = cJSON_GetArrayItem(weatherCode, i);
        if (cJSON_IsNumber(wc)) {
            retForecast.weatherData[i].weatherCode = wc->valueint;
        }
        cJSON* maxT = cJSON_GetArrayItem(maxTemperature, i);
        if(cJSON_IsNumber(maxT)){
            retForecast.weatherData[i].maxTemperature = (int)(maxT->valuedouble * 10.0);
        }
        cJSON* minT = cJSON_GetArrayItem(minTemperature, i);
        if(cJSON_IsNumber(minT)){
            retForecast.weatherData[i].minTemperature = (int)(minT->valuedouble * 10.0);
        }
        cJSON* rise = cJSON_GetArrayItem(sunrise, i);
        if(cJSON_IsString(rise)){
            int hour, minute;
            if(sscanf(rise->valuestring, "%*d-%*d-%*dT%d:%d", &hour, &minute) == 2){
                retForecast.weatherData[i].sunrise = hour * 100 + minute;
            }
        }
        cJSON* set = cJSON_GetArrayItem(sunset, i);
        if(cJSON_IsString(set)){
            int hour, minute;
            if(sscanf(set->valuestring, "%*d-%*d-%*dT%d:%d", &hour, &minute) == 2){
                retForecast.weatherData[i].sunset = hour * 100 + minute;
            }
        }
        retForecast.weatherData[i].wday = (i + timeinfo.tm_wday) % 7;
    }

    cJSON_Delete(root);
    retForecast.error = 0;
    free(buffer);
    return retForecast;
}
ks_extended_weather_forecast_t ks_weather_get_extended_forecast() {
    ks_extended_weather_forecast_t retForecast;
    memset(retForecast.weatherData, 0, sizeof(retForecast.weatherData));
    retForecast.error = 1;

    ks_value_t x = ks_nvs_load_int32("lon");
    ks_value_t y = ks_nvs_load_int32("lat");

    char url[512] = {};
    float lon = 0.0f, lat = 0.0f;

    if(x.error == 0 && y.error == 0){
        memcpy(&lon, &x.value, sizeof(float));
        memcpy(&lat, &y.value, sizeof(float));
    }

    snprintf(url, sizeof(url), "https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&hourly=temperature_2m,precipitation_probability,weather_code&forecast_days=3&timezone=Europe%%2FBerlin", lat, lon);

    if(s_httpExtendedClient == 0){
        esp_http_client_config_t clientConfig = {
            .url = url,
            .method = HTTP_METHOD_GET,
            .keep_alive_enable = 1,
            .buffer_size = 0,
            .timeout_ms = 5000,
        };
        s_httpExtendedClient = esp_http_client_init(&clientConfig);
    } else {
        esp_http_client_set_url(s_httpExtendedClient, url);
    }

    if(esp_http_client_open(s_httpExtendedClient, 0) != ESP_OK){
        printf("Forecast: open error\n");
        return retForecast;
    }
    if(esp_http_client_fetch_headers(s_httpExtendedClient) < 0){
        printf("Forecast: fetch error\n");
        esp_http_client_close(s_httpExtendedClient);
        return retForecast;
    }

    char* buffer = calloc(4096 + 1, 1);
    const int len = esp_http_client_read(s_httpExtendedClient, buffer, 4096);
    if(len == -1){
        free(buffer);
        printf("Forecast: read error\n");
        esp_http_client_close(s_httpExtendedClient);
        return retForecast;
    }
    buffer[len] = '\0';
    if(esp_http_client_close(s_httpExtendedClient) != ESP_OK){
        free(buffer);
        printf("Forecast: close error\n");
        return retForecast;
    }

    cJSON* root = cJSON_Parse(buffer);
    if(root == NULL){
        free(buffer);
        printf("Forecast: error parsing JSON\n");
        return retForecast;
    }

    cJSON* hourly = cJSON_GetObjectItem(root, "hourly");
    if(hourly == NULL){
        free(buffer);
        printf("Forecast: hourly error");
        return retForecast;
    }

    cJSON* times = cJSON_GetObjectItem(hourly, "time");
    if(times == NULL || !cJSON_IsArray(times) || cJSON_GetArraySize(times) == 0){
        free(buffer);
        printf("Forecast: times error");
        return retForecast;
    }

    cJSON* temperatures = cJSON_GetObjectItem(hourly, "temperature_2m");
    if(temperatures == NULL || !cJSON_IsArray(temperatures) || cJSON_GetArraySize(temperatures) == 0){
        free(buffer);
        printf("Forecast: temperatures error");
        return retForecast;
    }

    cJSON* probabilities = cJSON_GetObjectItem(hourly, "precipitation_probability");
    if(probabilities == NULL || !cJSON_IsArray(probabilities) || cJSON_GetArraySize(probabilities) == 0){
        free(buffer);
        printf("Forecast: probabilities error");
        return retForecast;
    }

    cJSON* wCodes = cJSON_GetObjectItem(hourly, "weather_code");
    if(wCodes == NULL || !cJSON_IsArray(wCodes) || cJSON_GetArraySize(wCodes) == 0){
        free(buffer);
        printf("Forecast: wCodes error");
        return retForecast;
    }

    const int size = MIN(
        cJSON_GetArraySize(times),
        MIN(cJSON_GetArraySize(temperatures),
        MIN(cJSON_GetArraySize(probabilities),
        cJSON_GetArraySize(wCodes))
    ));

    for (int i = 0; i < size; i++) {
        cJSON* wc = cJSON_GetArrayItem(wCodes, i);
        if(cJSON_IsNumber(wc)){
            retForecast.weatherData[i / 24].weatherCode[i % 24] = wc->valueint;
        }

        cJSON* t = cJSON_GetArrayItem(temperatures, i);
        if(cJSON_IsNumber(t)){
            retForecast.weatherData[i / 24].temperature[i % 24] = (int)(t->valuedouble * 10.0);
        }

        cJSON* p = cJSON_GetArrayItem(probabilities, i);
        if(cJSON_IsNumber(t)){
            retForecast.weatherData[i / 24].precipitationProbability[i % 24] = p->valueint;
        }

        cJSON* timeString = cJSON_GetArrayItem(times, i);
        if(cJSON_IsString(timeString)){
            int month, day;
            if(sscanf(timeString->valuestring, "%*d-%d-%dT%*d:%*d", &month, &day) == 2){
                retForecast.weatherData[i / 24].month = month;
                retForecast.weatherData[i / 24].day = day;
            }
        }
    }

    cJSON_Delete(root);
    retForecast.error = 0;
    free(buffer);
    return retForecast;
}
char* ks_symbols_get_weather_icon_day(const ks_weather_t* weather){
    switch(weather->weatherCode){
        case 0: return ((weather->windSpeed > WINDY) ? WI_DAY_WINDY : WI_DAY_SUNNY); break;
        case 1: return ((weather->windSpeed > WINDY) ? WI_DAY_WINDY : WI_DAY_SUNNY); break;
        case 2: return ((weather->windSpeed > WINDY) ? WI_DAY_CLOUDY_GUSTS : WI_DAY_CLOUDY); break;
        case 3: return ((weather->windSpeed > WINDY) ? WI_DAY_CLOUDY_GUSTS : WI_DAY_SUNNY_OVERCAST); break;
        case 45: 
        case 48: return WI_DAY_FOG; break;
        case 51: 
        case 53:
        case 55:
        case 56:
        case 57: return WI_DAY_SPRINKLE; break;
        case 61:
        case 63:
        case 65:
        case 66:
        case 67: return (weather->windSpeed > WINDY) ? WI_DAY_RAIN_WIND : WI_DAY_RAIN; break;
        case 71:
        case 73:
        case 75: 
        case 77: return (weather->windSpeed > WINDY) ? WI_DAY_SNOW_WIND : WI_DAY_SNOW; break;
        case 80:
        case 81:
        case 82: return WI_DAY_SHOWERS; break;
        case 85:
        case 86: return (weather->windSpeed > WINDY) ? WI_DAY_SNOW_WIND : WI_DAY_SNOW; break;
        case 95: return WI_DAY_THUNDERSTORM; break;
        case 96:
        case 99: return WI_DAY_SNOW_THUNDERSTORM; break;
        default: break;
    }
    return "";
}
char* ks_symbols_get_weather_icon_night(const ks_weather_t* weather){
   switch(weather->weatherCode){
        case 0: return (weather->windSpeed > WINDY) ? WI_STRONG_WIND : WI_NIGHT_CLEAR; break;
        case 1: return (weather->windSpeed > WINDY) ? WI_STRONG_WIND : WI_NIGHT_CLEAR; break;
        case 2: return (weather->windSpeed > WINDY) ? WI_NIGHT_ALT_CLOUDY_GUSTS : WI_NIGHT_ALT_CLOUDY; break;
        case 3: return (weather->windSpeed > WINDY) ? WI_NIGHT_ALT_CLOUDY_GUSTS : WI_NIGHT_ALT_CLOUDY; break;
        case 45: 
        case 48: return WI_NIGHT_FOG; break;
        case 51: 
        case 53:
        case 55:
        case 56:
        case 57: return WI_NIGHT_ALT_SPRINKLE; break;
        case 61:
        case 63:
        case 65:
        case 66:
        case 67: return (weather->windSpeed > WINDY) ? WI_NIGHT_ALT_RAIN_WIND : WI_NIGHT_ALT_RAIN; break;
        case 71:
        case 73:
        case 75: 
        case 77: return (weather->windSpeed > WINDY) ? WI_NIGHT_ALT_SNOW_WIND : WI_NIGHT_ALT_SNOW; break;
        case 80:
        case 81:
        case 82: return WI_NIGHT_ALT_SHOWERS; break;
        case 85:
        case 86: return (weather->windSpeed > WINDY) ? WI_NIGHT_ALT_SNOW_WIND : WI_NIGHT_ALT_SNOW; break;
        case 95: return WI_NIGHT_ALT_THUNDERSTORM; break;
        case 96:
        case 99: return WI_NIGHT_ALT_THUNDERSTORM; break;
        default: break;
    }
    return "";
}
char* ks_symbols_get_weather_icon_neutral(const uint8_t code){
    switch(code){
        case 0: 
        case 1: return WI_DAY_SUNNY; break;
        case 2: return WI_CLOUD; break;
        case 3: return WI_CLOUD; break;
        case 45: 
        case 48: return WI_FOG; break;
        case 51: 
        case 53:
        case 55:
        case 56:
        case 57: return WI_SPRINKLE; break;
        case 61:
        case 63:
        case 65:
        case 66:
        case 67: return WI_RAIN; break;
        case 71:
        case 73:
        case 75: 
        case 77: return WI_SNOW; break;
        case 80:
        case 81:
        case 82: return WI_SHOWERS; break;
        case 85:
        case 86: return WI_SNOW; break;
        case 95: return WI_THUNDERSTORM; break;
        case 96:
        case 99: return WI_THUNDERSTORM; break;
        default: break;
    }
    return "";
}