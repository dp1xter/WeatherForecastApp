add_executable(${PROJECT_NAME} main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME}
        PUBLIC config_parser
        PUBLIC request_processing
        PUBLIC interface_processing
)