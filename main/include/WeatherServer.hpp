#pragma once

#include "esp_http_server.h"
#include "SharedData.hpp"

#include <deque>

namespace ws {
    struct Measurement {
        int64_t timestamp{};
        float t{};
        float h{};
        float p{};
    };

    class WeatherServer {
    public:
        WeatherServer() = default;
        ~WeatherServer() = default;

        auto start() -> bool;
        auto stop() -> bool;

        auto set_measurement(const Measurement& m) -> void {
            m_data.currentMeasurement = m;
        }
        auto insert_measurement(const Measurement& m) -> void;

        auto has_location_changed() -> bool {
            const bool b = m_data.locationChanged;
            m_data.locationChanged = 0;
            return b;
        }

    public:
        struct Data {
            Measurement currentMeasurement;
            std::deque<Measurement> measurements;
            bool locationChanged{};
        }; 
    private:
        httpd_handle_t m_server{};
        Data m_data;
    };
}