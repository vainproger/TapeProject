#pragma once
#include <vector>
#include <cmath>
#include <string>

namespace TapeUtils {


    bool isNumber(const std::string& str) {
        return std::find_if(str.begin(), str.end(), [](unsigned char c) { return !std::isdigit(c); }) == str.end();
    }

    //! Переводит массив байтов в вектор и очищает место из-под data
    std::vector<int> convertBytesToVectorAndClear(char* data, int bytePerNumber, int dataSize) {

        if (dataSize % bytePerNumber != 0) {
            return std::vector<int>();
        }
        std::vector<int> numbers(dataSize / bytePerNumber);
        for (int i = 0, j = 0; i < dataSize; i += bytePerNumber, j++) {
            std::string str(&data[i], bytePerNumber);
            if (isNumber(str)) {
                numbers[j] = std::stoi(str);
            } else {
                break;
            }
        }
        numbers.erase(std::remove(numbers.begin(), numbers.end(), 0), numbers.end());
        delete[] data;
        return numbers;
    }

    //! Переводит вектор в массив байтов и очищает место из-под data динамически
    char* convertVectorToBytesAndClear(std::vector<int>& data) {
        
        int size = 0, index = 0;
        for (int& elem : data) {
            size += std::log10(elem) + 1;
        }
        char* result = new char[size];
        while (data.size() > 0){
            std::string str = std::to_string(data[0]);
            for (size_t i = 0; i < str.size(); i++, index++) {
                result[index] = str[i];
            }
            data.erase(data.begin());
        }
        return result;
    }
}