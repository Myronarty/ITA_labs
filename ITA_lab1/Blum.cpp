#include "Blum.h"

vector<uint16_t> to_blocks(const string& str)
{
    vector<uint16_t> blocks;

    for (size_t i = 0; i < str.size(); i += 2)
    {
        uint16_t x = (uint8_t)str[i];

        if (i + 1 < str.size())
        {
            x += ((uint16_t)(uint8_t)str[i + 1]) << 8;
        }

        blocks.push_back(x);
    }

    return blocks;
}

uint16_t hpoly(uint16_t k, const string& str)
{
    vector<uint16_t> blocks = to_blocks(str);

    uint32_t y = 1;

    for (uint16_t x : blocks)
    {
        y = (y * k + x) % N;
    }

    return (uint16_t)y;
}

BloomFilter::BloomFilter(int s) : s(s)
{
    table.resize(N / 64, 0);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<uint16_t> dist(1, N - 1);

    for (int i = 0; i < s; i++)
    {
        keys.push_back(dist(gen));
    }
}

void BloomFilter::set_bit(uint32_t pos)
{
    table[pos / 64] |= (1ULL << (pos % 64));
}

bool BloomFilter::get_bit(uint32_t pos) const
{
    return table[pos / 64] & (1ULL << (pos % 64));
}

void BloomFilter::add(const string& item)
{
    for (int i = 0; i < s; i++)
    {
        uint16_t hash = hpoly(keys[i], item);
        set_bit(hash);
    }
}

bool BloomFilter::contains(const string& item) const
{
    for (int i = 0; i < s; i++)
    {
        uint16_t hash = hpoly(keys[i], item);
        if (!get_bit(hash))
        {
            return false;
        }
    }
    return true;
}

void BloomFilter::clear()
{
    fill(table.begin(), table.end(), 0);
}
