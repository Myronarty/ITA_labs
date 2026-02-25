#include "Bit_Work.h"

void BitWorker::ReadBitSequence(string& a, int b)
{
    if (in_work)
    {
        this->flush();
    }
    for (int i = 0; i < b; i++)
    {
        if (r_bit_count == 0)
        {
            char temp;
            if (file.get(temp))
            {
                r_buffer = static_cast<uint8_t>(temp);
                r_bit_count = 8;
            }
            else
            {
                return;
            }
        }
        bool bit = (r_buffer >> (r_bit_count - 1)) & 1;
        a.push_back(bit ? '1' : '0');
        r_bit_count--;
    }
}

void BitWorker::WriteBitSequence(uint64_t a, int b)
{
    in_work = 1;
    for (int i = b - 1; i >= 0; --i)
    {
        bool bit = (a >> i) & 1;

        buffer = (buffer << 1) | bit;
        bit_count++;

        if (bit_count == 8)
        {
            file.put(static_cast<char>(buffer));
            buffer = 0;
            bit_count = 0;
        }
    }
}

void BitWorker::flush()
{
    if (bit_count > 0)
    {
        buffer <<= (8 - bit_count);
        file.put(static_cast<char>(buffer));
        buffer = 0;
        bit_count = 0;
    }
    in_work = 0;
}
