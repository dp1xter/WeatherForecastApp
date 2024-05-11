#pragma once

#include "request_processing.h"
#include <utility>
#include <ctime>
#include <cstdio>
#include <stdexcept>
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include <ftxui/screen/screen.hpp>
#include <ftxui/screen/color.hpp>
#include "ftxui/component/component.hpp"

static const size_t kMainWindowWidth = 170;
static const size_t kDayTimeWindowWidth = 35;
static const size_t kDayTimeWindowHeight = 7;
static const size_t kIconWidth = 12;
static const size_t kSecondsPerMinute = 60;
static const size_t kMaxIndexOfDay = 12;
static const int kNextPrintableDays = 3;

enum TimeTable {
    kMorning = 0,
    kAfternoon = 1,
    kEvening = 2,
    kNight = 3
};

namespace Icon {
    ftxui::Element ClearSky();

    ftxui::Element PartlyCloudy();

    ftxui::Element Cloudy();

    ftxui::Element Snow();

    ftxui::Element Rain();

    ftxui::Element Fog();

    ftxui::Element Thunderstorm();

    ftxui::Element Drizzle();

    ftxui::Element Showers();
}

std::pair<std::string, ftxui::Element> GetConditionNameAndIcon(int wmo_code);

ftxui::Element GetTimeWeather(const DayDescription& day, const TimeTable& time);

std::shared_ptr<ftxui::Node> SuccessfulTerminalOutput(const City& city, int current_days_count);

std::shared_ptr<ftxui::Node> FailureTerminalOutput(const UpdatingStatus& validating_response_from_api);

void PrintForecast(RequestProcessor& requester, const ConfigParser& cfg_parser);