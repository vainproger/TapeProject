#include "tapesorter.hpp"
#include "appconfig.hpp"

int main(int argc, char** argv) {
    
    if (argc != 3) {
        std::cout << "Wrong number of arguments";
        return 0;
    }
    std::string _1 = argv[1];
    std::string _2 = argv[2];

    if (!std::filesystem::exists(_1) ) {
        std::cout << "Can't find InputTape file!";
        return 0;
    }

    if (!std::filesystem::exists(_2) ) {
        std::cout << "Can't find OutputTape file!";
        return 0;
    }

    std::string config = "..\\config";

    if (!std::filesystem::exists(config) ) {
        std::cout << "No config in Directory! See docs to configure config file";
        return 0;
    }

    try{
        AppConfig conf = AppConfig::init(config, _1, _2);
    
        TapeExecutor inputTape(_1);
        TapeExecutor outputTape(_2);

        TapeSorter sorter(&conf);
        sorter.sort();

        std::cout << "  -------------- Successfull Sorting --------------  ";
    } catch(std::string ex) {
        std::cout << ex;
    }
    return 0;
}