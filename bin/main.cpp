#include "request_processing.h"
#include "interface_processing.h"

int main(int argc, char* argv[]) {
    if (argc == 1) {
        throw std::invalid_argument("No such JSON file");
    }

    ConfigParser cfg_parser(argv[1]);
    RequestProcessor requester(cfg_parser);
    PrintForecast(requester, cfg_parser);
}