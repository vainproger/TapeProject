#include "tapeexecutor.hpp"

TapeExecutor::TapeExecutor(std::string tapeFilePath) {
    this->tapeFilePath = tapeFilePath;

}

void TapeExecutor::write(char* data, int size) {
    
    fout.open(tapeFilePath, std::ios_base::app);
    fout.write(data, size);
    fout.close();
}

char* TapeExecutor::read( int blockSize, int offset ) {

    fin.open(tapeFilePath);
    fin.seekg(offset, std::ios::beg);
    char* buffer = new char[blockSize];
    fin.read(buffer, blockSize);
    fin.close();

    return buffer;
}

TapeExecutor::~TapeExecutor() {

}