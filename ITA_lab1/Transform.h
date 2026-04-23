#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include "Huff.h"
#include "LZW.h"
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;
//BWT
void BWT_EncodeFile(const string& input_file, const string& output_file);

void BWT_DecodeFile(const string& input_file, const string& output_file);

void Code_BWT_LZW(string input_file, string output_file, uint8_t max_bits = 12);

void deCode_LZW_BWT(string input_file, string output_file);

void Code_BWT_Huffman(string input_file, string output_file);

void deCode_Huffman_BWT(string input_file, string output_file);
//MTF
void MTF_EncodeFile(const string& input_file, const string& output_file);

void MTF_DecodeFile(const string& input_file, const string& output_file);

void Code_MTF_Huffman(string input_file, string output_file);

void deCode_Huffman_MTF(string input_file, string output_file);

void Code_MTF_LZW(string input_file, string output_file, uint8_t max_bits = 12);

void deCode_LZW_MTF(string input_file, string output_file);
//both
void Code_BWT_MTF_Huffman(string input_file, string output_file);

void deCode_Huffman_MTF_BWT(string input_file, string output_file);

void Code_BWT_MTF_LZW(string input_file, string output_file, uint8_t max_bits = 12);

void deCode_LZW_MTF_BWT(string input_file, string output_file);
