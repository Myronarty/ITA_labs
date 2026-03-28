#include "LZW.h"

void CodeLZW(string name_orig, string name_encode, uint8_t max_bits)
{
    ifstream inFile(name_orig, ios::binary);
    if (!inFile.is_open())
    {
        cerr << "Error: Cannot open input file." << endl;
        return;
    }

    fstream outFile(name_encode, ios::binary | ios::out | ios::trunc);
    if (!outFile.is_open())
    {
        cerr << "Error: Cannot create output file." << endl;
        inFile.close();
        return;
    }

    outFile.put(static_cast<char>(max_bits));

    BitWorker bw(outFile);
    uint32_t d_size = 1 << max_bits;

    map<vector<uint8_t>, uint32_t> dictionary;
    for (uint32_t i = 0; i < 256; i++)
    {
        dictionary[{static_cast<uint8_t>(i)}] = i;
    }
    uint32_t next_code = 256;

    vector<uint8_t> S;
    uint8_t c;

    if (inFile.read(reinterpret_cast<char*>(&c), 1))
    {
        S.push_back(c);
    }
    else
    {
        inFile.close();
        outFile.close();
        return;
    }
    while (inFile.read(reinterpret_cast<char*>(&c), 1))
    {
        vector<uint8_t> Sc = S;
        Sc.push_back(c);

        if (dictionary.count(Sc))
        {
            S = Sc;
        }
        else
        {
            uint32_t I = dictionary[S];
            bw.WriteBitSequence(I, max_bits);

            if (next_code < d_size)
            {
                dictionary[Sc] = next_code++;
            }
            
            S = { c };
        }
    }

    uint32_t I = dictionary[S];
    bw.WriteBitSequence(I, max_bits);
    bw.flush();

    inFile.close();
    outFile.close();
    cout << "LZW compression done: " << name_encode << " (Max bits: " << (int)max_bits << ")" << endl;
}

void deCodeLZW(string name_encode, string name_rez)
{
    fstream inFile(name_encode, ios::binary | ios::in);
    if (!inFile.is_open())
    {
        cerr << "Error: Cannot open compressed file." << endl;
        return;
    }

    char first_byte;
    if (!inFile.get(first_byte)) return;
    uint8_t max_bits = static_cast<uint8_t>(first_byte);
    uint32_t d_size = 1 << max_bits;

    ofstream outFile(name_rez, ios::binary);
    if (!outFile.is_open())
    {
        cerr << "Error: Cannot create output file." << endl;
        inFile.close();
        return;
    }

    vector<vector<uint8_t>> dictionary;
    dictionary.reserve(d_size);
    for (int i = 0; i < 256; i++)
    {
        dictionary.push_back({ static_cast<uint8_t>(i) });
    }

    BitWorker bw(inFile);
    uint32_t old_code, new_code;
    uint8_t c;

    if (!bw.ReadBits(old_code, max_bits))
    {
        inFile.close(); outFile.close(); return;
    }

    vector<uint8_t> S = dictionary[old_code];
    outFile.write(reinterpret_cast<char*>(S.data()), S.size());
    c = S[0];

    while (bw.ReadBits(new_code, max_bits))
    {
        vector<uint8_t> current_S;

        if (new_code < dictionary.size())
        {
            current_S = dictionary[new_code];
        }
        else if (new_code == dictionary.size())
        {
            current_S = dictionary[old_code];
            current_S.push_back(c);
        }
        else
        {
            cerr << "Error: Corrupted compressed file." << endl;
            break;
        }

        outFile.write(reinterpret_cast<char*>(current_S.data()), current_S.size());

        c = current_S[0];

        if (dictionary.size() < d_size)
        {
            vector<uint8_t> new_entry = dictionary[old_code];
            new_entry.push_back(c);
            dictionary.push_back(new_entry);
        }

        old_code = new_code;
    }

    inFile.close();
    outFile.close();
    cout << "LZW decompression done: " << name_rez << endl;
}
