#include "WeatherServer.hpp"
#include "Util.hpp"

extern "C" {
    #include "NVS.h"
}

#include <sstream>
#include <algorithm>
#include <iostream>

namespace ws {
    static esp_err_t root_get_handler(httpd_req_t *req) {
        std::string page = ws::load_page("/storage/index.html");
        httpd_resp_set_type(req, "text/html");
        httpd_resp_send(req, page.c_str(), HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }
    static esp_err_t api_get_handler(httpd_req_t *req) {
        char query[1024] = {};

        if(httpd_req_get_url_query_str(req, query, sizeof(query)) == ESP_OK){
            char type[256] = {};
            if(httpd_query_key_value(query, "type", type, sizeof(type)) == ESP_OK){
                auto add_entry = [](std::stringstream& ss, const Measurement& measurement) -> void {
                    ss << "{" << "\"timestamp\":" << measurement.timestamp << ","
                    << "\"temperature\":" << measurement.t << ","
                    << "\"pressure\": " << measurement.p << ","
                    << "\"humidity\": " << measurement.h << "}";
                };
                WeatherServer::Data* data = (WeatherServer::Data*)req->user_ctx;

                if(strcmp(type, "current") == 0) {
                    std::stringstream ss;
                    add_entry(ss, data->currentMeasurement);
                    
                    httpd_resp_set_type(req, "text/json");
                    httpd_resp_send(req, ss.str().c_str(), HTTPD_RESP_USE_STRLEN);
                    return ESP_OK;
                }
                else if(strcmp(type, "last") == 0) {
                    std::stringstream ss;

                    size_t limit = 100;
                    char limitChar[256] = {};
                    if(httpd_query_key_value(query, "limit", limitChar, sizeof(limitChar)) == ESP_OK){
                        limit = atoll(limitChar);
                    }

                    const size_t s = std::min(data->measurements.size(), limit);
                    ss << "[";
                    for(std::size_t i = 0; i < s; i++) {
                        add_entry(ss, data->measurements[i]);
                        if(i != s - 1) ss << ",";
                    }
                    ss << "]";
                    
                    httpd_resp_set_type(req, "text/plain");
                    httpd_resp_send(req, ss.str().c_str(), HTTPD_RESP_USE_STRLEN);
                    return ESP_OK;
                }
                else if(strcmp(type, "location") == 0) {
                    float lon = 0;
                    float lat = 0;

                    char lonChar[256] = {};
                    char latChar[256] = {};

                    if(httpd_query_key_value(query, "lon", lonChar, sizeof(lonChar)) == ESP_OK && httpd_query_key_value(query, "lat", latChar, sizeof(latChar)) == ESP_OK){
                        lon = strtof(lonChar, 0);
                        lat = strtof(latChar, 0);

                        int32_t x, y;
                        memcpy(&x, &lon, sizeof(int32_t));
                        memcpy(&y, &lat, sizeof(int32_t));

                        ks_nvs_save_int32("lon", x);
                        ks_nvs_save_int32("lat", y);

                        data->locationChanged = 1;

                        httpd_resp_set_type(req, "text/json");
                        httpd_resp_send(req, "[]", HTTPD_RESP_USE_STRLEN);
                        return ESP_OK;
                    }
                    else {
                        httpd_resp_set_status(req, "400 Bad Request");
                        httpd_resp_set_type(req, "text/plain");
                        httpd_resp_send(req, "Missing 'lon' or 'lat' query parameter", HTTPD_RESP_USE_STRLEN);
                        return ESP_FAIL;
                    }
                }
            }
        }
        
        httpd_resp_set_type(req, "text/plain");
        httpd_resp_send(req, "Invalid argument.", HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }
    esp_err_t style_get_handler(httpd_req_t *req){
        std::string page = ws::load_page("/storage/style.css");
        httpd_resp_set_type(req, "text/css");
        httpd_resp_send(req, page.c_str(), HTTPD_RESP_USE_STRLEN);
        return ESP_OK;
    }

    auto WeatherServer::start() -> bool {
        if(m_server) return 1;

        httpd_config_t config = HTTPD_DEFAULT_CONFIG();
        config.stack_size = 16384;
        esp_err_t err = httpd_start(&m_server, &config);
        if(err != ESP_OK) {
            return 0;
        }

        httpd_uri_t root = {
            .uri       = "/",
            .method    = HTTP_GET,
            .handler   = root_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(m_server, &root);

        httpd_uri_t api = {
            .uri       = "/api",
            .method    = HTTP_GET,
            .handler   = api_get_handler,
            .user_ctx  = &m_data
        };
        httpd_register_uri_handler(m_server, &api);

        httpd_uri_t styleUri = {
            .uri       = "/style.css",
            .method    = HTTP_GET,
            .handler   = style_get_handler,
            .user_ctx  = NULL
        };
        httpd_register_uri_handler(m_server, &styleUri);

        return 1;
    }

    auto WeatherServer::stop() -> bool {
        esp_err_t err = httpd_stop(m_server);
        if(err != ESP_OK) {
            return 0;
        }
        m_server = 0;
        return 1;
    }

    auto WeatherServer::insert_measurement(const Measurement& m) -> void {
        m_data.measurements.push_front(m);
        if(m_data.measurements.size() > 1500) {
            m_data.measurements.pop_back();
        }
    }
}