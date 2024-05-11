#pragma once

#include "cpr/cpr.h"
#include "config_parser.h"
#include <cstdlib>

using json = nlohmann::json;

static const size_t kCountDayTime = 4;
static const size_t kMaxByDayForecast = 15;
static const size_t kHoursPerDay = 24;
static const size_t kDayTimeDelay = 6;
static const size_t kCalendarDatePrefix = 10;
static const int kSuccessStatusCode = 200;

enum class UpdatingStatus {
    kApiNinjasNoConnection,
    kOpenMeteoNoConnection,
    kInvalidCityName,
    kSuccess,
};

class WeatherDescription {
public:
    WeatherDescription() = default;

    explicit WeatherDescription(const json& data_from_api, int day_after_current, int day_after_current_time);

    [[nodiscard]] int GetCode() const;

    [[nodiscard]] int GetTemperature() const;

    [[nodiscard]] int GetWindSpeed() const;

    [[nodiscard]] int GetVisibility() const;

    [[nodiscard]] int GetPrecipitation() const;
private:
    int wmo_code_ = -1;
    int temperature_ = INT_MAX;
    int wind_speed_ = INT_MAX;
    int visibility_ = INT_MAX;
    int precipitation_ = INT_MAX;
};

class DayDescription {
public:
    DayDescription() = default;

    explicit DayDescription(const json& data_open_meteo, int day_after_current);

    [[nodiscard]] std::string GetCalendarDate() const;

    WeatherDescription& operator[] (int day_after_current_time);

    [[nodiscard]] WeatherDescription At(int day_after_current_time) const;
private:
    std::string calendar_date_;
    WeatherDescription weather_day_[kCountDayTime];
};

class City {
public:
    City() = default;

    explicit City(const std::string& city_name);

    [[nodiscard]] std::string GetName() const;

    DayDescription& operator[] (int day_after_current);

    [[nodiscard]] DayDescription At(int day_after_current) const;
private:
    std::string city_name_;
    DayDescription forecast_[kMaxByDayForecast];
};

class RequestProcessor {
public:
    RequestProcessor() = default;

    explicit RequestProcessor(const ConfigParser& cfg_parser);

    void SetDataFromParser(const ConfigParser& cfg_parser);

    [[nodiscard]] City GetCityData(int index_of_city) const;

    [[nodiscard]] bool HasFirstUpdate() const;

    UpdatingStatus UpdateData();
private:
    ConfigParser cfg_parser_;
    std::vector<City> cities_;
    bool first_update_ = false;
};

UpdatingStatus ValidatingApiNinjaResponse(const cpr::Response& response_ninja);

UpdatingStatus ValidatingOpenMeteoResponse(const cpr::Response& response_open_meteo);