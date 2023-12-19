#pragma once
#include <fstream>
#include <cmath>
#include <filesystem>

class AppConfig {

public:

    static AppConfig init(std::string pathToConfig, std::string pathToInputTape, std::string pathToOutputTape) {
        AppConfig config;
        config.inputTape = pathToInputTape;
        config.outputTape = pathToOutputTape;
        std::ifstream fin(pathToConfig);
        std::string enabledRam, tmpDirectory, numberSize;
        fin >> enabledRam;
        if (enabledRam != "ram") {
            throw "No ram in config";
        } 
        fin >> config.bytesPerOneRead;
        fin >> tmpDirectory;
        if (tmpDirectory != "tmpDirectory") {
            throw "No tmpDirectory in config";
        }
        fin >> config.tmpDirectory;

        if (!std::filesystem::is_directory(std::filesystem::path(config.tmpDirectory))) {
            throw "Can't find tmpDirectory";
        }
        fin >> numberSize;
        if (numberSize != "numberSize") {
            throw "No numberSize in config";
        }
        fin >> config.numberSize;
        config.bytesPerOneRead = int(config.bytesPerOneRead / config.numberSize) * config.numberSize;
        config.tapeSize = std::filesystem::file_size(std::filesystem::path(pathToInputTape));
        config.tmpTapesCount = ceil(double(config.tapeSize) / config. bytesPerOneRead);
        config.tmpBytesPerOneRead = (config.bytesPerOneRead * 0.9) / config.tmpTapesCount;
        config.tmpBytesPerOneRead = int(config.tmpBytesPerOneRead / config.numberSize) * config.numberSize;
        config.bufferForSortingSize = config.bytesPerOneRead - config.tmpTapesCount*config.tmpBytesPerOneRead;
        return config;
    }

    std::string getTmpDirectory() {
        return tmpDirectory;
    }

    int getBytesPerOneRead() {
        return bytesPerOneRead;
    }

    int getNumberSize() {
        return numberSize;
    }

    int getTmpTapesCount() {
        return tmpTapesCount;
    }

    int getTmpBytesPerOneRead() {
        return tmpBytesPerOneRead;
    }

    int getTapeSize() {
        return tapeSize;
    }

    int getBufferForSortingSize() {
        return bufferForSortingSize;
    }
    
    std::string getPathToInputTape() {
        return inputTape;
    }
    
    std::string getPathToOutputTape() {
        return outputTape;
    }

private:
    AppConfig() { }
    std::string tmpDirectory;
    std::string inputTape;
    std::string outputTape;
    int bytesPerOneRead;
    int numberSize;
    int tmpTapesCount;
    int tmpBytesPerOneRead;
    int tapeSize;
    // Размер оставшейся памяти для отсортированных элементов во время склейки временных лент
    int bufferForSortingSize;
};