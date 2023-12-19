#pragma once

#include "tape.hpp"
#include <fstream>
#include <cstring>

class TapeExecutor : public Tape {
public:

    TapeExecutor(std::string tapeFilePath);
    ~TapeExecutor();

    virtual char* read( int elements, int offset );
    virtual void write(char* data, int size);
private:
    std::string tapeFilePath;
    std::ifstream fin;
    std::ofstream fout;
};