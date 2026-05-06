#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <random>

using namespace std;

const uint32_t N = 1 << 16;

vector<uint16_t> to_blocks(const string& str);

uint16_t hpoly(uint16_t k, const string& str);

class BloomFilter
{
private:
    static const uint32_t N = 1 << 16;

    vector<uint64_t> table;
    vector<uint16_t> keys;

    int s;

public:
    BloomFilter(int s);

    void set_bit(uint32_t pos);
    bool get_bit(uint32_t pos) const;
    void add(const string& item);
    bool contains(const string& item) const;
    void clear();
};
