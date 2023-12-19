#include "tapesorter.hpp"
#include "tapeutils.hpp"

void TapeSorter::sortChunks() {
        
    TapeExecutor* bigTape = new TapeExecutor(config->getPathToInputTape());
    size_t val = 0;
    while (val * config->getBytesPerOneRead() < config->getTapeSize()) {

        int bytesToRead = ((val + 1)*config->getBytesPerOneRead() > config->getTapeSize()) ?
            config->getTapeSize() - val*config->getBytesPerOneRead() :
            config->getBytesPerOneRead();

        char* chunk = bigTape->read(bytesToRead, val*config->getBytesPerOneRead());
        TapeExecutor* tmpTape = new TapeExecutor(config->getTmpDirectory() + "\\tmptape_" + std::to_string(val));

        std::vector<int> chunkNumbers = TapeUtils::convertBytesToVectorAndClear(chunk, config->getNumberSize(), config->getBytesPerOneRead());
        int size = chunkNumbers.size() * config->getNumberSize();
        std::sort(chunkNumbers.begin(), chunkNumbers.end());
        char* chunkToWrite = TapeUtils::convertVectorToBytesAndClear(chunkNumbers);
        tmpTape->write(chunkToWrite, size);
        delete tmpTape;
        delete[] chunkToWrite;
        val++;
    }
}

void TapeSorter::combineChunks() {

    TapeExecutor* outputTape = new TapeExecutor(config->getPathToOutputTape());
    // Временные ленты
    std::vector<std::vector<int>> tapes(config->getTmpTapesCount());
    // Пустая ли i-ая временная лента
    std::vector<bool> isEmpty(config->getTmpTapesCount(), 0);
    // Количество считываний из каждой ленты
    std::vector<int> reads(config->getTmpTapesCount(), 0);
    // Первое считывание из всех временных лент
    for (size_t fileNumber = 0; fileNumber < config->getTmpTapesCount(); fileNumber++) {
        TapeExecutor* tmpTape = new TapeExecutor(config->getTmpDirectory() + "\\tmptape_" + std::to_string(fileNumber));
        char* buffer = tmpTape->read(config->getTmpBytesPerOneRead(), 0);
        reads[fileNumber]++;
        tapes[fileNumber] = TapeUtils::convertBytesToVectorAndClear(buffer, config->getNumberSize(), config->getTmpBytesPerOneRead());
        delete tmpTape;
    }
    
    int sizeOfSortedVector = config->getBufferForSortingSize() / config->getNumberSize();
    std::vector<int> sortedVector;
    while (*std::min_element(isEmpty.begin() , isEmpty.end()) == 0) {

        size_t i = 0;
        int minVal = INT_MAX, index = 0;
        while (i < sizeOfSortedVector) {
            for (size_t j = 0; j < tapes.size(); j++) {
                if (tapes[j].empty()) {
                    continue;
                }
                if (minVal > tapes[j][0]) {
                    minVal = tapes[j][0];
                    index = j;
                }
            }
            sortedVector.push_back(minVal);
            i++;
            minVal = INT_MAX;
            tapes[index].erase(tapes[index].begin());
            
            if (tapes[index].empty()) {
                TapeExecutor* tmpTape = new TapeExecutor(config->getTmpDirectory() + "\\tmptape_" + std::to_string(index));
                if (reads[index] * config->getTmpBytesPerOneRead() < config->getBytesPerOneRead()) {
                    char* buffer = tmpTape->read(config->getTmpBytesPerOneRead(), reads[index] * config->getTmpBytesPerOneRead());
                    reads[index]++;
                    tapes[index] = TapeUtils::convertBytesToVectorAndClear(buffer, config->getNumberSize(), config->getTmpBytesPerOneRead());
                    // в последней временной ленте может быть меньше элементов чем во всех других
                    if (tapes[index].empty()) {
                        isEmpty[index] = true;
                        std::filesystem::remove(config->getTmpDirectory() + "\\tmptape_" + std::to_string(index));
                    }
                } else {
                    isEmpty[index] = true;
                    std::filesystem::remove(config->getTmpDirectory() + "\\tmptape_" + std::to_string(index));
                }
                delete tmpTape;
            }
        }
        int size = sortedVector.size() * config->getNumberSize();
        char* buffer = TapeUtils::convertVectorToBytesAndClear(sortedVector);
        outputTape->write(buffer, size);
        delete[] buffer;
    }
    delete outputTape;
}