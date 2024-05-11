#include "lib/include/request_processing.h"

WeatherDescription::WeatherDescription(const json& data_from_api, int day_after_current, int day_after_current_time) {
    wmo_code_ = data_from_api["hourly"]["weather_code"][kHoursPerDay * day_after_current + day_after_current_time];
    temperature_ = data_from_api["hourly"]["temperature_2m"][kHoursPerDay * day_after_current + day_after_current_time];
    wind_speed_ = data_from_api["hourly"]["wind_speed_10m"][kHoursPerDay * day_after_current + day_after_current_time];
    visibility_ = data_from_api["hourly"]["visibility"][kHoursPerDay * day_after_current + day_after_current_time];
    precipitation_ = data_from_api["hourly"]["precipitation"][kHoursPerDay * day_after_current + day_after_current_time];
}

int WeatherDescription::GetCode() const {
    return wmo_code_;
}

int WeatherDescription::GetTemperature() const {
    return temperature_;
}

int WeatherDescription::GetWindSpeed() const {
    return wind_speed_;
}

int WeatherDescription::GetVisibility() const {
    return visibility_;
}

int WeatherDescription::GetPrecipitation() const {
    return precipitation_;
}

DayDescription::DayDescription(const json& data_open_meteo, int day_after_current) {
    calendar_date_ = static_cast<std::string> (data_open_meteo["hourly"]["time"][kHoursPerDay * day_after_current]).substr(0, kCalendarDatePrefix);
    for (int i = 0; i < kCountDayTime; ++i) {
        weather_day_[i] = WeatherDescription(data_open_meteo, day_after_current, static_cast<int> (kDayTimeDelay * (i + 1)));
    }
}

std::string DayDescription::GetCalendarDate() const {
    return calendar_date_;
}

WeatherDescription& DayDescription::operator[](int day_after_current_time) {
    return weather_day_[day_after_current_time];
}

WeatherDescription DayDescription::At(int day_after_current_time) const {
    return weather_day_[day_after_current_time];
}

City::City(const std::string& city_name) {
    city_name_ = city_name;
}

std::string City::GetName() const {
    return city_name_;
}

DayDescription& City::operator[](int day_after_current) {
    return forecast_[day_after_current];
}

DayDescription City::At(int day_after_current) const {
    return forecast_[day_after_current];
}

RequestProcessor::RequestProcessor(const ConfigParser& cfg_parser) {
    cfg_parser_ = cfg_parser;
    cities_.resize(cfg_parser_.GetCountOfCities());
}

void RequestProcessor::SetDataFromParser(const ConfigParser &cfg_parser) {
    *this = RequestProcessor(cfg_parser);
}

City RequestProcessor::GetCityData(int index_of_city) const {
    return cities_[index_of_city];
}

bool RequestProcessor::HasFirstUpdate() const {
    return !first_update_;
}

UpdatingStatus ValidatingApiNinjaResponse(const cpr::Response& response_ninja) {
    if (response_ninja.status_code != kSuccessStatusCode) {
        return UpdatingStatus::kApiNinjasNoConnection;
    }

    if (response_ninja.text == "[]") {
        return UpdatingStatus::kInvalidCityName;
    }

    return UpdatingStatus::kSuccess;
}

UpdatingStatus ValidatingOpenMeteoResponse(const cpr::Response& response_open_meteo) {
    if (response_open_meteo.status_code != kSuccessStatusCode) {
        return UpdatingStatus::kOpenMeteoNoConnection;
    }

    return UpdatingStatus::kSuccess;
}

UpdatingStatus RequestProcessor::UpdateData() {
    first_update_ = true;

    for (int i = 0; i < cfg_parser_.GetCountOfCities(); ++i) {
        cities_[i] = City(cfg_parser_.GetCity(i));
        const cpr::Response response_ninja = cpr::Get(cpr::Url {"https://api.api-ninjas.com/v1/city"},
                                                      cpr::Header {{"X-Api-Key", cfg_parser_.GetApiCode()}},
                                                      cpr::Parameters {{"name", cfg_parser_.GetCity(i)}});

        const UpdatingStatus validating_api_ninja = ValidatingApiNinjaResponse(response_ninja);
        if (validating_api_ninja != UpdatingStatus::kSuccess) {
            return validating_api_ninja;
        }

        const json data_ninja = json::parse(response_ninja.text);
        const double latitude = data_ninja[0]["latitude"];
        const double longitude = data_ninja[0]["longitude"];

        const cpr::Response response_open_meteo = cpr::Get(cpr::Url {"https://api.open-meteo.com/v1/forecast"},
                                                           cpr::Parameters {
                                                                   {"latitude", std::to_string(latitude)},
                                                                   {"longitude", std::to_string(longitude)},
                                                                   {"forecast_days", "16"},
                                                                   {"hourly", "weather_code"},
                                                                   {"hourly", "temperature_2m"},
                                                                   {"hourly", "wind_speed_10m"},
                                                                   {"hourly", "visibility"},
                                                                   {"hourly", "precipitation"}});

        const UpdatingStatus validating_open_meteo = ValidatingOpenMeteoResponse(response_open_meteo);
        if (validating_open_meteo != UpdatingStatus::kSuccess) {
            return validating_open_meteo;
        }

        const json data_open_meteo = json::parse(response_open_meteo.text);
        for (int j = 0; j < kMaxByDayForecast; ++j) {
            cities_[i][j] = DayDescription(data_open_meteo, j);
        }
    }

    return UpdatingStatus::kSuccess;
}