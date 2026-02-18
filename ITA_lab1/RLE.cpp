#include "RLE.h"

void CodeRLE(string name_orig, string name_encode)
{
    if (name_encode.empty() || !(name_encode.ends_with(".rle")))
    {
        name_encode = name_orig + ".rle";
    }

    ifstream file(name_orig, ios::binary | ios::ate);
    if (!file.is_open())
    {
        cerr << "Womp womp, no such file" << endl;
        return;
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<uint8_t> x(size);
    file.read(reinterpret_cast<char*>(x.data()), size);
    file.close();

    vector<uint8_t> y;
    int t = 1;
    for (int i = 0; i < size; i++)
    {
        if (i + 1 < size && x[i] == x[i + 1] && t < 63)
        {
            t++;
        }
        else
        {
            if (t > 1 || (x[i] & 0x80))
            {
                if (x[i] & 0x80)
                {
                    y.push_back(static_cast<uint8_t>(t | 0xC0));
                }
                else
                {
                    y.push_back(static_cast<uint8_t>(t | 0x80));
                }
                y.push_back(x[i] & 0x7F);
            }
            else
            {
                y.push_back(x[i]);
            }
            t = 1;
        }
    }

    ofstream outputFile(name_encode, ios::binary);
    outputFile.write(reinterpret_cast<const char*>(y.data()), y.size());
    outputFile.close();

    cout << "Success! Saved to: " << name_encode << endl;
    return;
}

void deCodeRLE(string name_encode, string name_rez)
{
    if (!(name_encode.ends_with(".rle")))
    {
        cerr << "This is not rle encoded file." << endl;
        return;
    }

    if (name_rez == "")
    {
        string temp = name_encode;
        name_rez = temp.erase(temp.length() - 4);
    }

    ifstream file(name_encode, ios::binary | ios::ate);
    if (!file.is_open())
    {
        cerr << "No file found." << endl;
        return;
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    vector<uint8_t> y(size);
    file.read(reinterpret_cast<char*>(y.data()), size);
    file.close();

    vector<uint8_t> x;
    x.reserve(size * 2);

    for (int i = 0; i < y.size(); i++)
    {
        if (y[i] & 0x80)
        {
            if (i + 1 < y.size())
            {
                uint8_t d = y[i + 1];
                if (y[i] & 0x40)
                {
                    d |= 0x80;
                }

                for (int j = 0; j < (y[i] & 0x3F); j++)
                {
                    x.push_back(d);
                }
                i++;
            }
        }
        else
        {
            x.push_back(y[i]);
        }
    }

    ofstream outputFile(name_rez, ios::binary);
    outputFile.write(reinterpret_cast<const char*>(x.data()), x.size());
    outputFile.close();

    cout << "File restored to: " << name_rez << endl;
    return;
}
