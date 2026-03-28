#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include "Bit_Work.h"

using namespace std;

void CodeLZW(string name_orig, string name_encode, uint8_t max_bits = 12);

void deCodeLZW(string name_encode, string name_rez);
