#include "RLE.h"
#include "Base64.h"
#include "Bit_Work.h"
#include "Huff.h"
#include "LZW.h"
#include <filesystem>

using namespace std;

void Meny_for_1_lab()
{
    bool t = 1;
    bool h = 0;
    while (t)
    {
        int a;
        cout << "what do you want to do? (1 - encode to base RLE; 0 - decode from base RLE; 2 - nothing)" << endl;
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
            CodeRLE(or_file, rez_file);
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
            deCodeRLE(or_file, rez_file);
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

namespace fs = std::filesystem;

void ProcessBatchLZW(string base_dir_path)
{
    fs::path base_path = base_dir_path;

    if (!fs::exists(base_path) || !fs::is_directory(base_path))
    {
        cerr << "Womp womp, base directory does not exist: " << base_dir_path << endl;
        return;
    }

    for (const auto& dir_entry : fs::directory_iterator(base_path))
    {
        if (dir_entry.is_directory())
        {
            string folder_name = dir_entry.path().filename().string();

            if (folder_name.ends_with("_lzw"))
            {
                continue;
            }
            if (folder_name.ends_with("_huff"))
            {
                continue;
            }

            string out_folder_name = folder_name + "_lzw";
            fs::path out_folder_path = base_path / out_folder_name;

            if (!fs::exists(out_folder_path))
            {
                fs::create_directory(out_folder_path);
            }

            cout << "\n>>> Processing folder: [" << folder_name << "] -> saving to [" << out_folder_name << "]" << endl;

            for (const auto& file_entry : fs::directory_iterator(dir_entry.path()))
            {
                if (file_entry.is_regular_file())
                {
                    string input_file = file_entry.path().string();
                    string filename = file_entry.path().filename().string();

                    string output_file = (out_folder_path / (filename + ".lzw")).string();

                    cout << "  - Encoding LZW: " << filename << " ... ";

                    CodeLZW(input_file, output_file);
                }
            }
        }
    }
    cout << "\nBatch processing complete for LZW!" << endl;
}

int main()
{
    //CodeHuffman("D:/problems/term_6/ITA/lab_1/test_subjects/im67.jpg", "D:/problems/term_6/ITA/lab_1/test_subjects/im67.jpg.huff");
    //deCodeHuffman("D:/problems/term_6/ITA/lab_1/test_subjects/im67.jpg.huff", "D:/problems/term_6/ITA/lab_1/test_subjects/im67__.jpg");
    ProcessBatchLZW("D:/problems/term_6/ITA/lab_5");
	return 0;
}
