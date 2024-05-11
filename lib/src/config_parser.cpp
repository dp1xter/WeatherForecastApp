#include "../include/config_parser.h"

void ConfigParser::ConfigureData(const std::filesystem::path& json_file) {
    std::ifstream json_path(json_file);
    if (!json_path.is_open()) {
        throw std::invalid_argument("No such JSON file");
    }

    json data = json::parse(json_path);

    cities_name_ = std::vector<std::string> (data["cityNames"].begin(), data["cityNames"].end());

    refresh_interval_ = data["refreshInterval"];
    days_number_ = data["countDays"];
    user_api_code_ = data["apiCode"];
}

ConfigParser::ConfigParser(const std::filesystem::path& json_file) {
    ConfigureData(json_file);
}

void ConfigParser::SetFile(const std::filesystem::path& json_file) {
    ConfigureData(json_file);
}

std::string ConfigParser::GetCity(int index) const {
    return cities_name_.at(index);
}

int ConfigParser::GetInterval() const {
    return refresh_interval_;
}

int ConfigParser::GetDays() const {
    return days_number_;
}

std::string ConfigParser::GetApiCode() const {
    return user_api_code_;
}

size_t ConfigParser::GetCountOfCities() const {
    return cities_name_.size();
}