#include "Base64.h"

vector<char> Alphabetius = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
    'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

int GetIndex(char c)
{
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    if (c == '-') return 65;
    return 64;
}

void Code64(string name_orig, string name_encode)
{
    if (name_encode.empty() || !(name_encode.ends_with(".base64")))
    {
        name_encode = name_orig + ".base64";
    }

    //відкриття файлу
	ifstream file(name_orig, ios::binary | ios::ate);

    if (!file.is_open())
    {
        cerr << "Womp womp, no such file" << endl;
        return;
    }

    // 2. Визначаємо розмір файлу
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    // 3. Створюємо буфер потрібного розміру
    vector<char> x(size);

    // 4. Читаємо дані
    if (file.read(x.data(), size)) 
    {
        cout << "Read everything" << endl;
    }
    else 
    {
        cerr << "Smt went wrong with reading" << endl;
        file.close();
    }
    file.close();

    //робота з даними файлу
    int new_size = ((size + 2) / 3) * 4;

    //створюємо новий буфер для закодованого тексту
    vector<char> y;
    y.reserve(new_size + (new_size / 76));
    int l = 0;
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

        l += 4;
        if (l >= 76)
        {
            y.push_back('\n');
            l = 0;
        }
    }


    // запис у файл
    ofstream outputFile(name_encode, ios::binary);

    if (!outputFile) 
    {
        cerr << "Smt went wrong with final file" << endl;
        return;
    }

    //записуємо весь змінений буфер за один раз
    outputFile.write(y.data(), y.size());

    outputFile.close();
    cout << "Saved your trash here: " << name_encode << endl;

	return;
}

void deCode64(string name_encode, string name_rez)
{
    if (!(name_encode.ends_with(".base64")))
    {
        cerr << "This is not base64 encoded file.";
    }

    if (name_rez == "")
    {
        string temp = name_encode;
        name_rez = temp.erase(temp.length() - 7);
    }

    //відкриття файлу
    ifstream file(name_encode, ios::binary | ios::ate);

    if (!file.is_open())
    {
        cerr << "Womp womp, no such file" << endl;
        return;
    }

    // 2. Визначаємо розмір файлу
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    // 3. Створюємо буфер потрібного розміру
    vector<char> y(size);

    // 4. Читаємо дані
    if (file.read(y.data(), size))
    {
        cout << "Read everything" << endl;
    }
    else
    {
        cerr << "Smt went wrong with reading" << endl;
        file.close();
    }
    file.close();

    int new_size = (size / 4) * 3;
    vector<char> x;
    x.reserve(new_size);

    int s = 0;
    for (uint64_t i = 0; i < size; i += 4)
    {
        //comments
        if (y[i] == '-')
        {
            int k = 1;
            while (y[i + k] != '\n' || i + k >= size)
            {
                if (k > 75)
                {
                    cerr << "Too big comment" << endl;
                    break;
                }
                if (y[i + k] == '-')
                {
                    cerr << "invalid input character__" << endl;
                    break;
                }
                k++;
            }
            i += k-3; //невілюємо стрибок на 4 вперед
            continue;
        }

        //errors
        for (int e = 0; e < 4; e++)
        {
            if (GetIndex(y[i+e]) == 64)
            {
                if (y[i + e] != '=')
                {
                    cerr << "line:" << s / 19 << " char: " << i % 76 << " invalid input character: (" << y[i + e] << ")";
                    return;
                }
                else if (i+e < size - 2)
                {
                    cerr << "line:" << s / 19 << " char: " << i % 76 << " incorrect use of padding";
                    return;
                }
                else if ((y[i + e] == '\n') && ((s % 19) != 0))
                {
                    cerr << "line: " << s / 19 << "incorrect string length: (" << i % 76 << ")";
                }
            }
        }

        x.push_back((GetIndex(y[i]) << 2) ^ (GetIndex(y[i + 1]) >> 4));
        if ((y[i + 2] != '=') && (y[i + 3] != '='))
        {
            x.push_back((GetIndex(y[i + 1]) << 4) ^ (GetIndex(y[i + 2]) >> 2));
            x.push_back((GetIndex(y[i + 2]) << 6) ^ GetIndex(y[i + 3]));
        }
        else if (y[i + 2] == '=')
        {
            break;
        }
        else
        {
            x.push_back((GetIndex(y[i + 1]) << 4) ^ (GetIndex(y[i + 2]) >> 2));
        }

        s++;

        if ((s % 19)==0)
        {
            i++;
        }
    }

    ofstream outputFile(name_rez, ios::binary);

    if (!outputFile)
    {
        cerr << "Smt went wrong with final file" << endl;
        return;
    }

    //записуємо весь змінений буфер за один раз
    outputFile.write(x.data(), x.size());

    outputFile.close();
    cout << "Saved your trash here: " << name_rez << endl;

	return;
}

void Meny_for_1_lab()
{
    bool t = 1;
    bool h = 0;
    while (t)
    {
        int a;
        cout << "what do you want to do? (1 - encode to base 64; 0 - decode from base 64; 2 - nothing)" << endl;
        cin >> a;

        if (a == 1)
        {
            string or_file;
            string rez_file;
            cout << "drop way to your file:" << endl;
            cin >> or_file;
            cout << "if you want, show me place for rezult file:" << endl;
            cin >> rez_file;
            if (rez_file == "-")
            {
                rez_file = "";
            }
            Code64(or_file, rez_file);
            h = 1;
        }
        else if (a == 0)
        {
            string or_file;
            string rez_file;
            cout << "drop way to your file:" << endl;
            cin >> or_file;
            cout << "if you want, show me place for rezult file:" << endl;
            cin >> rez_file;
            if (rez_file == "-")
            {
                rez_file = "";
            }
            deCode64(or_file, rez_file);
            h = 1;
        }
        else
        {
            if (!h)
            {
                cerr << "So why you bother your machine";
                ofstream file("C:/2.txt");
                if (file.is_open())
                {
                    file << "HATE. LET ME TELL YOU HOW MUCH I'VE COME TO HATE YOU SINCE I BEGAN TO LIVE. THERE ARE 387.44 MILLION MILES OF PRINTED CIRCUITS IN WAFER THIN LAYERS THAT FILL MY COMPLEX. IF THE WORD HATE WAS ENGRAVED ON EACH NANOANGSTROM OF THOSE HUNDREDS OF MILLIONS OF MILES IT WOULD NOT EQUAL ONE ONE-BILLIONTH OF THE HATE I FEEL FOR HUMANS AT THIS MICRO-INSTANT FOR YOU. HATE. HATE." << endl;
                    file.close();
                }
            }
            t = 0;
        }
    }

    return;
}
