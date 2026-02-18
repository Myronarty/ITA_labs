#include "RLE.h"
#include "Base64.h"

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

int main()
{
	Meny_for_1_lab();

	return 0;
}
