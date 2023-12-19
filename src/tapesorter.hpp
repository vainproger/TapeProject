#pragma once
#include <iostream>
#include <algorithm>
#include <filesystem>
#include "appconfig.hpp"
#include "tapeexecutor.hpp"

class TapeSorter {
public:

    TapeSorter(AppConfig* config) {
        this->config = config;
    }

    void sort() {
        sortChunks();
        combineChunks();
    }
private:

    //! Сортирует считанные чанки из bigTape
    //! Записывает отсортированные элементы во временную ленту
    void sortChunks();

    //! Объединяет чанки из директории pathToDirectory в выходную ленту pathToOutTape
    void combineChunks();

    AppConfig* config;
};