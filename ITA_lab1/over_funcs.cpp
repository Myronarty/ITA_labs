#include "Transform.h"

namespace fs = std::filesystem;

void Code_BWT_Huffman(string input_file, string output_file)
{
    string temp_file = input_file + ".temp_bwt";

    cout << "Applying BWT..." << endl;
    BWT_EncodeFile(input_file, temp_file);

    cout << "Applying Huffman..." << endl;
    CodeHuffman(temp_file, output_file);

    if (fs::exists(temp_file)) {
        fs::remove(temp_file);
    }

    cout << "BWT+Huffman compression done: " << output_file << endl;
}

void deCode_Huffman_BWT(string input_file, string output_file)
{
    string temp_file = output_file + ".temp_bwt";

    cout << "Decoding Huffman..." << endl;
    deCodeHuffman(input_file, temp_file);

    cout << "Reversing BWT..." << endl;
    BWT_DecodeFile(temp_file, output_file);

    if (fs::exists(temp_file))
    {
        fs::remove(temp_file);
    }

    cout << "Huffman+BWT decompression done: " << output_file << endl;
}

void Code_BWT_LZW(string input_file, string output_file, uint8_t max_bits)
{
    string temp_file = input_file + ".temp_bwt";

    cout << "Applying BWT..." << endl;
    BWT_EncodeFile(input_file, temp_file);

    cout << "Applying LZW..." << endl;
    CodeLZW(temp_file, output_file, max_bits);

    if (fs::exists(temp_file))
    {
        fs::remove(temp_file);
    }

    cout << "BWT+LZW compression done: " << output_file << endl;
}

void deCode_LZW_BWT(string input_file, string output_file)
{
    string temp_file = output_file + ".temp_bwt";

    cout << "Decoding LZW..." << endl;
    deCodeLZW(input_file, temp_file);

    cout << "Reversing BWT..." << endl;
    BWT_DecodeFile(temp_file, output_file);

    if (fs::exists(temp_file))
    {
        fs::remove(temp_file);
    }

    cout << "LZW+BWT decompression done: " << output_file << endl;
}

void Code_MTF_Huffman(string input_file, string output_file)
{
    string temp_file = input_file + ".temp_mtf";

    cout << "Applying MTF..." << endl;
    MTF_EncodeFile(input_file, temp_file);

    cout << "Applying Huffman..." << endl;
    CodeHuffman(temp_file, output_file);

    if (fs::exists(temp_file)) fs::remove(temp_file);
    cout << "MTF+Huffman compression done!" << endl;
}

void deCode_Huffman_MTF(string input_file, string output_file)
{
    string temp_file = output_file + ".temp_mtf";

    cout << "Decoding Huffman..." << endl;
    deCodeHuffman(input_file, temp_file);

    cout << "Reversing MTF..." << endl;
    MTF_DecodeFile(temp_file, output_file);

    if (fs::exists(temp_file)) fs::remove(temp_file);
    cout << "Huffman+MTF decompression done!" << endl;
}

void Code_BWT_MTF_Huffman(string input_file, string output_file)
{
    string temp_bwt = input_file + ".temp_bwt";
    string temp_mtf = input_file + ".temp_mtf";

    cout << "Applying BWT..." << endl;
    BWT_EncodeFile(input_file, temp_bwt);

    cout << "Applying MTF..." << endl;
    MTF_EncodeFile(temp_bwt, temp_mtf);

    cout << "Applying Huffman..." << endl;
    CodeHuffman(temp_mtf, output_file);

    if (fs::exists(temp_bwt)) fs::remove(temp_bwt);
    if (fs::exists(temp_mtf)) fs::remove(temp_mtf);

    cout << "BWT+MTF+Huffman compression done: " << output_file << endl;
}

void deCode_Huffman_MTF_BWT(string input_file, string output_file)
{
    string temp_mtf = output_file + ".temp_mtf";
    string temp_bwt = output_file + ".temp_bwt";

    cout << "Decoding Huffman..." << endl;
    deCodeHuffman(input_file, temp_mtf);

    cout << "Reversing MTF..." << endl;
    MTF_DecodeFile(temp_mtf, temp_bwt);

    cout << "Reversing BWT..." << endl;
    BWT_DecodeFile(temp_bwt, output_file);

    // Прибираємо за собою
    if (fs::exists(temp_mtf)) fs::remove(temp_mtf);
    if (fs::exists(temp_bwt)) fs::remove(temp_bwt);

    cout << "Huffman+MTF+BWT decompression done: " << output_file << endl;
}

void Code_BWT_MTF_LZW(string input_file, string output_file, uint8_t max_bits)
{
    string temp_bwt = input_file + ".temp_bwt";
    string temp_mtf = input_file + ".temp_mtf";

    cout << "[Pipeline] 1. Applying BWT..." << endl;
    BWT_EncodeFile(input_file, temp_bwt);

    cout << "[Pipeline] 2. Applying MTF..." << endl;
    MTF_EncodeFile(temp_bwt, temp_mtf);

    cout << "[Pipeline] 3. Applying LZW..." << endl;
    CodeLZW(temp_mtf, output_file, max_bits);

    // Прибираємо за собою
    if (fs::exists(temp_bwt)) fs::remove(temp_bwt);
    if (fs::exists(temp_mtf)) fs::remove(temp_mtf);

    cout << ">>> BWT+MTF+LZW compression done: " << output_file << endl;
}

void deCode_LZW_MTF_BWT(string input_file, string output_file)
{
    string temp_mtf = output_file + ".temp_mtf";
    string temp_bwt = output_file + ".temp_bwt";

    cout << "[Pipeline] 1. Decoding LZW..." << endl;
    deCodeLZW(input_file, temp_mtf);

    cout << "[Pipeline] 2. Reversing MTF..." << endl;
    MTF_DecodeFile(temp_mtf, temp_bwt);

    cout << "[Pipeline] 3. Reversing BWT..." << endl;
    BWT_DecodeFile(temp_bwt, output_file);

    // Прибираємо за собою
    if (fs::exists(temp_mtf)) fs::remove(temp_mtf);
    if (fs::exists(temp_bwt)) fs::remove(temp_bwt);

    cout << ">>> LZW+MTF+BWT decompression done: " << output_file << endl;
}

void Code_MTF_LZW(string input_file, string output_file, uint8_t max_bits)
{
    string temp_mtf = input_file + ".temp_mtf";

    cout << "[Pipeline] 1. Applying MTF..." << endl;
    MTF_EncodeFile(input_file, temp_mtf);

    cout << "[Pipeline] 2. Applying LZW..." << endl;
    CodeLZW(temp_mtf, output_file, max_bits);

    if (fs::exists(temp_mtf)) {
        fs::remove(temp_mtf);
    }

    cout << ">>> MTF+LZW compression done: " << output_file << endl;
}

void deCode_LZW_MTF(string input_file, string output_file)
{
    string temp_mtf = output_file + ".temp_mtf";

    cout << "[Pipeline] 1. Decoding LZW..." << endl;
    deCodeLZW(input_file, temp_mtf);

    cout << "[Pipeline] 2. Reversing MTF..." << endl;
    MTF_DecodeFile(temp_mtf, output_file);

    // Прибираємо за собою
    if (fs::exists(temp_mtf)) {
        fs::remove(temp_mtf);
    }

    cout << ">>> LZW+MTF decompression done: " << output_file << endl;
}
