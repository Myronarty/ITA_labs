#pragma once
#include "iostream"
#include "cmath"
#include "vector"
#include <fstream>
#include <string>

using namespace std;

struct BitWorker
{
    fstream& file;
    uint8_t buffer = 0;
    int bit_count = 0;
    bool in_work = false;

    uint8_t r_buffer = 0;
    int r_bit_count = 0;

    BitWorker(fstream& f) : file(f) {}

    void ReadBitSequence(string& a, int b);

    void WriteBitSequence(uint64_t a, int b);

    void flush();
};
