#include "lib/include/interface_processing.h"

ftxui::Element Icon::ClearSky() {
    return ftxui::vbox({
        ftxui::text(" \\\\ . /"),
        ftxui::text(" - .-. -"),
        ftxui::text("‒ (   ) ‒"),
        ftxui::text(" . `-᾿ ."),
        ftxui::text("  / ' \\\\")
    });
}

ftxui::Element Icon::PartlyCloudy() {
    return ftxui::vbox({
        ftxui::text("_`/\"\".-."),
        ftxui::text(" ,\\_(   )."),
        ftxui::text("  /(___(__)")
    });
}

ftxui::Element Icon::Cloudy() {
    return ftxui::vbox({
        ftxui::text("    .--."),
        ftxui::text(" .-(    )."),
        ftxui::text("(___.__)__)")
    });
}

ftxui::Element Icon::Snow() {
    return ftxui::vbox({
        ftxui::text("  .-."),
        ftxui::text(" (   )."),
        ftxui::text("(___(__)"),
        ftxui::text(" *  *  *"),
        ftxui::text("*  *  *")
    });
}

ftxui::Element Icon::Rain() {
    return ftxui::vbox({
        ftxui::text("  .-."),
        ftxui::text(" (   )."),
        ftxui::text("(___(__)"),
        ftxui::text(" ' ' ' '"),
        ftxui::text("' ' ' '")
    });
}

ftxui::Element Icon::Fog() {
    return ftxui::vbox({
        ftxui::text("_ - _ - _ -"),
        ftxui::text(" _ - _ - _"),
        ftxui::text("_ - _ - _ -"),
        ftxui::text(" _ - _ - _")
    });
}

ftxui::Element Icon::Thunderstorm() {
    return ftxui::vbox({
        ftxui::text("   .-."),
        ftxui::text("  (   )."),
        ftxui::text(" (___(__)"),
        ftxui::text(" ,'7',7,'"),
        ftxui::text(",','7','")
    });
}

ftxui::Element Icon::Drizzle() {
    return ftxui::vbox({
        ftxui::text("_`/\"\".-."),
        ftxui::text(" ,\\_(   )."),
        ftxui::text("  /(___(__)"),
        ftxui::text("    ' , ' ,"),
        ftxui::text("   , ' , '")
    });
}

ftxui::Element Icon::Showers() {
    return ftxui::vbox({
        ftxui::text("   .-."),
        ftxui::text("  (   )."),
        ftxui::text(" (___(__)"),
        ftxui::text(" ','\"','"),
        ftxui::text("'\"','\"'")
    });
}

std::pair<std::string, ftxui::Element> GetConditionNameAndIcon(int wmo_code) {
    switch (wmo_code) {
        case 0: return {"Clear sky", Icon::ClearSky()};
        case 1: return {"Mainly clear", Icon::PartlyCloudy()};
        case 2: return {"Partly cloudy", Icon::PartlyCloudy()};
        case 3: return {"Overcast", Icon::Cloudy()};
        case 45: return {"Fog", Icon::Fog()};
        case 48: return {"Depositing rime fog", Icon::Fog()};
        case 51: return {"Light drizzle", Icon::Drizzle()};
        case 53: return {"Moderate drizzle", Icon::Drizzle()};
        case 55: return {"Dense drizzle", Icon::Drizzle()};
        case 56: return {"Light freezing drizzle", Icon::Drizzle()};
        case 57: return {"Dense freezing drizzle", Icon::Drizzle()};
        case 61: return {"Slight rain", Icon::Rain()};
        case 63: return {"Moderate rain", Icon::Rain()};
        case 65: return {"Heavy rain", Icon::Rain()};
        case 66: return {"Light freezing rain", Icon::Rain()};
        case 67: return {"Heavy freezing rain", Icon::Rain()};
        case 71: return {"Slight snow fall", Icon::Snow()};
        case 73: return {"Moderate snow fall", Icon::Snow()};
        case 75: return {"Heavy snow fall", Icon::Snow()};
        case 77: return {"Snow grains", Icon::Snow()};
        case 80: return {"Slight rain showers", Icon::Showers()};
        case 81: return {"Moderate rain showers", Icon::Showers()};
        case 82: return {"Violent rain showers", Icon::Showers()};
        case 85: return {"Slight snow showers", Icon::Showers()};
        case 86: return {"Heavy snow showers", Icon::Showers()};
        case 95: return {"Moderate thunderstorm", Icon::Thunderstorm()};
        case 96: return {"Thunderstorm with slight hail", Icon::Thunderstorm()};
        case 99: return {"Thunderstorm with heavy hail", Icon::Thunderstorm()};
        default: throw std::invalid_argument("Invalid WMO code");
    }
}

