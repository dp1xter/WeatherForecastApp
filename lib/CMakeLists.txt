add_library(request_processing include/request_processing.h src/request_processing.cpp)
add_library(config_parser include/config_parser.h src/config_parser.cpp)
add_library(interface_processing include/interface_processing.h src/interface_processing.cpp)

target_include_directories(request_processing PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(request_processing
        PUBLIC nlohmann_json::nlohmann_json
        PUBLIC cpr::cpr
        PUBLIC config_parser
)

link_libraries(request_processing settings)

target_link_libraries(config_parser
        PUBLIC nlohmann_json::nlohmann_json
)

target_link_libraries(interface_processing
        PUBLIC ftxui::component
        PUBLIC request_processing
)