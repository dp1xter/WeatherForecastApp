#pragma once

#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stdexcept>

using json = nlohmann::json;

class ConfigParser {
public:
    ConfigParser() = default;

    explicit ConfigParser(const std::filesystem::path& json_file);

    void SetFile(const std::filesystem::path& json_file);

    [[nodiscard]] std::string GetCity(int index) const;

    [[nodiscard]] int GetInterval() const;

    [[nodiscard]] int GetDays() const;

    [[nodiscard]] std::string GetApiCode() const;

    [[nodiscard]] size_t GetCountOfCities() const;
private:
    std::vector<std::string> cities_name_;
    int refresh_interval_ = -1;
    int days_number_ = -1;
    std::string user_api_code_;

    void ConfigureData(const std::filesystem::path& json_file);
};