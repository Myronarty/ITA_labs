#include "Transform.h"

const size_t BLOCK_SIZE = 4096;

void BWT_EncodeFile(const string& input_file, const string& output_file)
{
    ifstream inFile(input_file, ios::binary);
    ofstream outFile(output_file, ios::binary);

    if (!inFile.is_open() || !outFile.is_open())
    {
        cerr << "Error: Cannot open files for BWT encoding." << endl;
        return;
    }

    vector<uint8_t> block(BLOCK_SIZE);

    while (inFile)
    {
        inFile.read(reinterpret_cast<char*>(block.data()), BLOCK_SIZE);
        size_t bytes_read = inFile.gcount();
        if (bytes_read == 0) break;

        uint32_t block_size32 = static_cast<uint32_t>(bytes_read);
        outFile.write(reinterpret_cast<const char*>(&block_size32), sizeof(block_size32));

        vector<int> shifts(bytes_read);
        iota(shifts.begin(), shifts.end(), 0);

        sort(shifts.begin(), shifts.end(), [&block, bytes_read](int a, int b)
            {
            for (size_t i = 0; i < bytes_read; i++)
            {
                uint8_t ca = block[(a + i) % bytes_read];
                uint8_t cb = block[(b + i) % bytes_read];
                if (ca != cb) return ca < cb;
            }
            return false;
            });

        vector<uint8_t> encoded_block(bytes_read);
        uint32_t orig_idx = 0;

        for (size_t i = 0; i < bytes_read; i++)
        {
            if (shifts[i] == 0) orig_idx = static_cast<uint32_t>(i);
            encoded_block[i] = block[(shifts[i] + bytes_read - 1) % bytes_read];
        }

        outFile.write(reinterpret_cast<const char*>(&orig_idx), sizeof(orig_idx));
        outFile.write(reinterpret_cast<const char*>(encoded_block.data()), bytes_read);
    }

    cout << "BWT Encoding completed: " << output_file << endl;
}

void BWT_DecodeFile(const string& input_file, const string& output_file)
{
    ifstream inFile(input_file, ios::binary);
    ofstream outFile(output_file, ios::binary);

    if (!inFile.is_open() || !outFile.is_open())
    {
        cerr << "Error: Cannot open files for BWT decoding." << endl;
        return;
    }

    while (inFile)
    {
        uint32_t block_size32;
        if (!inFile.read(reinterpret_cast<char*>(&block_size32), sizeof(block_size32)))
        {
            break;
        }

        uint32_t orig_idx;
        inFile.read(reinterpret_cast<char*>(&orig_idx), sizeof(orig_idx));

        vector<uint8_t> encoded_block(block_size32);
        inFile.read(reinterpret_cast<char*>(encoded_block.data()), block_size32);

        vector<int> T(block_size32);

        uint32_t count[256] = { 0 };
        for (size_t i = 0; i < block_size32; i++)
        {
            count[encoded_block[i]]++;
        }

        uint32_t pos[256] = { 0 };
        uint32_t sum = 0;
        for (int i = 0; i < 256; i++)
        {
            pos[i] = sum;
            sum += count[i];
        }

        for (size_t i = 0; i < block_size32; i++)
        {
            T[pos[encoded_block[i]]++] = i;
        }

        vector<uint8_t> decoded_block(block_size32);
        int current_pos = orig_idx;

        for (size_t i = 0; i < block_size32; i++)
        {
            current_pos = T[current_pos];
            decoded_block[i] = encoded_block[current_pos];
        }

        outFile.write(reinterpret_cast<const char*>(decoded_block.data()), block_size32);
    }

    cout << "BWT Decoding completed: " << output_file << endl;
}

void MoveToFrontShift(uint8_t alphabet[], int pos)
{
    if (pos == 0) return;

    uint8_t target = alphabet[pos];

    for (int i = pos; i > 0; i--)
    {
        alphabet[i] = alphabet[i - 1];
    }

    alphabet[0] = target;
}

void MTF_EncodeFile(const string& input_file, const string& output_file)
{
    ifstream inFile(input_file, ios::binary);
    ofstream outFile(output_file, ios::binary);

    if (!inFile.is_open() || !outFile.is_open())
    {
        cerr << "Error: Cannot open files for MTF encoding." << endl;
        return;
    }

    uint8_t alphabet[256];
    for (int i = 0; i < 256; i++)
    {
        alphabet[i] = static_cast<uint8_t>(i);
    }

    char c;
    while (inFile.get(c))
    {
        uint8_t uc = static_cast<uint8_t>(c);
        int pos = 0;

        while (pos < 256 && alphabet[pos] != uc)
        {
            pos++;
        }

        outFile.put(static_cast<char>(pos));

        MoveToFrontShift(alphabet, pos);
    }

    cout << "MTF Encoding completed: " << output_file << endl;
}

void MTF_DecodeFile(const string& input_file, const string& output_file)
{
    ifstream inFile(input_file, ios::binary);
    ofstream outFile(output_file, ios::binary);

    if (!inFile.is_open() || !outFile.is_open())
    {
        cerr << "Error: Cannot open files for MTF decoding." << endl;
        return;
    }

    uint8_t alphabet[256];
    for (int i = 0; i < 256; i++)
    {
        alphabet[i] = static_cast<uint8_t>(i);
    }

    char pos_char;
    while (inFile.get(pos_char))
    {
        int pos = static_cast<unsigned char>(pos_char);

        uint8_t c = alphabet[pos];

        outFile.put(static_cast<char>(c));

        MoveToFrontShift(alphabet, pos);
    }

    cout << "MTF Decoding completed: " << output_file << endl;
}
