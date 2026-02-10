#include "Base64.h"

void Code64(string name_orig, string name_encode)
{
    //відкриття файлу
	std::ifstream file("name_orig", std::ios::binary | std::ios::ate);

    if (!file.is_open()) 
    {
        std::cerr << "Womp womp, no such file" << std::endl;
        return;
    }

    // 2. Визначаємо розмір файлу
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // 3. Створюємо буфер потрібного розміру
    std::vector<char> buffer(size);

    // 4. Читаємо дані
    // read приймає (char*, кількість_байтів)
    if (file.read(buffer.data(), size)) 
    {
        std::cout << "Read everything" << std::endl;
    }
    else 
    {
        std::cerr << "Smt went wrong with reading" << std::endl;
    }
    file.close();

    //робота з даними файлу
    int new_size;
    bool tri;
    if ((size / 3) * 3 == size)
    {
        new_size = (size / 3) * 4;
        tri = 1;
    }
    else
    {
        new_size = (size / 3) * 4 + 1;
        tri = 0;
    }
    //створюємо новий буфер для закодованого тексту
    vector<char> new_buffer(new_size);
    for (int i = 0; i < size; i++)
    {
        //тут розбиття на блоки, можливо винести в окрему функцію
    }


    // запис у файл
    std::ofstream outputFile(name_encode, std::ios::binary);

    if (!outputFile) 
    {
        std::cerr << "Не вдалося створити вихідний файл!" << std::endl;
        return;
    }

    // Записуємо весь змінений буфер за один раз
    outputFile.write(buffer.data(), buffer.size());

    outputFile.close();
    std::cout << "Файл успішно збережено як: " << name_encode << std::endl;

	return;
}

void deCode64(string name_encode, string name_rez)
{

	return;
}