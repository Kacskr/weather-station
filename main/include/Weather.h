#ifndef WEATHER_H_INCLUDED
#define WEATHER_H_INCLUDED

#include <stdbool.h>
#include <stdint.h>

// source: https://erikflowers.github.io/weather-icons/
#define WI_DAY_SUNNY "\xEF\x80\x8D"
#define WI_DAY_CLOUDY "\xEF\x80\x82"
#define WI_DAY_CLOUDY_GUSTS "\xEF\x80\x80"
#define WI_DAY_CLOUDY_WINDY "\xEF\x80\x81"
#define WI_DAY_FOG "\xEF\x80\x83"
#define WI_DAY_HAIL "\xEF\x80\x84"
#define WI_DAY_HAZE "\xEF\x82\xB6"
#define WI_DAY_LIGHTNING "\xEF\x80\x85"
#define WI_DAY_RAIN "\xEF\x80\x88"
#define WI_DAY_RAIN_MIX "\xEF\x80\x86"
#define WI_DAY_RAIN_WIND "\xEF\x80\x87"
#define WI_DAY_SHOWERS "\xEF\x80\x89"
#define WI_DAY_SLEET "\xEF\x82\xB2"
#define WI_DAY_SLEET_STORM "\xEF\x81\xA8"
#define WI_DAY_SNOW "\xEF\x80\x8A"
#define WI_DAY_SNOW_THUNDERSTORM "\xEF\x81\xAB"
#define WI_DAY_SNOW_WIND "\xEF\x81\xA5"
#define WI_DAY_SPRINKLE "\xEF\x80\x8B"
#define WI_DAY_STORM_SHOWERS "\xEF\x80\x8E"
#define WI_DAY_SUNNY_OVERCAST "\xEF\x80\x8C"
#define WI_DAY_THUNDERSTORM "\xEF\x80\x90"
#define WI_DAY_WINDY "\xEF\x82\x85"
#define WI_SOLAR_ECLIPSE "\xEF\x81\xAE"
#define WI_HOT "\xEF\x81\xB2"
#define WI_DAY_CLOUDY_HIGH "\xEF\x81\xBD"
#define WI_DAY_LIGHT_WIND "\xEF\x83\x84"
#define WI_NIGHT_CLEAR "\xEF\x80\xAE"
#define WI_NIGHT_ALT_CLOUDY "\xEF\x82\x86"
#define WI_NIGHT_ALT_CLOUDY_GUSTS "\xEF\x80\xA2"
#define WI_NIGHT_ALT_CLOUDY_WINDY "\xEF\x80\xA3"
#define WI_NIGHT_ALT_HAIL "\xEF\x80\xA4"
#define WI_NIGHT_ALT_LIGHTNING "\xEF\x80\xA5"
#define WI_NIGHT_ALT_RAIN "\xEF\x80\xA8"
#define WI_NIGHT_ALT_RAIN_MIX "\xEF\x80\xA6"
#define WI_NIGHT_ALT_RAIN_WIND "\xEF\x80\xA7"
#define WI_NIGHT_ALT_SHOWERS "\xEF\x80\xA9"
#define WI_NIGHT_ALT_SLEET "\xEF\x82\xB4"
#define WI_NIGHT_ALT_SLEET_STORM "\xEF\x81\xAA"
#define WI_NIGHT_ALT_SNOW "\xEF\x80\xAA"
#define WI_NIGHT_ALT_SNOW_THUNDERSTORM "\xEF\x81\xAD"
#define WI_NIGHT_ALT_SNOW_WIND "\xEF\x81\xA7"
#define WI_NIGHT_ALT_SPRINKLE "\xEF\x80\xAB"
#define WI_NIGHT_ALT_STORM_SHOWERS "\xEF\x80\xAC"
#define WI_NIGHT_ALT_THUNDERSTORM "\xEF\x80\xAD"
#define WI_NIGHT_CLOUDY "\xEF\x80\xB1"
#define WI_NIGHT_CLOUDY_GUSTS "\xEF\x80\xAF"
#define WI_NIGHT_CLOUDY_WINDY "\xEF\x80\xB0"
#define WI_NIGHT_FOG "\xEF\x81\x8A"
#define WI_NIGHT_HAIL "\xEF\x80\xB2"
#define WI_NIGHT_LIGHTNING "\xEF\x80\xB3"
#define WI_NIGHT_PARTLY_CLOUDY "\xEF\x82\x83"
#define WI_NIGHT_RAIN "\xEF\x80\xB6"
#define WI_NIGHT_RAIN_MIX "\xEF\x80\xB4"
#define WI_NIGHT_RAIN_WIND "\xEF\x80\xB5"
#define WI_NIGHT_SHOWERS "\xEF\x80\xB7"
#define WI_NIGHT_SLEET "\xEF\x82\xB3"
#define WI_NIGHT_SLEET_STORM "\xEF\x81\xA9"
#define WI_NIGHT_SNOW "\xEF\x80\xB8"
#define WI_NIGHT_SNOW_THUNDERSTORM "\xEF\x81\xAC"
#define WI_NIGHT_SNOW_WIND "\xEF\x81\xA6"
#define WI_NIGHT_SPRINKLE "\xEF\x80\xB9"
#define WI_NIGHT_STORM_SHOWERS "\xEF\x80\xBA"
#define WI_NIGHT_THUNDERSTORM "\xEF\x80\xBB"
#define WI_LUNAR_ECLIPSE "\xEF\x81\xB0"
#define WI_STARS "\xEF\x81\xB7"
#define WI_STORM_SHOWERS "\xEF\x80\x9D"
#define WI_THUNDERSTORM "\xEF\x80\x9E"
#define WI_NIGHT_ALT_CLOUDY_HIGH "\xEF\x81\xBE"
#define WI_NIGHT_CLOUDY_HIGH "\xEF\x82\x80"
#define WI_NIGHT_ALT_PARTLY_CLOUDY "\xEF\x82\x81"
#define WI_CLOUD "\xEF\x81\x81"
#define WI_CLOUDY "\xEF\x80\x93"
#define WI_CLOUDY_GUSTS "\xEF\x80\x91"
#define WI_CLOUDY_WINDY "\xEF\x80\x92"
#define WI_FOG "\xEF\x80\x94"
#define WI_HAIL "\xEF\x80\x95"
#define WI_RAIN "\xEF\x80\x99"
#define WI_RAIN_MIX "\xEF\x80\x97"
#define WI_RAIN_WIND "\xEF\x80\x98"
#define WI_SHOWERS "\xEF\x80\x9A"
#define WI_SLEET "\xEF\x82\xB5"
#define WI_SNOW "\xEF\x80\x9B"
#define WI_SPRINKLE "\xEF\x80\x9C"
#define WI_SNOW_WIND "\xEF\x81\xA4"
#define WI_SMOG "\xEF\x81\xB4"
#define WI_SMOKE "\xEF\x81\xA2"
#define WI_LIGHTNING "\xEF\x80\x96"
#define WI_RAINDROPS "\xEF\x81\x8E"
#define WI_RAINDROP "\xEF\x81\xB8"
#define WI_DUST "\xEF\x81\xA3"
#define WI_SNOWFLAKE_COLD "\xEF\x81\xB6"
#define WI_WINDY "\xEF\x80\xA1"
#define WI_STRONG_WIND "\xEF\x81\x90"
#define WI_SANDSTORM "\xEF\x82\x82"
#define WI_EARTHQUAKE "\xEF\x83\x86"
#define WI_FIRE "\xEF\x83\x87"
#define WI_FLOOD "\xEF\x81\xBC"
#define WI_METEOR "\xEF\x81\xB1"
#define WI_TSUNAMI "\xEF\x83\x85"
#define WI_VOLCANO "\xEF\x83\x88"
#define WI_HURRICANE "\xEF\x81\xB3"
#define WI_TORNADO "\xEF\x81\x96"
#define WI_SMALL_CRAFT_ADVISORY "\xEF\x83\x8C"
#define WI_GALE_WARNING "\xEF\x83\x8D"
#define WI_STORM_WARNING "\xEF\x83\x8E"
#define WI_HURRICANE_WARNING "\xEF\x83\x8F"
#define WI_WIND_DIRECTION "\xEF\x82\xB1"
#define WI_ALIEN "\xEF\x81\xB5"
#define WI_CELSIUS "\xEF\x80\xBC"
#define WI_FAHRENHEIT "\xEF\x81\x85"
#define WI_DEGREES "\xEF\x81\x82"
#define WI_THERMOMETER "\xEF\x81\x95"
#define WI_THERMOMETER_EXTERIOR "\xEF\x81\x93"
#define WI_THERMOMETER_INTERNAL "\xEF\x81\x94"
#define WI_CLOUD_DOWN "\xEF\x80\xBD"
#define WI_CLOUD_UP "\xEF\x81\x80"
#define WI_CLOUD_REFRESH "\xEF\x80\xBE"
#define WI_HORIZON "\xEF\x81\x87"
#define WI_HORIZON_ALT "\xEF\x81\x86"
#define WI_SUNRISE "\xEF\x81\x91"
#define WI_SUNSET "\xEF\x81\x92"
#define WI_MOONRISE "\xEF\x83\x89"
#define WI_MOONSET "\xEF\x83\x8A"
#define WI_REFRESH "\xEF\x81\x8C"
#define WI_REFRESH_ALT "\xEF\x81\x8B"
#define WI_UMBRELLA "\xEF\x82\x84"
#define WI_BAROMETER "\xEF\x81\xB9"
#define WI_HUMIDITY "\xEF\x81\xBA"
#define WI_NA "\xEF\x81\xBB"
#define WI_TRAIN "\xEF\x83\x8B"
#define WI_MOON_NEW "\xEF\x82\x95"
#define WI_MOON_WAXING_CRESCENT_1 "\xEF\x82\x96"
#define WI_MOON_WAXING_CRESCENT_2 "\xEF\x82\x97"
#define WI_MOON_WAXING_CRESCENT_3 "\xEF\x82\x98"
#define WI_MOON_WAXING_CRESCENT_4 "\xEF\x82\x99"
#define WI_MOON_WAXING_CRESCENT_5 "\xEF\x82\x9A"
#define WI_MOON_WAXING_CRESCENT_6 "\xEF\x82\x9B"
#define WI_MOON_FIRST_QUARTER "\xEF\x82\x9C"
#define WI_MOON_WAXING_GIBBOUS_1 "\xEF\x82\x9D"
#define WI_MOON_WAXING_GIBBOUS_2 "\xEF\x82\x9E"
#define WI_MOON_WAXING_GIBBOUS_3 "\xEF\x82\x9F"
#define WI_MOON_WAXING_GIBBOUS_4 "\xEF\x82\xA0"
#define WI_MOON_WAXING_GIBBOUS_5 "\xEF\x82\xA1"
#define WI_MOON_WAXING_GIBBOUS_6 "\xEF\x82\xA2"
#define WI_MOON_FULL "\xEF\x82\xA3"
#define WI_MOON_WANING_GIBBOUS_1 "\xEF\x82\xA4"
#define WI_MOON_WANING_GIBBOUS_2 "\xEF\x82\xA5"
#define WI_MOON_WANING_GIBBOUS_3 "\xEF\x82\xA6"
#define WI_MOON_WANING_GIBBOUS_4 "\xEF\x82\xA7"
#define WI_MOON_WANING_GIBBOUS_5 "\xEF\x82\xA8"
#define WI_MOON_WANING_GIBBOUS_6 "\xEF\x82\xA9"
#define WI_MOON_THIRD_QUARTER "\xEF\x82\xAA"
#define WI_MOON_WANING_CRESCENT_1 "\xEF\x82\xAB"
#define WI_MOON_WANING_CRESCENT_2 "\xEF\x82\xAC"
#define WI_MOON_WANING_CRESCENT_3 "\xEF\x82\xAD"
#define WI_MOON_WANING_CRESCENT_4 "\xEF\x82\xAE"
#define WI_MOON_WANING_CRESCENT_5 "\xEF\x82\xAF"
#define WI_MOON_WANING_CRESCENT_6 "\xEF\x82\xB0"
#define WI_MOON_ALT_NEW "\xEF\x83\xAB"
#define WI_MOON_ALT_WAXING_CRESCENT_1 "\xEF\x83\x90"
#define WI_MOON_ALT_WAXING_CRESCENT_2 "\xEF\x83\x91"
#define WI_MOON_ALT_WAXING_CRESCENT_3 "\xEF\x83\x92"
#define WI_MOON_ALT_WAXING_CRESCENT_4 "\xEF\x83\x93"
#define WI_MOON_ALT_WAXING_CRESCENT_5 "\xEF\x83\x94"
#define WI_MOON_ALT_WAXING_CRESCENT_6 "\xEF\x83\x95"
#define WI_MOON_ALT_FIRST_QUARTER "\xEF\x83\x96"
#define WI_MOON_ALT_WAXING_GIBBOUS_1 "\xEF\x83\x97"
#define WI_MOON_ALT_WAXING_GIBBOUS_2 "\xEF\x83\x98"
#define WI_MOON_ALT_WAXING_GIBBOUS_3 "\xEF\x83\x99"
#define WI_MOON_ALT_WAXING_GIBBOUS_4 "\xEF\x83\x9A"
#define WI_MOON_ALT_WAXING_GIBBOUS_5 "\xEF\x83\x9B"
#define WI_MOON_ALT_WAXING_GIBBOUS_6 "\xEF\x83\x9C"
#define WI_MOON_ALT_FULL "\xEF\x83\x9D"
#define WI_MOON_ALT_WANING_GIBBOUS_1 "\xEF\x83\x9E"
#define WI_MOON_ALT_WANING_GIBBOUS_2 "\xEF\x83\x9F"
#define WI_MOON_ALT_WANING_GIBBOUS_3 "\xEF\x83\xA0"
#define WI_MOON_ALT_WANING_GIBBOUS_4 "\xEF\x83\xA1"
#define WI_MOON_ALT_WANING_GIBBOUS_5 "\xEF\x83\xA2"
#define WI_MOON_ALT_WANING_GIBBOUS_6 "\xEF\x83\xA3"
#define WI_MOON_ALT_THIRD_QUARTER "\xEF\x83\xA4"
#define WI_MOON_ALT_WANING_CRESCENT_1 "\xEF\x83\xA5"
#define WI_MOON_ALT_WANING_CRESCENT_2 "\xEF\x83\xA6"
#define WI_MOON_ALT_WANING_CRESCENT_3 "\xEF\x83\xA7"
#define WI_MOON_ALT_WANING_CRESCENT_4 "\xEF\x83\xA8"
#define WI_MOON_ALT_WANING_CRESCENT_5 "\xEF\x83\xA9"
#define WI_MOON_ALT_WANING_CRESCENT_6 "\xEF\x83\xAA"
#define WI_MOON_0 "\xEF\x82\x95"
#define WI_MOON_1 "\xEF\x82\x96"
#define WI_MOON_2 "\xEF\x82\x97"
#define WI_MOON_3 "\xEF\x82\x98"
#define WI_MOON_4 "\xEF\x82\x99"
#define WI_MOON_5 "\xEF\x82\x9A"
#define WI_MOON_6 "\xEF\x82\x9B"
#define WI_MOON_7 "\xEF\x82\x9C"
#define WI_MOON_8 "\xEF\x82\x9D"
#define WI_MOON_9 "\xEF\x82\x9E"
#define WI_MOON_10 "\xEF\x82\x9F"
#define WI_MOON_11 "\xEF\x82\xA0"
#define WI_MOON_12 "\xEF\x82\xA1"
#define WI_MOON_13 "\xEF\x82\xA2"
#define WI_MOON_14 "\xEF\x82\xA3"
#define WI_MOON_15 "\xEF\x82\xA4"
#define WI_MOON_16 "\xEF\x82\xA5"
#define WI_MOON_17 "\xEF\x82\xA6"
#define WI_MOON_18 "\xEF\x82\xA7"
#define WI_MOON_19 "\xEF\x82\xA8"
#define WI_MOON_20 "\xEF\x82\xA9"
#define WI_MOON_21 "\xEF\x82\xAA"
#define WI_MOON_22 "\xEF\x82\xAB"
#define WI_MOON_23 "\xEF\x82\xAC"
#define WI_MOON_24 "\xEF\x82\xAD"
#define WI_MOON_25 "\xEF\x82\xAE"
#define WI_MOON_26 "\xEF\x82\xAF"
#define WI_MOON_27 "\xEF\x82\xB0"
#define WI_TIME_1 "\xEF\x82\x8A"
#define WI_TIME_2 "\xEF\x82\x8B"
#define WI_TIME_3 "\xEF\x82\x8C"
#define WI_TIME_4 "\xEF\x82\x8D"
#define WI_TIME_5 "\xEF\x82\x8E"
#define WI_TIME_6 "\xEF\x82\x8F"
#define WI_TIME_7 "\xEF\x82\x90"
#define WI_TIME_8 "\xEF\x82\x91"
#define WI_TIME_9 "\xEF\x82\x92"
#define WI_TIME_10 "\xEF\x82\x93"
#define WI_TIME_11 "\xEF\x82\x94"
#define WI_TIME_12 "\xEF\x82\x89"
#define WI_DIRECTION_UP "\xEF\x81\x98"
#define WI_DIRECTION_UP_RIGHT "\xEF\x81\x97"
#define WI_DIRECTION_RIGHT "\xEF\x81\x8D"
#define WI_DIRECTION_DOWN_RIGHT "\xEF\x82\x88"
#define WI_DIRECTION_DOWN "\xEF\x81\x84"
#define WI_DIRECTION_DOWN_LEFT "\xEF\x81\x83"
#define WI_DIRECTION_LEFT "\xEF\x81\x88"
#define WI_DIRECTION_UP_LEFT "\xEF\x82\x87"
#define WI_WIND_BEAUFORT_0 "\xEF\x82\xB7"
#define WI_WIND_BEAUFORT_1 "\xEF\x82\xB8"
#define WI_WIND_BEAUFORT_2 "\xEF\x82\xB9"
#define WI_WIND_BEAUFORT_3 "\xEF\x82\xBA"
#define WI_WIND_BEAUFORT_4 "\xEF\x82\xBB"
#define WI_WIND_BEAUFORT_5 "\xEF\x82\xBC"
#define WI_WIND_BEAUFORT_6 "\xEF\x82\xBD"
#define WI_WIND_BEAUFORT_7 "\xEF\x82\xBE"
#define WI_WIND_BEAUFORT_8 "\xEF\x82\xBF"
#define WI_WIND_BEAUFORT_9 "\xEF\x83\x80"
#define WI_WIND_BEAUFORT_10 "\xEF\x83\x81"
#define WI_WIND_BEAUFORT_11 "\xEF\x83\x82"
#define WI_WIND_BEAUFORT_12 "\xEF\x83\x83"

