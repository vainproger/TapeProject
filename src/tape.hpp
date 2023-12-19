#pragma once
#include <vector>

class Tape {

public:

    virtual ~Tape() { }

    virtual char* read( int elements, int offset ) = 0;
    virtual void write(char* data, int size) = 0;
};