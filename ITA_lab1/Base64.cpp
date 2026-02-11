#include "Base64.h"

vector<char> Alphabetius = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '\\' };

void Code64(string name_orig, string name_encode)
{
    if (name_encode.empty() || !(name_encode.ends_with(".base64")))
    {
        name_encode = name_orig + ".base64";
    }

    //відкриття файлу
	std::ifstream file(name_orig, std::ios::binary | std::ios::ate);

    if (!file.is_open())
    {
        std::cerr << "Womp womp, no such file" << std::endl;
        return;
    }

    // 2. Визначаємо розмір файлу
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // 3. Створюємо буфер потрібного розміру
    std::vector<char> x(size);

    // 4. Читаємо дані
    // read приймає (char*, кількість_байтів)
    if (file.read(x.data(), size)) 
    {
        std::cout << "Read everything" << std::endl;
    }
    else 
    {
        std::cerr << "Smt went wrong with reading" << std::endl;
        file.close();
    }
    file.close();

    //робота з даними файлу
    int new_size = ((size + 2) / 3) * 4;;

    //створюємо новий буфер для закодованого тексту
    vector<char> y;
    y.reserve(new_size);
    for (int i = 0; i < size; i=i+3)
    {
        unsigned char b1 = (unsigned char)x[i];
        unsigned char b2 = (i + 1 < size) ? (unsigned char)x[i + 1] : 0;
        unsigned char b3 = (i + 2 < size) ? (unsigned char)x[i + 2] : 0;

        unsigned char y_1 = b1 >> 2;

        unsigned char y_2 = ((b1 & 0x03) << 4) | (b2 >> 4);

        unsigned char y_3 = ((b2 & 0x0F) << 2) | (b3 >> 6);

        unsigned char y_4 = b3 & 0x3F;

        y.push_back(Alphabetius[y_1]);
        y.push_back(Alphabetius[y_2]);

        if (i + 1 < size)
        {
           y.push_back(Alphabetius[y_3]);
        }
        else 
        {
            y.push_back('=');
        }

        if (i + 2 < size) 
        {
            y.push_back(Alphabetius[y_4]);
        }
        else 
        {
            y.push_back('=');
        }
    }


    // запис у файл
    std::ofstream outputFile(name_encode, std::ios::binary);

    if (!outputFile) 
    {
        std::cerr << "Smt went wrong with final file" << std::endl;
        return;
    }

    // Записуємо весь змінений буфер за один раз
    outputFile.write(y.data(), y.size());

    outputFile.close();
    std::cout << "Saved your trash here: " << name_encode << std::endl;

	return;
}

void deCode64(string name_encode, string name_rez)
{

	return;
}