extern const char* ks_days[7];
extern const char* ks_months[12];

typedef struct{
    bool error;
    bool isDay;
    int temperature;
    int weatherCode;
    int windSpeed;
    int humidity;
    int apparentTemperature;
    int surfacePressure;
    int windDirection;
} ks_weather_t;

typedef struct{
    uint8_t wday;
    uint8_t weatherCode;
    int maxTemperature;
    int minTemperature;
    int sunrise;
    int sunset;
} weather_data_t;

typedef struct{
    uint8_t month;
    uint8_t day;
    int temperature[24];
    int precipitationProbability[24];
    int weatherCode[24];
} extended_weather_data_t;

typedef struct{
    bool error;
    weather_data_t weatherData[4];
} ks_weather_forecast_t;

typedef struct{
    bool error;
    extended_weather_data_t weatherData[4];
} ks_extended_weather_forecast_t;

extern ks_weather_t ks_weather_update_weather();
extern ks_weather_t ks_weather_get_current_weather();
extern ks_weather_forecast_t ks_weather_get_forecast();
extern ks_extended_weather_forecast_t ks_weather_get_extended_forecast();
extern char* ks_symbols_get_weather_icon_day(const ks_weather_t* weather);
extern char* ks_symbols_get_weather_icon_night(const ks_weather_t* weather);
extern char* ks_symbols_get_weather_icon_neutral(const uint8_t code);

#endif // WEATHER_H_INCLUDED