ftxui::Element GetTimeWeather(const DayDescription& day, const TimeTable& time) {
    const std::pair<std::string, ftxui::Element> condition_data = GetConditionNameAndIcon(day.At(time).GetCode());
    std::string time_name;
    switch (time) {
        case 0: {
            time_name = "Morning";
            break;
        }
        case 1: {
            time_name = "Afternoon";
            break;
        }
        case 2: {
            time_name = "Evening";
            break;
        }
        case 3: {
            time_name = "Night";
            break;
        }
        default: throw std::invalid_argument("Invalid day time");
    }

    auto text_info = ftxui::vbox({
        ftxui::filler(),
        ftxui::text("Temperature: " + std::to_string(day.At(time).GetTemperature()) + " °C"),
        ftxui::text("Wind speed: " + std::to_string(day.At(time).GetWindSpeed()) + " km/h"),
        ftxui::text("Visibility: " + std::to_string(day.At(time).GetVisibility()) + " m"),
        ftxui::text("Precipitation: " + std::to_string(day.At(time).GetPrecipitation()) + " mm"),
        ftxui::filler()
    }) | ftxui::color(ftxui::Color::White) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, kDayTimeWindowHeight);

    auto data_block = ftxui::vbox({
        ftxui::text(condition_data.first) | ftxui::center | ftxui::color(ftxui::Color::Orange1),
        ftxui::filler(),
        ftxui::hbox({
            ftxui::filler(),
            condition_data.second | ftxui::color(ftxui::Color::Orange1) | ftxui::center | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, kIconWidth) | ftxui::flex,
            ftxui::filler(),
            text_info,
            ftxui::filler()
        }),
        ftxui::filler()
    });

    auto constructed_window = ftxui::window(ftxui::text(time_name) | ftxui::color(ftxui::Color::Purple) | ftxui::center, ftxui::vbox({
        data_block
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, kDayTimeWindowWidth) | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, kDayTimeWindowHeight));

    return constructed_window;
}

std::shared_ptr<ftxui::Node> SuccessfulTerminalOutput(const City& city, int start_day) {
    ftxui::Elements days_forecast;
    for (int j = start_day; j < start_day + kNextPrintableDays; ++j) {
        ftxui::Element current_day_data = ftxui::window(ftxui::text(city.At(j).GetCalendarDate()) | ftxui::center , ftxui::hbox({
            GetTimeWeather(city.At(j), TimeTable::kMorning),
            ftxui::filler(),
            GetTimeWeather(city.At(j), TimeTable::kAfternoon),
            ftxui::filler(),
            GetTimeWeather(city.At(j), TimeTable::kEvening),
            ftxui::filler(),
            GetTimeWeather(city.At(j), TimeTable::kNight)
        })) | ftxui::color(ftxui::Color::White) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, kMainWindowWidth);
        days_forecast.push_back(current_day_data);
    }

    auto constructed_window = ftxui::vbox({
        ftxui::hbox({
            ftxui::filler(),
            ftxui::borderDouble(ftxui::text("   " + city.GetName() + "   ")) | ftxui::color(ftxui::Color::Aquamarine1),
            ftxui::filler()
        }),
        ftxui::vbox(days_forecast)
    }) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, kMainWindowWidth);

    return constructed_window;
}

std::shared_ptr<ftxui::Node> FailureTerminalOutput(const UpdatingStatus& validating_response_from_api) {
    std::string failure_name;
    switch (validating_response_from_api) {
        case UpdatingStatus::kApiNinjasNoConnection: {
            failure_name = "No connection to the site API Ninjas";
            break;
        }
        case UpdatingStatus::kOpenMeteoNoConnection: {
            failure_name = "No connection to the site Open Meteo";
            break;
        }
        case UpdatingStatus::kInvalidCityName: {
            failure_name = "Invalid city name in the json file";
            break;
        }
        default: throw std::invalid_argument("Undefined problem");
    }

    return ftxui::text(failure_name) | ftxui::color(ftxui::Color::Red);
}

void PrintForecast(RequestProcessor& requester, const ConfigParser& cfg_parser) {
    long long current_city = 0;
    long long start_day = 0;
    clock_t previous_timer = clock();
    UpdatingStatus validating_response_from_api;

    auto screen = ftxui::ScreenInteractive::TerminalOutput();

    auto data_printer = ftxui::Renderer([&] {
        clock_t current_timer = clock();

        if ((static_cast<double> (current_timer - previous_timer) / (CLOCKS_PER_SEC * kSecondsPerMinute) >= cfg_parser.GetInterval()) || requester.HasFirstUpdate()) {
            validating_response_from_api = requester.UpdateData();
            previous_timer = current_timer;
        }

        if (validating_response_from_api != UpdatingStatus::kSuccess) {
            return FailureTerminalOutput(validating_response_from_api);
        }

        return SuccessfulTerminalOutput(requester.GetCityData(current_city), start_day);
    }) | ftxui::CatchEvent([&](const ftxui::Event& event) {
        if (event.character() == "n") {
            current_city = std::min(current_city + 1, static_cast<long long> (cfg_parser.GetCountOfCities() - 1));
        } else if (event.character() == "p") {
            current_city = std::max(current_city - 1, 0ll);
        } else if (event.character() == "u") {
            validating_response_from_api = requester.UpdateData();
            previous_timer = clock();
        } else if (event == ftxui::Event::Character("+")) {
            start_day = std::min(start_day + 1, static_cast<long long> (kMaxIndexOfDay));
        } else if (event == ftxui::Event::Character("-")) {
            start_day = std::max(start_day - 1, 0ll);
        } else if (event == ftxui::Event::Escape) {
            screen.ExitLoopClosure()();
        } else {
            return false;
        }

        return true;
    });

    screen.Loop(data_printer);
}